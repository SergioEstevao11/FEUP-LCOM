#include <lcom/lcf.h>
#include <lcom/lab2.h>
#include <minix/sysutil.h>

#include <stdint.h>
#include <stdbool.h>

#include "i8042.h"


/** @defgroup keyboard keyboard
 * @{
 * 
 * Functions that handle the keyboard interrupts
 */


/**
 * @brief Handles the keyboard interrupts, using the scan_code and scanaux variables to pass the scan/break codes read
 */
void (kbc_ih)();

/**
 * @brief Subscribes and enables keyboard interrupts
 * @param bit_no address of memory to be initialized with the
 *         bit number to be set in the mask returned upon an interrupt
 * @return Return 0 upon success and non-zero otherwise
 */
int kbd_subscribe_int(uint32_t *bit_no);

/**
 * @brief Unsubscribes mouse interrupts
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int kbd_unsubscribe_int();

/**
 * @brief Invokes sys_inb() system call but reads the value into a uint8_t variable.
 * 
 * @param port 	the input port that is to be read
 * @param value variable to be updated with the value read
 * @return Return 0 upon success and non-zero otherwise
 */
int util_sys_inb(int port, uint8_t *value);

/**
 * @brief Writes a command to the KBC command buffer
 * 
 * @param c command to be written
 * @return Return 0 upon success and non-zero otherwise
 */
int KBC_send_cmd(uint8_t c);
/**@}*/
