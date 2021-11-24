#pragma once
#include <lcom/lcf.h>
#include <minix/sysutil.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include "menu.h"

#define TrasparencyColor 0xFF000000

/**
 * @brief Initializes the video module in graphics mode.
 * 
 * @param mode VBE mode to set
 * @return Return 0 upon success and 1 otherwise
 */
int vh_set_mode(uint16_t mode);

/**
 * @brief Changes the color of a pixel
 * 
 * @param x x coordinate of the pixel
 * @param y y coordinate of the pixel
 * @param color color to set the pixel
 */
void change_color(uint16_t x, uint16_t y, uint32_t color);

/**
 * @brief Draws a pixmap image on the screen
 * 
 * @param x x coordinate of the initial point
 * @param y y coordinate of the initial point
 * @param width width of the image
 * @param height height of the image
 * @param bytes array with the colors of each pixel of the image
 */
void vg_draw_pixmap(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t * bytes);

/**
 * @brief Clears the screen by drawing the current menu's corresponding background
 * 
 * @param game Variable that contains the info to the current menu
 */
void clear_screen(Game game);

/**
 * @brief Draws the Penguin on the screen
 * 
 * @param Penguin Variable that contains the info to the Penguin's current position and direction that he's facing
 */
void vg_draw_Penguin(Penguin Penguin);

/**
 * @brief Draws the corresponding buttons of each menu
 * 
 * @param begin Position in the array of the first button to be drawn
 * @param end Position in the array where the function stops
 * @param buttons Array that contains all buttons
 * @param highlighted Variable that contains the info of which button is highlighted
 */
void vg_draw_Buttons(int begin, int end, Button *buttons, ButtonType highlighted);

/**
 * @brief Function that is able to draw on screen the characters ':', '/' and all decimal numbers (0-9)
 * 
 * @param game Where the characters sprites are saved
 * @param a character to be drawn
 * @param x x coordinate of the initial point
 * @param y y coordinate of the initial point
 */
void vg_draw_char(Game game, char a, int x, int y);

/**
 * @brief Function that draws on the screen any number up to 100
 * 
 * @param game Where the characters sprites are saved
 * @param num number to be drawn
 * @param x x coordinate of the initial point, that is update as the characters of the number are drawn
 * @param y y coordinate of the initial point
 */
void vg_draw_int(Game game, int num, int*x, int y);

/**
 * @brief Function that draws on the screen the current time in a "dd/mm/yy" format
 * 
 * @param game Where the characters sprites are saved
 * @param date date in a ddmmyy format
 * @param x x coordinate of the initial point, that is update as the characters of the number are drawn
 * @param y y coordinate of the initial point
 */
void vg_draw_date(Game game, unsigned long date, int *x, int y);

/**
 * @brief Function that draws on the screen the current time in a "minmin:ss:msms" format
 * 
 * @param game Where the characters sprites are saved
 * @param time time in ticks
 * @param x x coordinate of the initial point, that is update as the characters of the number are drawn
 * @param y y coordinate of the initial point
 */
void vg_draw_time(Game game, unsigned long time, int *x, int y);

/**
 * @brief Function that draws on the screen best completion times of a specific level and when they were made
 * 
 * @param game Where the characters sprites are saved
 */
void vg_draw_besttimes(Game game);

/**
 * @brief Function that moves the content of the invisible buffer variable video_buffer to the variable video_mem, so that the screen updates without stuttering
 */
void double_buffer();

/**
 * @brief Function that frees the memory allocated for the variable "video_buffer"
 */
void free_buffer();

/**
 * @brief Function that draws the objects for a level
 * 
 * @param objects array of objects
 * @param size number of objects to be drawn
 */
void vg_draw_object(Object * objects,int size);

/**
 * @brief Function that updates the screen when the user is in a level
 * 
 * @param Penguin player character
 * @param objects array of objects
 * @param game stores information about the state of the program 
 */
void update_screen(Penguin Penguin,Object * objects,Game *game);

/**
 * @brief Function that updates the screen when the user is in a menu
 * 
 * @param buttons array of buttons
 * @param game information about the state of the program
 * @param highlighted button who is highlighted
 * @param mouse location of the program's cursor
 * @param Penguin Player character
 * @param objects array of objects
 *  
 */
void update_menu(Button * buttons, Game *game, ButtonType highlighted, Mouse mouse, Penguin Penguin,Object * objects);
/**@}*/
