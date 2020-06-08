#include <stdint.h>
#include "uart.h"

extern int main(void);

static void start(void)
{
  int result = main();
  // isp_run_app should exit upon seeing this message
  printf("Program exited with code: %d\n", result);
  exit(result);
}

void _init(void)
{
  int result;

  uart0_init();

  // set up floating point computation
  uintptr_t misa, mstatus;
  asm volatile("csrr %0, misa" : "=r"(misa));
  if (misa & (1 << ('f' - 'a'))) {
    asm volatile("csrr %0, mstatus" : "=r"(mstatus));
    mstatus |= 0x6000;
    asm volatile("csrw mstatus, %0; csrw frm, 0" :: "r"(mstatus));
  }

#if USE_VM == 1
  vm_boot(start);
#else
  start();
#endif
}
