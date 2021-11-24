#pragma once
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <lcom/lcf.h>
#include <minix/sysutil.h>

#include "sprite.h"
#include "mouse.h"
#include "images.xpm"

/** @defgroup menu menu
 * @{
 * 
 * Functions that manage the state of the game and the menus
 */

/**
 * @brief Enumerated type for the different menus in the game
 */
typedef enum{
    mainmenu, singleplayer, BestTime, inGame, paused, victoryscreen, deathscreen, end
}Menu;

/**
 * @brief Enumerated type for the different types of buttons in the game
 */
typedef enum{
    singleplayerButton, multiplayerButton, quitgameButton, level1Button, level2Button, level3Button, gotomainmenuButton, BestT1Button, BestT2Button, BestT3Button, Xbutton, quitlevelButton, resumeButton, tryagainButton, noButton
}ButtonType;

/**
 * @brief Struct type to save info about the buttons in the game
 */
typedef struct{
    ButtonType type;                                /**< @brief Button's type*/
    int x;                                          /**< @brief x coordinate of the top-left pixel*/
    int y;                                          /**< @brief y coordinate of the top-left pixel*/
    xpm_image_t normal, highlighted;                
    uint32_t * Normal_color, * Highlighted_color;
}Button;

/**
 * @brief Struct type to save info about the program
 */
typedef struct{
    Menu mode;                                                              /**< @brief Game's current menu*/
    int level;                                                              /**< @brief Game's current level*/
    int tickAlternator;                                                     /**< @brief current gameTick's increment*/
    unsigned long gameTick;                                                 /**< @brief levels' time elapse counter (ticks)*/
    unsigned long BestTimes1[3];                                            /**< @brief 3 Best completion times for level 1*/
    unsigned long BestTimes2[3];                                            /**< @brief 3 Best completion times for level 2*/                                            
    unsigned long BestTimes3[3];                                            /**< @brief 3 Best completion times for level 3*/
    unsigned long BestTDate1[3];                                            /**< @brief Dates of the 3 Best completion times for level 1*/
    unsigned long BestTDate2[3];                                            /**< @brief Dates of the 3 Best completion times for level 2*/
    unsigned long BestTDate3[3];                                            /**< @brief Dates of the 3 Best completion times for level 3*/
    xpm_image_t background_Img, title_Img, logoLevel_Img, finishline_Img, river_Img, victoryscreen_Img, deathscreen_Img, pausescreen_Img, besttimescreen_Img, numbers_Img;
    uint32_t *background_color, *title_color, *logoLevel_color,*finishline_color,*river_color, *victoryscreen_color, *deathscreen_color, *pausescreen_color, *besttimescreen_color, *numbers_color;
}Game;

/**
 * @brief Initializes the parameters of an "Button" variable
 * 
 * @param button Buttons to be initialized
 * @param type button's corresponding type
 * @param x x coordinate of the top-left corner of the button
 * @param y y coordinate of the top-left corner of the button
 * @param normal button's normal sprite
 * @param highl button's sprite when highlighted
 */
void AssignButton(Button *button, ButtonType type, int x, int y, xpm_map_t normal, xpm_map_t  highl);

/**
 * @brief Initializes all the buttons in the game, by calling the AssinButton for each one of them
 * 
 * @param object Pointer to the beginning of the array of buttons to the initialized
 */
void ButtonBuilder(Button * buttons);

/**
 * @brief Initializes the parameters of an "Game" variable
 * 
 * @param game Variable to be initialized
 */
void GameBuilder(Game *game);

/**
 * @brief Changes the displayed menu based on the pressed button
 * 
 * @param game Variable to be altered
 * @param button Button type that was pressed
 * @param objects If the button pressed is supposed to start a level, the objects need to be altered to appear on the screen accordingly
 * @param penguin If the button pressed is supposed to start a level, the Penguin needs to be altered to appear on the screen accordingly
 */
void Change_Menu(Game *game, ButtonType button, Object *objects, Penguin *penguin);

/**
 * @brief Changes the button's sprite to the highlighted one if the mouse is hovering over it, or changes the menu displayed if the left button of the mouse was pressed on top of a game's button
 * 
 * @param mouse Mouse variable that contains the information about the cursor's position
 * @param game Variable to be altered, if necessary
 * @param highlighted Variable that dictates which button is highlighted
 * @param objects If the button pressed is supposed to start a level, the objects need to be altered to appear on the screen accordingly
 * @param penguin If the button pressed is supposed to start a level, the Penguin needs to be altered to appear on the screen accordingly
 */
void CheckMouse(Mouse mouse, Button * buttons, Game * game, ButtonType *highlighted, Object *objects, Penguin * penguin);

/**
 * @brief Function that increments the chronometer in every interrupt of the timer
 * 
 * Since 1 second divided by 30hz(game's refresh rate) is a irracional number (0.0333(3)), the tick increment
 *  is done in a 3-3-4 order, because incrementing a infinite decimal number in every timer interrupt could cause
 *   to rounding erros by the program
 * 
 * @param game Variable where the game time is saved
 */
void incrementGameTick(Game *game);

/**
 * @brief Updates the arrays that contain the 3 best completion times for each level and the corresponding dates when they were made
 * 
 * @param game Variable where there times and dates are saved
 * @param day current day
 * @param month current month
 * @param year current year
 */
void updateBestTimes(Game *game,uint32_t *day,uint32_t *month,uint32_t *year);

/**
 * @brief Reads the best completion times for each level and the dates when they were made from a file and saves them to the corresponding paramenters of a Game type variable
 * 
 * @param game Variable to be updated
 */
void readFromFile(Game *game);

/**
 * @brief Saves the best completion times for each level and the dates when they were made to a file
 * 
 * @param game Variable where these times and dates are stored
 */
void saveScorestoFile(Game *game);
/**@}*/
