#include "sprite.h"

void buildObject(Object* object, xpm_map_t sprite, int x, int y, int speed, Movement mov, Entity e){
    object->Color = (uint32_t *)xpm_load(sprite, XPM_8_8_8_8, &(object->img));
    object->x = x;
    object->y = y;
    object->speed = speed;
    object->mov = mov;
    object->width = object->img.width;
    object->height = object->img.height;
    object->ObjectType = e;
}

void level_setup(Object* objects, int level){
    switch(level){
        case 1:{
            buildObject(&objects[0], snow_wall, 200, 290, 4, right, IceBlock);
            buildObject(&objects[1], snow_wall, 400, 415, 4, left, IceBlock);
            buildObject(&objects[2], snowman, 40, 60, 0, none, SnowMan);
            buildObject(&objects[3], rockwall, 400, 180, 0, none, RockWall);
            buildObject(&objects[4], trunk, 0, 128, 2, right, Trunck);
            buildObject(&objects[5], rockwall, 0, 180, 0, none, RockWall);
            break;
        }

        case 2:{           
            buildObject(&objects[0], snow_wall, 200, 50, 5, right, IceBlock);
            buildObject(&objects[1], snow_wall, 400, 290, 5, left, IceBlock);
            buildObject(&objects[2], trunk, 0, 430, 3, right, Trunck);
            buildObject(&objects[3], trunk, 0, 188, 3, right, Trunck);
            buildObject(&objects[4], rockwall, 0, 485, 0, none, RockWall);
            buildObject(&objects[5], rockwall, 0, 240, 0, none, RockWall);
            buildObject(&objects[6], snowman, 40, 365, 0, none, SnowMan);
            buildObject(&objects[7], snowman, 40, 120, 0, none, SnowMan);
            break;
        }
        case 3:{
            buildObject(&objects[0], snow_wall, 260, 50, 15, right, IceBlock);
            buildObject(&objects[1], snow_wall, 400, 290, 15, left, IceBlock);
            buildObject(&objects[2], trunk, 0, 430, 4, right, Trunck);
            buildObject(&objects[3], trunk, 0, 188, 10, right, Trunck);
            buildObject(&objects[4], trunk, 450, 373, 10, right, Trunck);
            buildObject(&objects[5],rockwall, 0, 485, 0, none, RockWall);
            buildObject(&objects[6], rockwall, 0, 240, 0, none, RockWall);
            break;
        }
        default:{
            break;
        }
    }
    
}

void update_objects(Object * objects, int level){
    int end,c;
    switch(level){
        case 1:
            end = 6;  
            break;    
        case 2:
            end = 8;
            break;
        case 3:
            end = 7;
            break;
    }
    for(c = 0; c<end;c++){
                switch(objects[c].mov){
                    case up:{
                        objects[c].y = (objects[c].y - objects[c].speed) % 600;
                        if (objects[c].y < 0)
                            objects[c].y = objects[c].y + 600;
                        break;
                    }

                    case down:{
                        objects[c].y = (objects[c].y + objects[c].speed) % 600;
                        if (objects[c].y < 0)
                            objects[c].y = objects[c].y + 600;
                        break;
                    }

                    case left:{
                        objects[c].x = (objects[c].x - objects[c].speed) % 800;
                        if (objects[c].x < 0)
                            objects[c].x = objects[c].x + 800;
                        break;
                    }

                    case right:{
                        objects[c].x = (objects[c].x + objects[c].speed) % 800; 
                        break;
                    }

                    default:{
                        break;
                    }
                }
            }
}

bool checkCollisions(Penguin Penguin, Object *objects,int level){
    int o1,o2,o3,o4,p1,p2,p3,p4;
    int end;
    switch(level){
        case 1:
            end = 6;  
            break;    
        case 2:
            end = 8;
            break;
        case 3:
            end = 7;
            break;
    }
    for(int c=0;c<end;c++){
        o1 = objects[c].x;
        o2 = objects[c].x + objects[c].width;
        o3 = objects[c].y;
        o4 = objects[c].y + objects[c].height;
        p1 = Penguin.x;
        p2 = Penguin.x + Penguin.ImgUp1.width;
        p3 = Penguin.y;
        p4 = Penguin.y + Penguin.ImgUp1.height;
        if((p2 > o1 && p4 > o3 && o4 > p3 && o2 > p1) ){
            if(objects[c].ObjectType == Trunck){
                return true;
            }
            return false;
         }
    }
    return true;
}

