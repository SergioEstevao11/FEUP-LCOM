#include "menu.h"

void AssignButton(Button *button, ButtonType type, int x, int y, xpm_map_t normal, xpm_map_t  highl){
    button->type = type;
    button->x = x;
    button->y = y;
    button->Normal_color = (uint32_t *)xpm_load(normal, XPM_8_8_8_8, &button->normal);
    button->Highlighted_color = (uint32_t *)xpm_load(highl, XPM_8_8_8_8, &button->highlighted);
}


void ButtonBuilder(Button * buttons){
    AssignButton(&buttons[0], singleplayerButton, 300, 350, SinglePlayer, SinglePlayerH);
    AssignButton(&buttons[1], quitgameButton, 350, 450, Quit, QuitH);
    AssignButton(&buttons[2], level1Button, 160, 260, Level1, Level1H);
    AssignButton(&buttons[3], level2Button, 360, 260, Level2, Level2H);
    AssignButton(&buttons[4], level3Button, 560, 260, Level3, Level3H);
    AssignButton(&buttons[5], BestT1Button, 145, 350, Besttimes, BesttimesH);
    AssignButton(&buttons[6], BestT2Button, 345, 350, Besttimes, BesttimesH);
    AssignButton(&buttons[7], BestT3Button, 545, 350, Besttimes, BesttimesH);
    AssignButton(&buttons[8], gotomainmenuButton, 550, 508, GoToMainMenu, GoToMainMenuH);
    AssignButton(&buttons[9], Xbutton, 542, 165, xbutton, xbuttonH);
    AssignButton(&buttons[10], resumeButton, 325, 275, Resume, ResumeH);
    AssignButton(&buttons[11], quitlevelButton, 350, 325, QuitPause, QuitH);
    AssignButton(&buttons[12], tryagainButton, 325, 275, tryagain, tryagainH);
}

void readFromFile(Game *game){
    FILE * bestTimes;
    char * buffer = NULL;
    size_t characters = 0;
    int counter = 0;
    bestTimes = fopen("/home/lcom/labs/proj/bestTimes.txt","r+");
    while(getline(&buffer,&characters,bestTimes) != -1){
        if(counter >= 6){
            game->BestTimes3[counter-6] = atoi(buffer);
            getline(&buffer,&characters,bestTimes);
            game->BestTDate3[counter-6] = atoi(buffer);
        }
        else if(counter >= 3){
            game->BestTimes2[counter-3] = atoi(buffer);
            getline(&buffer,&characters,bestTimes);
            game->BestTDate2[counter-3] = atoi(buffer);
        }
        else {
            game->BestTimes1[counter] = atoi(buffer);
            getline(&buffer,&characters,bestTimes);
            game->BestTDate1[counter] = atoi(buffer);
        }
        counter++;
    }
    fclose(bestTimes);
}


void GameBuilder(Game *game){
    game->mode = mainmenu;
    game->level = 0;
    game->tickAlternator = 0;
    game->gameTick = 0;

    readFromFile(game);

    game->background_color = (uint32_t *)xpm_load(snow_texture, XPM_8_8_8_8, &game->background_Img);
    game->title_color = (uint32_t *)xpm_load(Title, XPM_8_8_8_8, &game->title_Img);
    game->logoLevel_color = (uint32_t *)xpm_load(Level, XPM_8_8_8_8, &game->logoLevel_Img);
    game->river_color = (uint32_t *)xpm_load(river, XPM_8_8_8_8, &game->river_Img);
    game->finishline_color = (uint32_t *)xpm_load(finishline, XPM_8_8_8_8, &game->finishline_Img);
    game->victoryscreen_color = (uint32_t *)xpm_load(Victoryscreen, XPM_8_8_8_8, &game->victoryscreen_Img);
    game->deathscreen_color = (uint32_t *)xpm_load(Deathscreen, XPM_8_8_8_8, &game->deathscreen_Img);
    game->pausescreen_color = (uint32_t *)xpm_load(pausedscreen, XPM_8_8_8_8, &game->pausescreen_Img);
    game->besttimescreen_color = (uint32_t *)xpm_load(Besttimesscreen, XPM_8_8_8_8, &game->besttimescreen_Img);
    game->numbers_color = (uint32_t *)xpm_load(numbers, XPM_8_8_8_8, &game->numbers_Img);
}


