#include "hw/hw.h"
#include "hw/boards.h"
#include "hw/i386/pc.h"
#include "hw/isa/isa.h"

#include "cpu.h"
#include "sysemu/kvm.h"

static void cpu_put_seg(QEMUFile *f, SegmentCache *dt)
{
    qemu_put_be32(f, dt->selector);
    qemu_put_betl(f, dt->base);
    qemu_put_be32(f, dt->limit);
    qemu_put_be32(f, dt->flags);
}

static void cpu_get_seg(QEMUFile *f, SegmentCache *dt)
{
    dt->selector = qemu_get_be32(f);
    dt->base = qemu_get_betl(f);
    dt->limit = qemu_get_be32(f);
    dt->flags = qemu_get_be32(f);
}

void cpu_save(QEMUFile *f, void *opaque)
{
    CPUX86State *env = opaque;
    uint16_t fptag, fpus, fpuc, fpregs_format;
    uint32_t hflags;
    int32_t a20_mask;
    int i;

    cpu_synchronize_state(ENV_GET_CPU(env), 0);

    for(i = 0; i < CPU_NB_REGS; i++)
        qemu_put_betls(f, &env->regs[i]);
    qemu_put_betls(f, &env->eip);
    qemu_put_betls(f, &env->eflags);
    hflags = env->hflags; 
    qemu_put_be32s(f, &hflags);

    
    fpuc = env->fpuc;
    fpus = (env->fpus & ~0x3800) | (env->fpstt & 0x7) << 11;
    fptag = 0;
    for(i = 0; i < 8; i++) {
        fptag |= ((!env->fptags[i]) << i);
    }

    qemu_put_be16s(f, &fpuc);
    qemu_put_be16s(f, &fpus);
    qemu_put_be16s(f, &fptag);

    fpregs_format = 1;
    qemu_put_be16s(f, &fpregs_format);

    for(i = 0; i < 8; i++) {
        qemu_put_be64(f, env->fpregs[i].mmx.MMX_Q(0));
    }

    for(i = 0; i < 6; i++)
        cpu_put_seg(f, &env->segs[i]);
    cpu_put_seg(f, &env->ldt);
    cpu_put_seg(f, &env->tr);
    cpu_put_seg(f, &env->gdt);
    cpu_put_seg(f, &env->idt);

    qemu_put_be32s(f, &env->sysenter_cs);
    qemu_put_betls(f, &env->sysenter_esp);
    qemu_put_betls(f, &env->sysenter_eip);

    qemu_put_betls(f, &env->cr[0]);
    qemu_put_betls(f, &env->cr[2]);
    qemu_put_betls(f, &env->cr[3]);
    qemu_put_betls(f, &env->cr[4]);

    for(i = 0; i < 8; i++)
        qemu_put_betls(f, &env->dr[i]);

    
    a20_mask = (int32_t) env->a20_mask;
    qemu_put_sbe32s(f, &a20_mask);

    
    qemu_put_be32s(f, &env->mxcsr);
    for(i = 0; i < CPU_NB_REGS; i++) {
        qemu_put_be64s(f, &env->xmm_regs[i].XMM_Q(0));
        qemu_put_be64s(f, &env->xmm_regs[i].XMM_Q(1));
    }

#ifdef TARGET_X86_64
    qemu_put_be64s(f, &env->efer);
    qemu_put_be64s(f, &env->star);
    qemu_put_be64s(f, &env->lstar);
    qemu_put_be64s(f, &env->cstar);
    qemu_put_be64s(f, &env->fmask);
    qemu_put_be64s(f, &env->kernelgsbase);
#endif
    qemu_put_be32s(f, &env->smbase);

    qemu_put_be64s(f, &env->pat);
    qemu_put_be32s(f, &env->hflags2);

    qemu_put_be64s(f, &env->vm_hsave);
    qemu_put_be64s(f, &env->vm_vmcb);
    qemu_put_be64s(f, &env->tsc_offset);
    qemu_put_be64s(f, &env->intercept);
    qemu_put_be16s(f, &env->intercept_cr_read);
    qemu_put_be16s(f, &env->intercept_cr_write);
    qemu_put_be16s(f, &env->intercept_dr_read);
    qemu_put_be16s(f, &env->intercept_dr_write);
    qemu_put_be32s(f, &env->intercept_exceptions);
    qemu_put_8s(f, &env->v_tpr);

    
    for(i = 0; i < 11; i++)
        qemu_put_be64s(f, &env->mtrr_fixed[i]);
    qemu_put_be64s(f, &env->mtrr_deftype);
    for(i = 0; i < 8; i++) {
        qemu_put_be64s(f, &env->mtrr_var[i].base);
        qemu_put_be64s(f, &env->mtrr_var[i].mask);
    }

    for (i = 0; i < sizeof(env->interrupt_bitmap)/8; i++) {
        qemu_put_be64s(f, &env->interrupt_bitmap[i]);
    }
    qemu_put_be64s(f, &env->tsc);
    qemu_put_be32s(f, &env->mp_state);

    
    qemu_put_be64s(f, &env->mcg_cap);
    if (env->mcg_cap) {
        qemu_put_be64s(f, &env->mcg_status);
        qemu_put_be64s(f, &env->mcg_ctl);
        for (i = 0; i < (env->mcg_cap & 0xff); i++) {
            qemu_put_be64s(f, &env->mce_banks[4*i]);
            qemu_put_be64s(f, &env->mce_banks[4*i + 1]);
            qemu_put_be64s(f, &env->mce_banks[4*i + 2]);
            qemu_put_be64s(f, &env->mce_banks[4*i + 3]);
        }
    }
}