void buildPenguin(Penguin *Penguin){
    Penguin->pos = down1;

    Penguin->x = 400;
    Penguin->y = 540;

    Penguin->ColorUp1 = (uint32_t *)xpm_load(Pingu_up1, XPM_8_8_8_8, &Penguin->ImgUp1);
    Penguin->ColorUp2 = (uint32_t *)xpm_load(Pingu_up2, XPM_8_8_8_8, &Penguin->ImgUp2);
    Penguin->ColorUp3 = (uint32_t *)xpm_load(Pingu_up3, XPM_8_8_8_8, &Penguin->ImgUp3);
    Penguin->ColorDown1 = (uint32_t *)xpm_load(Pingu_down1, XPM_8_8_8_8, &Penguin->ImgDown1);
    Penguin->ColorDown2 = (uint32_t *)xpm_load(Pingu_down2, XPM_8_8_8_8, &Penguin->ImgDown2);
    Penguin->ColorDown3 = (uint32_t *)xpm_load(Pingu_down3, XPM_8_8_8_8, &Penguin->ImgDown3);
    Penguin->ColorRight1 = (uint32_t *)xpm_load(Pingu_right1, XPM_8_8_8_8, &Penguin->ImgRight1);
    Penguin->ColorRight2 = (uint32_t *)xpm_load(Pingu_right2, XPM_8_8_8_8, &Penguin->ImgRight2);
    Penguin->ColorRight3 = (uint32_t *)xpm_load(Pingu_right3, XPM_8_8_8_8, &Penguin->ImgRight3);
    Penguin->ColorLeft1 = (uint32_t *)xpm_load(Pingu_left1, XPM_8_8_8_8, &Penguin->ImgLeft1);
    Penguin->ColorLeft2 = (uint32_t *)xpm_load(Pingu_left2, XPM_8_8_8_8, &Penguin->ImgLeft2);
    Penguin->ColorLeft3 = (uint32_t *)xpm_load(Pingu_left3, XPM_8_8_8_8, &Penguin->ImgLeft3);
    Penguin->ColorDeath = (uint32_t *)xpm_load(skull, XPM_8_8_8_8, &Penguin->ImgDeath);

}

bool checkWin(Penguin Penguin){
    if(Penguin.y + Penguin.ImgUp1.height <= 60 )
        return true;
    return false;
}

bool CheckTrunks(Penguin *Penguin, uint16_t mode, Object *objects, int level){
    vbe_mode_info_t info;
    vbe_get_mode_info(mode,&info);
    switch(level){
        case 1:
            if((Penguin->x >= objects[4].x && Penguin->x <= (objects[4].x+objects[4].width)) && ((Penguin->y + Penguin->ImgUp1.height) <= (objects[4].y+objects[4].height) && (Penguin->y >= 120 ))){
                if (Penguin->x+Penguin->ImgRight1.width >= info.XResolution)
                    return false;
                Penguin->x = (Penguin->x + objects[4].speed);
                return true;
            }
            break;
        case 2:
            if((Penguin->x >= objects[2].x && Penguin->x <= (objects[2].x+objects[2].width)) && ((Penguin->y + Penguin->ImgUp1.height) <= (objects[2].y+objects[2].height) && (Penguin->y+Penguin->ImgUp1.height)>= 425)){
                    if (Penguin->x+Penguin->ImgRight1.width >= info.XResolution)
                        return false;
                    Penguin->x = (Penguin->x + objects[2].speed);
                    return true;
                }
            if((Penguin->x >= objects[3].x && Penguin->x <= (objects[3].x+objects[3].width)) && ((Penguin->y + Penguin->ImgUp1.height) <= (objects[3].y+objects[3].height) && (Penguin->y+Penguin->ImgUp1.height)>= 180)){
                    if (Penguin->x+Penguin->ImgRight1.width >= info.XResolution)
                        return false;
                    Penguin->x = (Penguin->x + objects[3].speed);
                    return true;
                }
            break;
        case 3:
            if((Penguin->x >= objects[2].x && Penguin->x <= (objects[2].x+objects[2].width)) && ((Penguin->y + Penguin->ImgUp1.height) <= (objects[2].y+objects[2].height) && (Penguin->y+Penguin->ImgUp1.height)>= 425)){
                    if (Penguin->x+Penguin->ImgRight1.width >= info.XResolution)
                        return false;
                    Penguin->x = (Penguin->x + objects[2].speed);
                    return true;
                }
            if((Penguin->x >= objects[3].x && Penguin->x <= (objects[3].x+objects[3].width)) && ((Penguin->y + Penguin->ImgUp1.height) <= (objects[3].y+objects[3].height) && (Penguin->y+Penguin->ImgUp1.height)>= 180)){
                    if (Penguin->x+Penguin->ImgRight1.width >= info.XResolution)
                        return false;
                    Penguin->x = (Penguin->x + objects[3].speed);
                    return true;
                }
            if((Penguin->x >= objects[4].x && Penguin->x <= (objects[4].x+objects[4].width)) && ((Penguin->y + Penguin->ImgUp1.height) <= (objects[4].y+objects[4].height) && (Penguin->y+Penguin->ImgUp1.height) >= 365)){
                    if (Penguin->x+Penguin->ImgRight1.width >= info.XResolution)
                        return false;
                    Penguin->x = (Penguin->x + objects[4].speed);
                    return true;
                }
            // if((Penguin->x >= objects[5].x && Penguin->x <= (objects[5].x+objects[5].width)) && ((Penguin->y + Penguin->ImgUp1.height) <= (objects[5].y+objects[5].height) && (Penguin->y+Penguin->ImgUp1.height)>= 180)){
            //         if (Penguin->x+Penguin->ImgRight1.width >= info.XResolution)
            //             return false;
            //         Penguin->x = (Penguin->x + objects[5].speed);
            //         return true;
            //     }
            break;
    }
    return false;
}

