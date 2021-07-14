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
  uint8_t uart ## uart_number ## _rxchar(void) \
  { return uart_rxchar(&Uart##uart_number);}

/**
 * Transmit a single byte. Polling mode, waits until finished
 */
#define UART_TXCHAR(uart_number) \
  uint8_t uart##uart_number##_txchar(uint8_t c) \
  { return uart_txchar(&Uart##uart_number, c); }
/**
 * Transmit a buffer. Waits indefinitely for a UART TX mutex,
 * returns number of transferred bytes or -1 in case of an error.
 * Synchronous API.
 */
#define UART_TXBUFFER(uart_number) \
  int uart##uart_number##_txbuffer(uint8_t *ptr, int len) \
  { return uart_txbuffer(&Uart##uart_number, ptr, len); }


#define UART_RXBUFFER(uart_number) \
  int uart##uart_number##_rxbuffer(uint8_t *ptr, int len) \
  { return uart_rxbuffer(&Uart##uart_number, ptr, len);}


#if BSP_USE_UART0
bool uart0_rxready(void);
uint8_t uart0_rxchar(void);
uint8_t uart0_txchar(uint8_t c);
int uart0_rxbuffer(uint8_t *ptr, int len);
int uart0_txbuffer(uint8_t *ptr, int len);
void uart0_init(void);
#endif

#if BSP_USE_UART1
bool uart1_rxready(void);
uint8_t uart1_rxuint8_t(void);
uint8_t uart1_txuint8_t(uint8_t c);
int uart1_rxbuffer(uint8_t *ptr, int len);
int uart1_txbuffer(uint8_t *ptr, int len);
void uart1_init(void);
#endif

#if BSP_USE_UART2
bool uart2_rxready(void);
uint8_t uart2_rxuint8_t(void);
uint8_t uart2_txuint8_t(uint8_t c);
int uart2_rxbuffer(uint8_t *ptr, int len);
int uart2_txbuffer(uint8_t *ptr, int len);
void uart2_init(void);
#endif

#endif
