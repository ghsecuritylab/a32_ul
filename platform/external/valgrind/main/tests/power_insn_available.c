#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <setjmp.h>

typedef enum exit_codes_ {

#if defined(VGA_ppc32) || defined(VGA_ppc64)
  
  POWER_INSN_AVAILABLE    = 0,
  
  POWER_INSN_UNAVAILABLE  = 1,
  
  POWER_INSN_UNRECOGNIZED = 2,

  
  USAGE_ERROR
#else
  
  NOT_POWER_ARCH          = 255,
#endif

} exit_code;

#if defined(VGA_ppc32) || defined(VGA_ppc64)
static jmp_buf unsup_insn_env;
static void unsup_insn_handler(int signal_number)
{
  if (signal_number == SIGILL)
    longjmp(unsup_insn_env, 1);
  return;
}
static struct sigaction unsup_insn_action = (struct sigaction) {
  .sa_handler = &unsup_insn_handler,
};

static bool dcbzl_available(void)
{
#define MAX_DCBZL_SZB (128) 
  char *test_block = NULL;
  register char *rb asm ("r14");
  int err;
  bool dcbzl_exists = false;

  err = posix_memalign ((void **)&test_block, MAX_DCBZL_SZB, 4 * MAX_DCBZL_SZB);
  if (err) {
    fprintf(stderr, "posix_memalign() failed (err = %d [%s])\n", err, strerror(err));
    return err;
  }

  rb = test_block;

  if (setjmp(unsup_insn_env) != 0)
    dcbzl_exists = false;
  else {
    sigaction(SIGILL, &unsup_insn_action, NULL);
    asm volatile ("dcbzl 0, %[RB]" : : [RB] "r" (rb));
    dcbzl_exists = true;
  }

  free(test_block);
  return dcbzl_exists;
}
#endif

int main(int argc, char **argv)
{
  exit_code status;

#if defined(VGA_ppc32) || defined(VGA_ppc64)
  char *insn;
  if (argc != 2) {
    fprintf(stderr, "usage: power_insn_available <insn>\n" );
    exit(USAGE_ERROR);
  }

  insn = argv[1];
  if (strcmp (insn, "dcbzl") == 0)
    status = ((dcbzl_available ()) ? POWER_INSN_AVAILABLE : POWER_INSN_UNAVAILABLE);
  else
    
    status = POWER_INSN_UNRECOGNIZED;
#else
    status = NOT_POWER_ARCH;
#endif
  return status;
}