bool checkInWater(Penguin *Penguin,Object *objects,int level){
    if(CheckTrunks(Penguin,0x115,objects,level))
        return false;
    switch(level){
        case 1:
            if(Penguin->y >= 120 && ((Penguin->y + Penguin->ImgUp1.height) <= 180))
                return true;
            return false;
            break;
        case 2:
            if((Penguin->y >= 420 && ((Penguin->y + Penguin->ImgUp1.height) <= 485)))
                    return true;
            if( (Penguin->y >= 180 && ((Penguin->y + Penguin->ImgUp1.height) <= 240)))
                    return true;
            return false;
    
            break;
        case 3:{
            if((Penguin->y >= 355 && ((Penguin->y + Penguin->ImgUp1.height) <= 485)))
                    return true;
            if( (Penguin->y >= 180 && ((Penguin->y + Penguin->ImgUp1.height) <= 240)))
                    return true;
            return false;
            break;
        }
    }
    return true;
}

void reset_penguin(Penguin *Penguin){
    Penguin->pos = down1;
    Penguin->x = 400;
    Penguin->y = 540;
}

void updatePenguin(Penguin *Penguin, Movement pos,uint16_t mode, Object *objects,int level){
    vbe_mode_info_t info;
    vbe_get_mode_info(mode,&info);
    switch(pos){
        case up:{
            if (Penguin->y - Penguin->ImgUp1.height < 0)
                return;
            
            Penguin->y-= 60;

            if(Penguin->pos == up1){
                Penguin->pos = up2;
            }
            else if(Penguin->pos == up2)
                Penguin->pos = up3;
            else{
                Penguin->pos = up1;
            }
            break;

        }
        case down:{
            if (Penguin->y + 60 >= info.YResolution)
                return;
            Penguin->y+=60;
            if(Penguin->pos == down1){
                Penguin->pos = down2;
            }
            else if(Penguin->pos == down2)
                Penguin->pos = down3;
            else{
                Penguin->pos = down1;
            }
            break;

        }
        case right:{
            if (Penguin->x+Penguin->ImgRight1.width + 40 >= info.XResolution)
                return;
            Penguin->x+=40;
            if(Penguin->pos == right1){
                Penguin->pos = right2;
            }
            else if(Penguin->pos == right2)
                Penguin->pos = right3;
            else{
                Penguin->pos = right1;
            }
            break;

        }
        case left:{
            if (Penguin->x-40 < 0)
                return;
            Penguin->x-=40;
            if(Penguin->pos == left1){
                Penguin->pos = left2;
            }
            else if(Penguin->pos == left2)
                Penguin->pos = left3;
            else{
                Penguin->pos = left1;
            }
            break;
        }
        default:
            break;
    }
}
