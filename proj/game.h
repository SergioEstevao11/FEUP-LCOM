#pragma once
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#include "keyboard.h"
#include "timer.h"
#include "graphics.h"
#include "rtc.h"

/** @defgroup game game
 * @{
 * 
 * Main function of the game where the driver receive loop is implemented
 */
/**
 * @brief Fucntion that handles the project's main loop
 * 
 * @return returns 0 if no errors occurred, 1 otherwise
 */
int game();
/**@}*/
