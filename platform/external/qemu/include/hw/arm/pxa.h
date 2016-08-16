/*
 * Intel XScale PXA255/270 processor support.
 *
 * Copyright (c) 2006 Openedhand Ltd.
 * Written by Andrzej Zaborowski <balrog@zabor.org>
 *
 * This code is licenced under the GNU GPL v2.
 */
#ifndef PXA_H
# define PXA_H			"hw/arm/pxa.h"

#include "cpu.h"

# define PXA2XX_PIC_SSP3	0
# define PXA2XX_PIC_USBH2	2
# define PXA2XX_PIC_USBH1	3
# define PXA2XX_PIC_KEYPAD	4
# define PXA2XX_PIC_PWRI2C	6
# define PXA25X_PIC_HWUART	7
# define PXA27X_PIC_OST_4_11	7
# define PXA2XX_PIC_GPIO_0	8
# define PXA2XX_PIC_GPIO_1	9
# define PXA2XX_PIC_GPIO_X	10
# define PXA2XX_PIC_I2S 	13
# define PXA26X_PIC_ASSP	15
# define PXA25X_PIC_NSSP	16
# define PXA27X_PIC_SSP2	16
# define PXA2XX_PIC_LCD		17
# define PXA2XX_PIC_I2C		18
# define PXA2XX_PIC_ICP		19
# define PXA2XX_PIC_STUART	20
# define PXA2XX_PIC_BTUART	21
# define PXA2XX_PIC_FFUART	22
# define PXA2XX_PIC_MMC		23
# define PXA2XX_PIC_SSP		24
# define PXA2XX_PIC_DMA		25
# define PXA2XX_PIC_OST_0	26
# define PXA2XX_PIC_RTC1HZ	30
# define PXA2XX_PIC_RTCALARM	31

# define PXA2XX_RX_RQ_I2S	2
# define PXA2XX_TX_RQ_I2S	3
# define PXA2XX_RX_RQ_BTUART	4
# define PXA2XX_TX_RQ_BTUART	5
# define PXA2XX_RX_RQ_FFUART	6
# define PXA2XX_TX_RQ_FFUART	7
# define PXA2XX_RX_RQ_SSP1	13
# define PXA2XX_TX_RQ_SSP1	14
# define PXA2XX_RX_RQ_SSP2	15
# define PXA2XX_TX_RQ_SSP2	16
# define PXA2XX_RX_RQ_ICP	17
# define PXA2XX_TX_RQ_ICP	18
# define PXA2XX_RX_RQ_STUART	19
# define PXA2XX_TX_RQ_STUART	20
# define PXA2XX_RX_RQ_MMCI	21
# define PXA2XX_TX_RQ_MMCI	22
# define PXA2XX_USB_RQ(x)	((x) + 24)
# define PXA2XX_RX_RQ_SSP3	66
# define PXA2XX_TX_RQ_SSP3	67

# define PXA2XX_SDRAM_BASE	0xa0000000
# define PXA2XX_INTERNAL_BASE	0x5c000000
# define PXA2XX_INTERNAL_SIZE	0x40000

qemu_irq *pxa2xx_pic_init(hwaddr base, CPUOldState *env);

void pxa25x_timer_init(hwaddr base, qemu_irq *irqs);
void pxa27x_timer_init(hwaddr base, qemu_irq *irqs, qemu_irq irq4);

typedef struct PXA2xxGPIOInfo PXA2xxGPIOInfo;
PXA2xxGPIOInfo *pxa2xx_gpio_init(hwaddr base,
                CPUOldState *env, qemu_irq *pic, int lines);
qemu_irq *pxa2xx_gpio_in_get(PXA2xxGPIOInfo *s);
void pxa2xx_gpio_out_set(PXA2xxGPIOInfo *s,
                int line, qemu_irq handler);
void pxa2xx_gpio_read_notifier(PXA2xxGPIOInfo *s, qemu_irq handler);

typedef struct PXA2xxDMAState PXA2xxDMAState;
PXA2xxDMAState *pxa255_dma_init(hwaddr base,
                qemu_irq irq);
PXA2xxDMAState *pxa27x_dma_init(hwaddr base,
                qemu_irq irq);
void pxa2xx_dma_request(PXA2xxDMAState *s, int req_num, int on);

typedef struct PXA2xxLCDState PXA2xxLCDState;
PXA2xxLCDState *pxa2xx_lcdc_init(hwaddr base,
                qemu_irq irq);