void Change_Menu(Game *game, ButtonType button, Object *objects, Penguin * penguin){
    switch(button){
        case singleplayerButton:{
            game->mode = singleplayer;
            break;
        }
        case quitgameButton:{
            game->mode = end;
            break;
        }
        case level1Button:{
            game->mode = inGame;
            game->level = 1;
            game->gameTick = 0;
            game->tickAlternator = 0;
            reset_penguin(penguin);
            level_setup(objects, 1);
            break;
        }
        case level2Button:{
            game->mode = inGame;
            game->level = 2;
            game->gameTick = 0;
            game->tickAlternator = 0;
            reset_penguin(penguin);
            level_setup(objects, 2);
            break;
        }
        case level3Button:{
            game->mode = inGame;
            game->level = 3;
            game->gameTick = 0;
            game->tickAlternator = 0;
            reset_penguin(penguin);
            level_setup(objects, 3);
            break;
        }
        case gotomainmenuButton:{
            game->mode = mainmenu;
            break;
        }
        case resumeButton:{
            game->mode = inGame;
            break;
        }
        case quitlevelButton:{
            game->mode = singleplayer;
            break;
        }
        case tryagainButton:{
            game->mode = inGame;
            game->gameTick = 0;
            game->tickAlternator = 0;
            reset_penguin(penguin);
            level_setup(objects, game->level);
            break;
        }
        case BestT1Button:{
            game->mode = BestTime;
            game->level = 1;
            break;
        }
        case BestT2Button:{
            game->mode = BestTime;
            game->level = 2;
            break;
        }
        case BestT3Button:{
            game->mode = BestTime;
            game->level = 3;
            break;
        }
        case Xbutton:{
            game->mode = singleplayer;
        }
        default:
            break;
    }
}

void CheckMouse(Mouse mouse, Button * buttons, Game * game, ButtonType *highlighted, Object *objects, Penguin * penguin){
    int lowerLim=0, upperLim=0;
    if (game->mode == mainmenu){
        lowerLim = 0;
        upperLim = 2;
           
    }
    else if(game->mode == singleplayer){
        lowerLim = 2;
        upperLim = 9;
    }
    else if(game->mode == victoryscreen){
        lowerLim = 11;
        upperLim = 12;
    }
    else if(game->mode == deathscreen){
        lowerLim = 11;
        upperLim = 13;
    }
    else if(game->mode == BestTime){
        lowerLim = 9;
        upperLim = 10;
    }
    else if(game->mode == paused){
        lowerLim = 10;
        upperLim = 12;
    }

    for(int c = lowerLim; c < upperLim; c++){
        if (mouse.x >= buttons[c].x && mouse.x < buttons[c].x+buttons[c].highlighted.width && mouse.y >= buttons[c].y && mouse.y < buttons[c].y+buttons[c].highlighted.height){
            if (mouse.lbpressed){
                Change_Menu(game, buttons[c].type, objects, penguin);
                break;
            }
            else{
                *highlighted = buttons[c].type;
                return;
            }
        }
    }
    *highlighted = noButton;
}

void incrementGameTick(Game *game){
    game->tickAlternator++;
    if(game->tickAlternator != 3)
        game->gameTick+=3;
    else
    {
        game->gameTick+=4;
        game->tickAlternator = 0;
    }
    
}

