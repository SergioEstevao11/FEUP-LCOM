#pragma once
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <lcom/lcf.h>
#include <minix/sysutil.h>

#include "Pingu_up.xpm"
#include "Pingu_down.xpm"
#include "Pingu_right.xpm"
#include "Pingu_left.xpm"
#include "snow.xpm"
#include "snow_texture.xpm"
#include "finishline.xpm"
#include "river.xpm"
#include "trunk.xpm"
#include "snowman.xpm"
#include "rockwall.xpm"

/** @defgroup sprite sprite
 * @{
 * 
 * Functions for managing the movement of the Player and independent objects
 */
/**
 * @brief Enumerated type for specifying the Penguin's sprite and position
 */
typedef enum{
    up1, up2, up3, down1, down2, down3, right1, right2, right3, left1, left2, left3

} Position;

/**
 * @brief Enumerated type for specifying objects' type
 */
typedef enum{
    RockWall, IceBlock, Trunck, SnowMan
} Entity;
/**
 * @brief Enumerated type for specifying the objects' movement type
 */
typedef enum{
    up, down, left, right, none
} Movement;

/**
 * @brief Struct type to save info about the objects in the game
 */
typedef struct{
    int x;                  /**< @brief x coordinate of the top-left pixel*/
    int y;                  /**< @brief y coordinate of the top-left pixel*/             
    int width;              /**< @brief object's width  */
    int height;             /**< @brief object's height */
    Movement mov;           /**< @brief direction of the object's movement */
    int speed;              /**< @brief object's speed (pixels/tick) */
    Entity ObjectType;      /**< @brief object's type */
    xpm_image_t img;    
    uint32_t * Color;   
}Object;

/**
 * @brief Struct type to save info about the Penguin (Playing Character) in the game
 */
typedef struct{
    int x;                  /**< @brief x coordinate of the top-left pixel*/  
    int y;                  /**< @brief y coordinate of the top-left pixel*/             
    Position pos;           /**< @brief current penguin's position*/  
    xpm_image_t ImgUp1, ImgUp2, ImgUp3, ImgDown1, ImgDown2, ImgDown3, ImgRight1, ImgRight2, ImgRight3,  ImgLeft1, ImgLeft2, ImgLeft3, ImgDeath;
    uint32_t * ColorUp1, *ColorUp2, *ColorUp3, *ColorDown1, *ColorDown2, *ColorDown3, *ColorRight1, *ColorRight2, *ColorRight3, *ColorLeft1, *ColorLeft2, *ColorLeft3, *ColorDeath;

}Penguin;

/**
 * @brief Initializes the parameters of an "Object" element
 * 
 * @param object Object to the initialize
 * @param sprite Object's image (xpm)
 * @param x Initial x coordinate of the top-left corner of the object
 * @param y Initial y coordinate of the top-left corner of the object
 * @param speed Object's speed (pixel/tick)
 * @param mov Object's moving direction
 * @param e Object's type
 */
void buildObject(Object* object, xpm_map_t sprite, int x, int y, int speed, Movement mov, Entity e);

/**
 * @brief Initializes the objects for the specified level
 * 
 * @param object Pointer to the beginning of the array of objects to the initialized
 * @param level Level selected by the player
 */
void level_setup(Object* objects, int level);

/**
 * @brief Initializes the parameters of an "Penguin" element
 * 
 * @param Penguin 
 */
void buildPenguin(Penguin *Penguin);

/**
 * @brief Reset the Penguin's position to the starting one
 * 
 * @param Penguin Variable to be rested
 */
void reset_penguin(Penguin *Penguin);

/**
 * @brief Updates the Penguin's position provoked by human input or if the Penguin is on top of a "Trunk" type object
 * 
 * @param Penguin Variable to be updated
 * @param pos User's movement input
 * @param mode VBE mode
 * @param objects Objects' array
 * @param level Current level
 */
void updatePenguin(Penguin *Penguin, Movement pos,uint16_t mode, Object *objects,int level);

/**
 * @brief Updates the Penguin's position if he's on top of a trunk and checks if he is out of bounds
 * 
 * @param Penguin Variable to be updated
 * @param mode VBE mode
 * @param objects Objects' array
 * @param level Current level
 * @return False if the Penguin is out of bounds, true otherwise
 */
bool CheckTrunks(Penguin *Penguin, uint16_t mode, Object *objects, int level);

/**
 * @brief Updates the Objects' position
 * 
 * @param objects Array of objects to be updated
 * @param level Current level
 */
void update_objects(Object * objects, int level);

/**
 * @brief Checks if the Penguin is colliding with other objects at the current time
 * 
 * @param Penguin Penguin variable
 * @param objects Array of objects
 * @param level Current level
 * @return False if Penguin is colliding with an object, true otherwise
 */
bool checkCollisions(Penguin Penguin, Object *objects,int level);

/**
 * @brief Checks if the Penguin is at the finnish line
 * 
 * @param Penguin Penguin variable
 * @param objects Array of objects
 * @param level Current level
 * @return True if the Penguin is at the finnish line, false otherwise
 */
bool checkWin(Penguin Penguin);

/**
 * @brief Checks if the Penguin is inside a water tile
 * 
 * @param Penguin Penguin variable
 * @param objects Array of objects (used to invoke the CheckTrunk function)
 * @param level Current level
 * @return False if the Penguin is inside a water tile, false otherwise
 */
bool checkInWater(Penguin *Penguin,Object *objects,int level);
/**@}*/
