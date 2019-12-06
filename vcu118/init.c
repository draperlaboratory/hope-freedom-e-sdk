#include "uart.h"

extern int main(void);

void _init(void)
{
  int result;

  uart0_init();

  result = main();

  // isp_run_app should exit upon seeing this message
  printf("Program exited with code: %d\n", result);

  for(;;);
}
