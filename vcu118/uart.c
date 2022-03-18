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

/*****************************************************************************/
/* Peripheral specific definitions */
#if BSP_USE_UART0
/**
 * Initialize UART 0 peripheral
 */
void uart0_init(void)
{
    uart_init(&Uart0, XPAR_UARTNS550_0_DEVICE_ID, 0);
}

/**
 * Return 1 if UART0 has at leas 1 byte in the RX FIFO
 */
bool uart0_rxready(void)
{
    return uart_rxready(&Uart0);
}

/**
 * Receive a single byte. Polling mode, waits until finished
 */
char uart0_rxchar(void)
{
    return (char)uart_rxchar(&Uart0);
}

/**
 * Transmit a buffer. Waits indefinitely for a UART TX mutex,
 * returns number of transferred bytes or -1 in case of an error.
 * Synchronous API.
 */
int uart0_txbuffer(char *ptr, int len)
{
    return uart_txbuffer(&Uart0, (uint8_t *)ptr, len);
}

/**
 * Transmit a single byte. Polling mode, waits until finished
 */
char uart0_txchar(char c)
{
    return (char)uart_txchar(&Uart0, (uint8_t)c);
}

int uart0_rxbuffer(char *ptr, int len)
{
    return uart_rxbuffer(&Uart0, (uint8_t *)ptr, len);
}
#endif /* BSP_USE_UART0 */

#if BSP_USE_UART1
/**
 * Initialize UART 1 peripheral
 */
void uart1_init(void)
{
    uart_init(&Uart1, XPAR_UARTNS550_1_DEVICE_ID, 0);
}

/**
 * Return 1 if UART1 has at leas 1 byte in the RX FIFO
 */
bool uart1_rxready(void)
{
    return uart_rxready(&Uart1);
}

/**
 * Receive a single byte.
 */
char uart1_rxchar(void)
{
    return (char)uart_rxchar(&Uart1);
}

/**
 * Transmit a buffer. Waits indefinitely for a UART TX mutex,
 * returns number of transferred bytes or -1 in case of an error.
 * Synchronous API.
 */
int uart1_txbuffer(char *ptr, int len)
{
    return uart_txbuffer(&Uart1, (uint8_t *)ptr, len);
}

/**
 * Transmit a single byte.
 */
char uart1_txchar(char c)
{
    return (char)uart_txchar(&Uart1, (uint8_t)c);
}

/**
 * Transmit buffer.
 */
int uart1_rxbuffer(char *ptr, int len)
{
    return uart_rxbuffer(&Uart1, (uint8_t *)ptr, len);
}
#endif /* BSP_USE_UART1 */

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
    XUartNs550_Config uart_config = { device_id, 0, 0, 0 };
    switch (device_id)
    {
#if BSP_USE_UART0
    case 0:
        Uart->Device = UartNs550_0;
        uart_config.BaseAddress = XPAR_UARTNS550_0_BASEADDR;
        uart_config.InputClockHz = XPAR_UARTNS550_0_CLOCK_HZ;
        uart_config.DefaultBaudRate = XPAR_UARTNS550_0_BAUD_RATE;
        break;
#endif
#if BSP_USE_UART1
    case 1:
        Uart->Device = UartNs550_1;
        uart_config.BaseAddress = XPAR_UARTNS550_1_BASEADDR;
        uart_config.InputClockHz = XPAR_UARTNS550_1_CLOCK_HZ;
        uart_config.DefaultBaudRate = XPAR_UARTNS550_1_BAUD_RATE;
        break;
#endif
    default:
        // Trigger a fault: unsupported device ID
        break;
    };

    /* Initialize the UartNs550 driver so that it's ready to use */
    XUartNs550_CfgInitialize(&Uart->Device, &uart_config, uart_config.BaseAddress);

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
