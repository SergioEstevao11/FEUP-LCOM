#include "graphics.h"

void *video_mem;
void *video_buffer;

uint16_t res_y;
uint16_t res_x;
uint16_t bytes_pixel;
uint16_t bits_pixel;


int vh_set_mode(uint16_t mode){
    int r;
    struct reg86 reg;
    struct minix_mem_range mr;
    unsigned int vram_size = 0;

    vbe_mode_info_t info;
    vbe_get_mode_info(mode,&info);
    res_x = info.XResolution;
    res_y = info.YResolution;
    bits_pixel = info.BitsPerPixel;
    bytes_pixel = (info.BitsPerPixel + 7) / 8 ; // +7 para arrendondar para cima

    vram_size = res_x*res_y*bytes_pixel;

    video_buffer = malloc(vram_size);

    mr.mr_base = (phys_bytes) info.PhysBasePtr;
    mr.mr_limit = mr.mr_base + vram_size;
    if( OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
        panic("sys_privctl (ADD_MEM) failed: %d\n", r);
    
    video_mem = vm_map_phys(SELF, (void*)mr.mr_base, vram_size);
    if(video_mem == MAP_FAILED)
        panic("couldn’t map video memory");
    
    memset(&reg, 0, sizeof(reg));

    reg.ax = 0x4F02; // VBE call, function 02 -- set VBE mode
    reg.bx = 1<<14|mode; // set bit 14: linear framebuffer
    reg.intno = 0x10;
    if (sys_int86(&reg) != OK) {
        printf("set_vbe_mode: sys_int86() failed \n");
        return 1;
    }
    return 0;
}

void double_buffer(){
    memcpy(video_mem, video_buffer, res_x*res_y*bytes_pixel);
}

void free_buffer(){
    free(video_buffer);
}

void change_color(uint16_t x, uint16_t y, uint32_t color){
    if (color != TrasparencyColor){
        uint32_t pixel_mem = (uint32_t) video_buffer;   
        pixel_mem += (y* res_x + x)*(bytes_pixel);
        memcpy((void *) pixel_mem,&color,bytes_pixel);
    }
}



void vg_draw_pixmap(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t * bytes){
    int counter = 0;
    for(int h = y; h < height+y; h++){
        for(int w=x; w < width+x; w++){
            change_color(w, h, bytes[counter]);
            counter++;
        }
    }

}

void clear_screen(Game game){
    
    vg_draw_pixmap(0,0,res_x,res_y,game.background_color);
    if (game.mode == inGame || game.mode == victoryscreen || game.mode == deathscreen || game.mode == paused){
        switch(game.level){
        
            case 1:
                vg_draw_pixmap(0,0,game.finishline_Img.width,game.finishline_Img.height,game.finishline_color);
                for(int c= 0;c <= 600; c+=200)
                    vg_draw_pixmap(c,120,game.river_Img.width, game.river_Img.height,game.river_color);
                break;
            case 2:
                vg_draw_pixmap(0,0,game.finishline_Img.width,game.finishline_Img.height,game.finishline_color);
                for(int c= 0;c <= 600; c+=200)
                    vg_draw_pixmap(c,425,game.river_Img.width, game.river_Img.height,game.river_color);
                for(int h= 0;h <= 600; h+=200)
                    vg_draw_pixmap(h,180,game.river_Img.width, game.river_Img.height,game.river_color);
                break;
            case 3:
                vg_draw_pixmap(0,0,game.finishline_Img.width,game.finishline_Img.height,game.finishline_color);
                for(int c= 0;c <= 600; c+=200)
                    vg_draw_pixmap(c,425,game.river_Img.width, game.river_Img.height,game.river_color);
                for(int h= 0;h <= 600; h+=200)
                    vg_draw_pixmap(h,180,game.river_Img.width, game.river_Img.height,game.river_color);
                for(int l= 0;l <= 600; l+=200)
                    vg_draw_pixmap(l,365,game.river_Img.width, game.river_Img.height,game.river_color);
                break;
            default:
                break;
        }
    }
}

void vg_draw_Buttons(int begin, int end, Button *buttons, ButtonType highlighted){
    for(int c = begin; c < end;c++){
        if(buttons[c].type == highlighted){
            vg_draw_pixmap(buttons[c].x, buttons[c].y, buttons[c].highlighted.width, buttons[c].highlighted.height, buttons[c].Highlighted_color);
        }
        else{
            vg_draw_pixmap(buttons[c].x, buttons[c].y, buttons[c].normal.width, buttons[c].normal.height, buttons[c].Normal_color);
        }
    }
}

void vg_draw_char(Game game, char a, int x, int y){
    int step =29 * (a - '/');
    int counter = 0;
    counter = step*game.numbers_Img.width;
    for(int h = y; h < y+ 29; h++){
        for(int w=x; w < x+game.numbers_Img.width; w++){
            change_color(w, h, game.numbers_color[counter]);
            counter++;
        }
    }
}

void vg_draw_int(Game game, int num, int*x, int y){
    char a;
    a = '0'+ num/10;
    vg_draw_char(game, a, *x, y);
    *x+= 14;
    a = '0'+ num%10;
    vg_draw_char(game, a, *x, y);
    *x+=14;
}

void vg_draw_date(Game game, unsigned long date, int *x, int y){
    int d, m , year;
    year=date%100;
    date = (date-year)/100;
    m=date%100;
    date = (date-m)/100;
    d = date;
    vg_draw_int(game, d, x, y);
    vg_draw_char(game, '/', *x, y);
    *x+=14;
    vg_draw_int(game, m, x, y);
    vg_draw_char(game, '/', *x, y);
    *x+=14;
    vg_draw_int(game, year, x, y);
}

void vg_draw_time(Game game, unsigned long time, int *x, int y){
    int ms, s, min;
    ms = time%100;
    time = (time-ms)/100;
    s = (time)%60;
    time = (time-s)/60;
    min = time;

    vg_draw_int(game, min, x, y);
    vg_draw_char(game, ':', *x, y);
    *x+=14;
    vg_draw_int(game, s, x, y);
    vg_draw_char(game, ':', *x, y);
    *x+=14;
    vg_draw_int(game, ms, x, y);
    *x+= 14*2;
}


void vg_draw_besttimes(Game game){
    int y = 330, x=280;
    switch (game.level){
        case 1:
            for (int c = 0; c < 3; c++){
                if (game.BestTimes1[c] != 0){
                    vg_draw_time(game, game.BestTimes1[c], &x, y);
                    vg_draw_date(game, game.BestTDate1[c], &x, y);
                }
                y+=40;
                x=280;
            }
            break;
        case 2:
            for (int c = 0; c < 3; c++){
                if (game.BestTimes1[c] != 0){
                    vg_draw_time(game, game.BestTimes2[c], &x, y);
                    vg_draw_date(game, game.BestTDate2[c], &x, y);
                }
                x=280;
                y+=40;
            }
            break;
        case 3:
            for (int c = 0; c < 3; c++){
                if (game.BestTimes1[c] != 0){
                    vg_draw_time(game, game.BestTimes3[c], &x, y);
                    vg_draw_date(game, game.BestTDate3[c], &x, y);
                }
                x=280;
                y+=40;
            }
            break;
    }
    return;
}



void vg_draw_object(Object * objects,int size){
    for(int c = 0; c<size; c++){
        vg_draw_pixmap(objects[c].x,objects[c].y,objects[c].img.width,objects[c].img.height,objects[c].Color);
    }

}
void vg_draw_Penguin(Penguin Penguin){
    switch (Penguin.pos){
        case up1:{
            vg_draw_pixmap(Penguin.x, Penguin.y, Penguin.ImgUp1.width, Penguin.ImgUp1.height, Penguin.ColorUp1);
            break;
        }
        case up2:{
            vg_draw_pixmap(Penguin.x, Penguin.y, Penguin.ImgUp2.width, Penguin.ImgUp2.height, Penguin.ColorUp2);
            break;
        }
        case up3:{
            vg_draw_pixmap(Penguin.x, Penguin.y, Penguin.ImgUp3.width, Penguin.ImgUp3.height, Penguin.ColorUp3);
            break;
        }
        case down1:{
            vg_draw_pixmap(Penguin.x, Penguin.y, Penguin.ImgDown1.width, Penguin.ImgDown1.height, Penguin.ColorDown1);
            break;
        }
        case down2:{
            vg_draw_pixmap(Penguin.x, Penguin.y, Penguin.ImgDown2.width, Penguin.ImgDown2.height, Penguin.ColorDown2);
            break;
        }
        case down3:{
            vg_draw_pixmap(Penguin.x, Penguin.y, Penguin.ImgDown3.width, Penguin.ImgDown3.height, Penguin.ColorDown3);
            break;
        }
        case right1:{
            vg_draw_pixmap(Penguin.x, Penguin.y, Penguin.ImgRight1.width, Penguin.ImgRight1.height, Penguin.ColorRight1);
            break;
        }
        case right2:{
            vg_draw_pixmap(Penguin.x, Penguin.y, Penguin.ImgRight2.width, Penguin.ImgRight2.height, Penguin.ColorRight2);
            break;
        }
        case right3:{
            vg_draw_pixmap(Penguin.x, Penguin.y, Penguin.ImgRight3.width, Penguin.ImgRight3.height, Penguin.ColorRight3);
            break;
        }
        case left1:{
            vg_draw_pixmap(Penguin.x, Penguin.y, Penguin.ImgLeft1.width, Penguin.ImgLeft1.height, Penguin.ColorLeft1);
            break;
        }
        case left2:{
            vg_draw_pixmap(Penguin.x, Penguin.y, Penguin.ImgLeft2.width, Penguin.ImgLeft2.height, Penguin.ColorLeft2);
            break;
        }
        case left3:{
            vg_draw_pixmap(Penguin.x, Penguin.y, Penguin.ImgLeft3.width, Penguin.ImgLeft3.height, Penguin.ColorLeft3);
            break;
        }

    }
}

void update_screen(Penguin Penguin,Object * objects,Game *game){
    int size, x=10;
    switch(game->level){
        case 1:
            size = 6;  
            break;    
        case 2:
            size = 8;
            break;
        case 3:
            size = 7;
            break;
    }
    clear_screen(*game);
    vg_draw_object(objects,size);
    vg_draw_Penguin(Penguin);
    vg_draw_time(*game, game->gameTick, &x, 570);
    double_buffer();
}


void update_menu(Button * buttons, Game *game, ButtonType highlighted, Mouse mouse, Penguin Penguin,Object * objects){

    clear_screen(*game);
    if (game->mode == end){
        return;
    }
    if (game->mode == mainmenu){
        vg_draw_pixmap(200, 50, game->title_Img.width, game->title_Img.height, game->title_color);
        vg_draw_pixmap(385, 180, Penguin.ImgRight1.width, Penguin.ImgRight1.height, Penguin.ColorRight1);
        vg_draw_Buttons(0, 2, buttons, highlighted);
        vg_draw_pixmap(mouse.x -10, mouse.y, mouse.img.width, mouse.img.height, mouse.color);
    }
    
    else if(game->mode == singleplayer){
        vg_draw_pixmap(200, 50, game->title_Img.width, game->title_Img.height, game->title_color);
        vg_draw_Buttons(2, 9, buttons, highlighted);
        vg_draw_pixmap(mouse.x -10, mouse.y, mouse.img.width, mouse.img.height, mouse.color);
    }
    else if(game->mode == BestTime){
        vg_draw_pixmap(225, 132, game->besttimescreen_Img.width, game->besttimescreen_Img.height, game->besttimescreen_color);
        vg_draw_Buttons(9, 10, buttons, highlighted);
        vg_draw_char(*game, '0'+game->level, 440, 213);
        vg_draw_besttimes(*game);
        vg_draw_pixmap(mouse.x -10, mouse.y, mouse.img.width, mouse.img.height, mouse.color);
    }

    else{
        int size, x=10;
        switch(game->level){
            case 1:
                size = 6;  
                break;    
            case 2:
                size = 8;
                break;
            case 3:
                size = 7;
                break;
        }
        vg_draw_Penguin(Penguin);
        vg_draw_object(objects,size);
        vg_draw_time(*game, game->gameTick, &x, 570);
        if (game->mode == deathscreen){
            vg_draw_pixmap(300, 175, game->deathscreen_Img.width, game->deathscreen_Img.height, game->deathscreen_color);
            vg_draw_Buttons(11, 13, buttons, highlighted);
        }
        
        else if(game->mode == victoryscreen){
            int x = 345;
            vg_draw_pixmap(250, 82, game->victoryscreen_Img.width, game->victoryscreen_Img.height, game->victoryscreen_color);
            vg_draw_Buttons(11, 12, buttons, highlighted);
            vg_draw_time(*game, game->gameTick, &x, 270);
        }
        
        else{
            vg_draw_pixmap(275, 175, game->pausescreen_Img.width, game->pausescreen_Img.height, game->pausescreen_color);
            vg_draw_Buttons(10, 12, buttons, highlighted);

        }
        vg_draw_pixmap(mouse.x -10, mouse.y, mouse.img.width, mouse.img.height, mouse.color);
    }
    double_buffer();
    
}
