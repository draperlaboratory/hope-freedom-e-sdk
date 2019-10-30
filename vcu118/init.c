#include "uart.h"

extern int main(void);

void _init(void)
{
  uart0_init();

  main();
  for(;;);
}
