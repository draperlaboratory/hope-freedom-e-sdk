#ifndef __UART_16550_H__
#define __UART_16550_H__

#include "bsp.h"
#include <stdbool.h>

/**
 * Initialize UART peripheral
 */
#define UART_INIT(uart_number)	    \
  void uart##uart_number##_init(void) \
  { uart_init(&Uart ## uart_number, XPAR_UARTNS550_ ## uart_number ## _DEVICE_ID , 0); }

/**
 * Return 1 if UART has at leas 1 byte in the RX FIFO
 */
#define UART_RXREADY(uart_number) \
  bool uart##uart_number##_rxready(void) \
  {return uart_rxready(&Uart##uart_number);}

/**
 * Receive a single byte. Polling mode, waits until finished
 */
#define UART_RXCHAR(uart_number) \
  char uart ## uart_number ## _rxchar(void) \
  { return (char)uart_rxchar(&Uart##uart_number);}

/**
 * Transmit a single byte. Polling mode, waits until finished
 */
#define UART_TXCHAR(uart_number) \
  char uart##uart_number##_txchar(char c) \
  { return (char)uart_txchar(&Uart##uart_number, (uint8_t)c); }
/**
 * Transmit a buffer. Waits indefinitely for a UART TX mutex,
 * returns number of transferred bytes or -1 in case of an error.
 * Synchronous API.
 */
#define UART_TXBUFFER(uart_number) \
  int uart##uart_number##_txbuffer(char *ptr, int len) \
  { return uart_txbuffer(&Uart##uart_number, (uint8_t *)ptr, len); }


#define UART_RXBUFFER(uart_number) \
  int uart##uart_number##_rxbuffer(char *ptr, int len) \
  { return uart_rxbuffer(&Uart##uart_number, (uint8_t *)ptr, len);}


#if BSP_USE_UART0
bool uart0_rxready(void);
char uart0_rxchar(void);
char uart0_txchar(char c);
int uart0_rxbuffer(char *ptr, int len);
int uart0_txbuffer(char *ptr, int len);
void uart0_init(void);
#endif

#if BSP_USE_UART1
bool uart1_rxready(void);
char uart1_rxchar(void);
char uart1_txchar(char c);
int uart1_rxbuffer(char *ptr, int len);
int uart1_txbuffer(char *ptr, int len);
void uart1_init(void);
#endif

#if BSP_USE_UART2
bool uart2_rxready(void);
char uart2_rxchar(void);
char uart2_txchar(char c);
int uart2_rxbuffer(char *ptr, int len);
int uart2_txbuffer(char *ptr, int len);
void uart2_init(void);
#endif

#endif