void pxa2xx_lcd_vsync_notifier(PXA2xxLCDState *s, qemu_irq handler);
void pxa2xx_lcdc_oritentation(void *opaque, int angle);

typedef struct PXA2xxMMCIState PXA2xxMMCIState;
PXA2xxMMCIState *pxa2xx_mmci_init(hwaddr base,
                BlockDriverState *bd, qemu_irq irq, void *dma);
void pxa2xx_mmci_handlers(PXA2xxMMCIState *s, qemu_irq readonly,
                qemu_irq coverswitch);

typedef struct PXA2xxPCMCIAState PXA2xxPCMCIAState;
PXA2xxPCMCIAState *pxa2xx_pcmcia_init(hwaddr base);
int pxa2xx_pcmcia_attach(void *opaque, PCMCIACardState *card);
int pxa2xx_pcmcia_dettach(void *opaque);
void pxa2xx_pcmcia_set_irq_cb(void *opaque, qemu_irq irq, qemu_irq cd_irq);

struct  keymap {
    int column;
    int row;
};
typedef struct PXA2xxKeyPadState PXA2xxKeyPadState;
PXA2xxKeyPadState *pxa27x_keypad_init(hwaddr base,
                qemu_irq irq);
void pxa27x_register_keypad(PXA2xxKeyPadState *kp, struct keymap *map,
                int size);

typedef struct PXA2xxI2CState PXA2xxI2CState;
PXA2xxI2CState *pxa2xx_i2c_init(hwaddr base,
                qemu_irq irq, uint32_t page_size);
i2c_bus *pxa2xx_i2c_bus(PXA2xxI2CState *s);

typedef struct PXA2xxI2SState PXA2xxI2SState;
typedef struct PXA2xxFIrState PXA2xxFIrState;

typedef struct {
    CPUOldState *env;
    qemu_irq *pic;
    qemu_irq reset;
    PXA2xxDMAState *dma;
    PXA2xxGPIOInfo *gpio;
    PXA2xxLCDState *lcd;
    SSIBus **ssp;
    PXA2xxI2CState *i2c[2];
    PXA2xxMMCIState *mmc;
    PXA2xxPCMCIAState *pcmcia[2];
    PXA2xxI2SState *i2s;
    PXA2xxFIrState *fir;
    PXA2xxKeyPadState *kp;

    
    hwaddr pm_base;
    uint32_t pm_regs[0x40];

    
    hwaddr cm_base;
    uint32_t cm_regs[4];
    uint32_t clkcfg;

    
    hwaddr mm_base;
    uint32_t mm_regs[0x1a];

    
    uint32_t pmnc;

    
    hwaddr rtc_base;
    uint32_t rttr;
    uint32_t rtsr;
    uint32_t rtar;
    uint32_t rdar1;
    uint32_t rdar2;
    uint32_t ryar1;
    uint32_t ryar2;
    uint32_t swar1;
    uint32_t swar2;
    uint32_t piar;
    uint32_t last_rcnr;
    uint32_t last_rdcr;
    uint32_t last_rycr;
    uint32_t last_swcr;
    uint32_t last_rtcpicr;
    int64_t last_hz;
    int64_t last_sw;
    int64_t last_pi;
    QEMUTimer *rtc_hz;
    QEMUTimer *rtc_rdal1;
    QEMUTimer *rtc_rdal2;
    QEMUTimer *rtc_swal1;
    QEMUTimer *rtc_swal2;
    QEMUTimer *rtc_pi;
} PXA2xxState;

struct PXA2xxI2SState {
    qemu_irq irq;
    PXA2xxDMAState *dma;
    void (*data_req)(void *, int, int);

    uint32_t control[2];
    uint32_t status;
    uint32_t mask;
    uint32_t clk;

    int enable;
    int rx_len;
    int tx_len;
    void (*codec_out)(void *, uint32_t);
    uint32_t (*codec_in)(void *);
    void *opaque;

    int fifo_len;
    uint32_t fifo[16];
};

# define PA_FMT			"0x%08lx"
# define REG_FMT		"0x" TARGET_FMT_plx

PXA2xxState *pxa270_init(unsigned int sdram_size, const char *revision);
PXA2xxState *pxa255_init(unsigned int sdram_size);

void usb_ohci_init_pxa(hwaddr base, int num_ports, int devfn,
                       qemu_irq irq);

#endif	