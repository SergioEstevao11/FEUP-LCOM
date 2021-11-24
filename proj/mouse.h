#pragma once
#include <lcom/lcf.h>
#include <minix/sysutil.h>

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#include "i8042.h"
#include "math.h"
#include "ponteiro.xpm"

/** @defgroup mouse mouse
 * @{
 * 
 * Functions that manage the cursor of the game
 */

/**
 * @brief Struct type to save info about the mouse cursor in the game
 */
typedef struct{
    int x;                  /**< @brief x coordinate of the top-center pixel*/
    int y;                  /**< @brief y coordinate of the top-center pixel*/
    bool lbpressed;         /**< @brief true if the left mouse button has been pressed, false otherwise*/
    xpm_image_t img;        /**< @brief cursor's image info*/
    uint32_t *color;        /**< @brief cursor's image color*/
}Mouse;


/**
 * @brief Initializes the parameters of a "Mouse" element
 * 
 * @param mouse Variable to be initialized
 */
void mouse_builder(Mouse *mouse);

/**
 * @brief Updates the parametres of a "Mouse" variable, namely the x and y position and the lbpressed variable
 *
 * @param pp Packet with the information about the most recent mouse movement
 * @param mouse Variable to be updated
 */
void update_mouse(struct packet *pp, Mouse * mouse);

/**
 * @brief Subscribes and enables mouse interrupts
 * @param bit_no address of memory to be initialized with the
 *         bit number to be set in the mask returned upon an interrupt
 * @return Return 0 upon success and non-zero otherwise
 */
int (mouse_subscribe_int)(uint32_t *bit_no);

/**
 * @brief Unsubscribes mouse interrupts
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int (mouse_unsubscribe_int)();

/**
 * @brief Handles the mouse interrupt
 */
void (mouse_ih)();

/**
 * @brief Adds to the "PacketMouse" array the information receive in the mouse interrupts
 */
void packetbuilder();

/**
 * @brief Updates the "stuct packet" variable with the info in "PacketMouse" array
 * @param pp Variable to be updated
 */
void arrangeStruct(struct packet * pp);

/**
 * @brief Disables the data reporting mode of the mouse
 * @return 0 upon success, 1 otherwise
 */
int mouse_disable_data_reporting();

/**
 * @brief Enables the data reporting mode of the mouse
 * @return 0 upon success, 1 otherwise
 */
int mouse_eenable_data_reporting();

/**
 * @brief Passes in the KBC command buffer the 0xD4 command, followed by the command passed in the argument "cmd"
 * 
 * @param cmd Mouse command to be written in the KBC command buffer
 * @return 0 upon success, 1 otherwise
 */
int mouse_cmd(uint8_t cmd);
/**@}*/