int cpu_load(QEMUFile *f, void *opaque, int version_id)
{
    CPUX86State *env = opaque;
    int i, guess_mmx;
    uint32_t hflags;
    uint16_t fpus, fpuc, fptag, fpregs_format;
    int32_t a20_mask;

    if (version_id < 3 || version_id > CPU_SAVE_VERSION)
        return -EINVAL;
    for(i = 0; i < CPU_NB_REGS; i++)
        qemu_get_betls(f, &env->regs[i]);
    qemu_get_betls(f, &env->eip);
    qemu_get_betls(f, &env->eflags);
    qemu_get_be32s(f, &hflags);

    qemu_get_be16s(f, &fpuc);
    qemu_get_be16s(f, &fpus);
    qemu_get_be16s(f, &fptag);
    qemu_get_be16s(f, &fpregs_format);

    guess_mmx = ((fptag == 0xff) && (fpus & 0x3800) == 0);
    for(i = 0; i < 8; i++) {
        uint64_t mant;
        uint16_t exp;

        switch(fpregs_format) {
        case 0:
            mant = qemu_get_be64(f);
            exp = qemu_get_be16(f);
            
            if (guess_mmx)
                env->fpregs[i].mmx.MMX_Q(0) = mant;
            else
                env->fpregs[i].d = cpu_set_fp80(mant, exp);
            break;
        case 1:
            mant = qemu_get_be64(f);
            env->fpregs[i].mmx.MMX_Q(0) = mant;
            break;
        default:
            return -EINVAL;
        }
    }

    env->fpuc = fpuc;
    
    env->fpstt = (fpus >> 11) & 7;
    env->fpus = fpus & ~0x3800;
    fptag ^= 0xff;
    for(i = 0; i < 8; i++) {
        env->fptags[i] = (fptag >> i) & 1;
    }

    for(i = 0; i < 6; i++)
        cpu_get_seg(f, &env->segs[i]);
    cpu_get_seg(f, &env->ldt);
    cpu_get_seg(f, &env->tr);
    cpu_get_seg(f, &env->gdt);
    cpu_get_seg(f, &env->idt);

    qemu_get_be32s(f, &env->sysenter_cs);
    if (version_id >= 7) {
        qemu_get_betls(f, &env->sysenter_esp);
        qemu_get_betls(f, &env->sysenter_eip);
    } else {
        env->sysenter_esp = qemu_get_be32(f);
        env->sysenter_eip = qemu_get_be32(f);
    }

    qemu_get_betls(f, &env->cr[0]);
    qemu_get_betls(f, &env->cr[2]);
    qemu_get_betls(f, &env->cr[3]);
    qemu_get_betls(f, &env->cr[4]);

    for(i = 0; i < 8; i++)
        qemu_get_betls(f, &env->dr[i]);
    cpu_breakpoint_remove_all(env, BP_CPU);
    cpu_watchpoint_remove_all(env, BP_CPU);
    for (i = 0; i < 4; i++)
        hw_breakpoint_insert(env, i);

    
    qemu_get_sbe32s(f, &a20_mask);
    env->a20_mask = a20_mask;

    qemu_get_be32s(f, &env->mxcsr);
    for(i = 0; i < CPU_NB_REGS; i++) {
        qemu_get_be64s(f, &env->xmm_regs[i].XMM_Q(0));
        qemu_get_be64s(f, &env->xmm_regs[i].XMM_Q(1));
    }

#ifdef TARGET_X86_64
    qemu_get_be64s(f, &env->efer);
    qemu_get_be64s(f, &env->star);
    qemu_get_be64s(f, &env->lstar);
    qemu_get_be64s(f, &env->cstar);
    qemu_get_be64s(f, &env->fmask);
    qemu_get_be64s(f, &env->kernelgsbase);
#endif
    if (version_id >= 4) {
        qemu_get_be32s(f, &env->smbase);
    }
    if (version_id >= 5) {
        qemu_get_be64s(f, &env->pat);
        qemu_get_be32s(f, &env->hflags2);
        if (version_id < 6)
            qemu_get_be32s(f, &ENV_GET_CPU(env)->halted);

        qemu_get_be64s(f, &env->vm_hsave);
        qemu_get_be64s(f, &env->vm_vmcb);
        qemu_get_be64s(f, &env->tsc_offset);
        qemu_get_be64s(f, &env->intercept);
        qemu_get_be16s(f, &env->intercept_cr_read);
        qemu_get_be16s(f, &env->intercept_cr_write);
        qemu_get_be16s(f, &env->intercept_dr_read);
        qemu_get_be16s(f, &env->intercept_dr_write);
        qemu_get_be32s(f, &env->intercept_exceptions);
        qemu_get_8s(f, &env->v_tpr);
    }

    if (version_id >= 8) {
        
        for(i = 0; i < 11; i++)
            qemu_get_be64s(f, &env->mtrr_fixed[i]);
        qemu_get_be64s(f, &env->mtrr_deftype);
        for(i = 0; i < 8; i++) {
            qemu_get_be64s(f, &env->mtrr_var[i].base);
            qemu_get_be64s(f, &env->mtrr_var[i].mask);
        }
    }
    if (version_id >= 9) {
        for (i = 0; i < sizeof(env->interrupt_bitmap)/8; i++) {
            qemu_get_be64s(f, &env->interrupt_bitmap[i]);
        }
        qemu_get_be64s(f, &env->tsc);
        qemu_get_be32s(f, &env->mp_state);
    }

    if (version_id >= 10) {
        qemu_get_be64s(f, &env->mcg_cap);
        if (env->mcg_cap) {
            qemu_get_be64s(f, &env->mcg_status);
            qemu_get_be64s(f, &env->mcg_ctl);
            for (i = 0; i < (env->mcg_cap & 0xff); i++) {
                qemu_get_be64s(f, &env->mce_banks[4*i]);
                qemu_get_be64s(f, &env->mce_banks[4*i + 1]);
                qemu_get_be64s(f, &env->mce_banks[4*i + 2]);
                qemu_get_be64s(f, &env->mce_banks[4*i + 3]);
            }
        }
    }


    
    
    env->hflags = hflags;
    tlb_flush(env, 1);
    cpu_synchronize_state(ENV_GET_CPU(env), 1);
    return 0;
}
