#include <stdint.h>
#include <string.h> // for `memset`
#include "uart.h"

/* Xilinx driver includes. */
#include "xuartns550.h"
#include "bsp.h"

/*****************************************************************************/
/* Defines */

#define UART_TX_DELAY pdMS_TO_TICKS(500)
#define UART_RX_DELAY pdMS_TO_TICKS(10000)
#define UART_BUFFER_SIZE 254

/*****************************************************************************/
/* Structs */

/* Device driver for UART peripheral */
struct UartDriver
{
    XUartNs550 Device; /* Xilinx Uart driver */
    /* Counters used to determine when buffer has been send and received */
    volatile int TotalReceivedCount;
    volatile int TotalSentCount;
    volatile int TotalErrorCount;
    volatile uint8_t Errors;
    int tx_len;                 /* Length of TX transaction */
    int rx_len;                 /* Length of RX transaction */
};

/*****************************************************************************/
/* Static functions, macros etc */

static bool uart_rxready(struct UartDriver *Uart);
static uint8_t uart_rxchar(struct UartDriver *Uart);
static uint8_t uart_txchar(struct UartDriver *Uart, uint8_t c);
static int uart_rxbuffer(struct UartDriver *Uart, uint8_t *ptr, int len);
static int uart_txbuffer(struct UartDriver *Uart, uint8_t *ptr, int len);
static void uart_init(struct UartDriver *Uart, uint8_t device_id, uint8_t plic_source_id);

/*****************************************************************************/
/* Global defines */

/* Instance of UART device */
#if BSP_USE_UART0
static XUartNs550 UartNs550_0;
struct UartDriver Uart0;
#endif

#if BSP_USE_UART1
static XUartNs550 UartNs550_1;
struct UartDriver Uart1;
#endif

#if BSP_USE_UART2
static XUartNs550 UartNs550_2;
struct UartDriver Uart2;
#endif

/*****************************************************************************/
/* Peripheral specific definitions */
#if BSP_USE_UART0
/**
 * Initialize UART 0 peripheral
 */
UART_INIT(0)
UART_RXREADY(0)
UART_RXCHAR(0)
UART_TXCHAR(0)
UART_TXBUFFER(0)
UART_RXBUFFER(0)

#endif /* BSP_USE_UART0 */

#if BSP_USE_UART1

UART_INIT(1)
UART_RXREADY(1)
UART_RXCHAR(1)
UART_TXCHAR(1)
UART_TXBUFFER(1)
UART_RXBUFFER(1)

#endif /* BSP_USE_UART1 */

#if BSP_USE_UART2

UART_INIT(2)
UART_RXREADY(2)
UART_RXCHAR(2)
UART_TXCHAR(2)
UART_TXBUFFER(2)
UART_RXBUFFER(2)

#endif /* BSP_USE_UART2 */


/*****************************************************************************/
/* Driver specific defintions */

static bool uart_rxready(struct UartDriver *Uart)
{
    return (bool)XUartNs550_IsReceiveData(Uart->Device.BaseAddress);
}

/**
 * Initialize UART peripheral
 */
static void uart_init(struct UartDriver *Uart, uint8_t device_id, uint8_t plic_source_id)
{
    switch (device_id)
    {
#if BSP_USE_UART0
    case 0:
        Uart->Device = UartNs550_0;
        break;
#endif
#if BSP_USE_UART1
    case 1:
        Uart->Device = UartNs550_1;
        break;
#endif
#if BSP_USE_UART2
    case 2:
        Uart->Device = UartNs550_2;
        break;
#endif
    default:
        // Trigger a fault: unsupported device ID
        break;
    };

    /* Initialize the UartNs550 driver so that it's ready to use */
    XUartNs550_Initialize(&Uart->Device, device_id);

    /* Perform a self-test to ensure that the hardware was built correctly */
    /* configASSERT(XUartNs550_SelfTest(&Uart->Device) == XST_SUCCESS); */

    (void)plic_source_id;
    uint16_t Options = XUN_OPTION_FIFOS_ENABLE | XUN_FIFO_TX_RESET | XUN_FIFO_RX_RESET;
    XUartNs550_SetOptions(&Uart->Device, Options);
}

/**
 * Receive a single byte from UART peripheral. Polling mode,
 * waits until finished.
 */
static uint8_t uart_rxchar(struct UartDriver *Uart)
{
    return XUartNs550_RecvByte(Uart->Device.BaseAddress);
}

/**
 * Transmit a single byte from UART peripheral. Polling mode,
 * waits until finished.
 */
static uint8_t uart_txchar(struct UartDriver *Uart, uint8_t c)
{
    XUartNs550_SendByte(Uart->Device.BaseAddress, c);
    return c;
}

/**
 * Transmit a buffer. Waits for @UART_TX_DELAY ms. Synchronous API.
 * Returns number of transmitted bytes or -1 in case of a timeout.
 */
static int uart_txbuffer(struct UartDriver *Uart, uint8_t *ptr, int len)
{
    static int returnval;

    for (int i = 0; i < len; i++)
    {
        XUartNs550_SendByte(Uart->Device.BaseAddress, ptr[i]);
    }
    returnval = len;

    return returnval;
}

/**
 * Receive a buffer of data. Synchronous API. Note that right now it waits
 * indefinitely until the buffer is filled.
 */
static int uart_rxbuffer(struct UartDriver *Uart, uint8_t *ptr, int len)
{
    static int returnval;

    for (int i = 0; i < len; i++)
    {
        ptr[i] = XUartNs550_RecvByte(Uart->Device.BaseAddress);
    }
    returnval = len;

    return returnval;
}