void updateBestTimes(Game *game,uint32_t *day,uint32_t *month,uint32_t *year){
    int c;
    unsigned long time = game->gameTick;
    switch(game->level){
        case 1:
            for(c = 0;c<3;c++){
                if(time < game->BestTimes1[c] || game->BestTimes1[c] == 0){
                    if(c == 0){
                        
                        game->BestTimes1[2] = game->BestTimes1[1];
                        game->BestTDate1[2] = game->BestTDate1[1];

                        game->BestTimes1[1] = game->BestTimes1[0];
                        game->BestTDate1[1] = game->BestTDate1[0];

                        game->BestTimes1[0] = time;
                        game->BestTDate1[0] =(*day)*pow(10, 4) + (*month)*pow(10, 2)+ (*year);
                        break;
                        
                    }
                    if(c == 1){
                        
                        game->BestTimes1[2] = game->BestTimes1[1];
                        game->BestTDate1[2] = game->BestTDate1[1];

                        game->BestTimes1[1] = time;
                        game->BestTDate1[1] = (*day)*pow(10, 4) + (*month)*pow(10, 2)+ (*year);
                        break;
                    }
                    if(c == 2){
                        
                        game->BestTimes1[2] = time;
                        game->BestTDate1[2] = (*day)*pow(10, 4) + (*month)*pow(10, 2)+ (*year);
                        break;
                    }
                }      
            } 
            break;
        case 2:
            for(c = 0;c<3;c++){
                if(time < game->BestTimes2[c] || game->BestTimes2[c] == 0){
                    if(c == 0){
                        
                        game->BestTimes2[2] = game->BestTimes2[1];
                        game->BestTDate2[2] = game->BestTDate2[1];

                        game->BestTimes2[1] = game->BestTimes2[0];
                        game->BestTDate2[1] = game->BestTDate2[0];

                        game->BestTimes2[0] = time;
                        game->BestTDate2[0] = (*day)*pow(10, 4) + (*month)*pow(10, 2)+ (*year);
                        break;
                    }
                    if(c == 1){
                        
                        game->BestTimes2[2] = game->BestTimes2[1];
                        game->BestTDate2[2] = game->BestTDate2[1];

                        game->BestTimes2[1] = time;
                        game->BestTDate2[1] = (*day)*pow(10, 4) + (*month)*pow(10, 2)+ (*year);
                        break;
                    }
                    if(c == 2){
                        
                        game->BestTimes2[2] = time;
                        game->BestTDate2[2] = (*day)*pow(10, 4) + (*month)*pow(10, 2)+ (*year);
                        break;
                    }
                }      
            } 
            break;
        case 3:
            for(c = 0;c<3;c++){
                if(time < game->BestTimes3[c] || game->BestTimes3[c] == 0){
                    if(c == 0){
                        
                        game->BestTimes3[2] = game->BestTimes3[1];
                        game->BestTDate3[2] = game->BestTDate3[1];

                        game->BestTimes3[1] = game->BestTimes3[0];
                        game->BestTDate3[1] = game->BestTDate3[0];

                        game->BestTimes3[0] = time;
                        game->BestTDate3[0] = (*day)*pow(10, 4) + (*month)*pow(10, 2)+ (*year);
                        break;
                    }
                    if(c == 1){
                        
                        game->BestTimes3[2] = game->BestTimes3[1];
                        game->BestTDate3[2] = game->BestTDate3[1];

                        game->BestTimes3[1] = time;
                        game->BestTDate3[1] = (*day)*pow(10, 4) + (*month)*pow(10, 2)+ (*year);
                        break;
                    }
                    if(c == 2){
                        
                        game->BestTimes3[2] = time;
                        game->BestTDate3[2] = (*day)*pow(10, 4) + (*month)*pow(10, 2)+ (*year);
                        break;
                    }
                }      
            } 
            break;
    }
}

void saveScorestoFile(Game *game){
    FILE * bestTimes;
    bestTimes = fopen("/home/lcom/labs/proj/bestTimes.txt","w");
    for(int c = 0;c<3; c++){
        fprintf(bestTimes,"%lu\n", game->BestTimes1[c]);
        fprintf(bestTimes, "%lu\n", game->BestTDate1[c]);
    }  
    for(int c = 0;c<3; c++){
        fprintf(bestTimes,"%lu\n", game->BestTimes2[c]);
        fprintf(bestTimes, "%lu\n", game->BestTDate2[c]);
    }  
    for(int c = 0;c<3; c++){
        fprintf(bestTimes,"%lu\n", game->BestTimes3[c]);
        fprintf(bestTimes, "%lu\n", game->BestTDate3[c]);
    } 
    fclose(bestTimes);
}
