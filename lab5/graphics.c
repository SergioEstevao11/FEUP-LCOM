#include "graphics.h"

void *video_mem; 

uint16_t res_y;
uint16_t res_x;
uint16_t bytes_pixel;
uint16_t bits_pixel;
uint8_t redScreenMask, greenScreenMask, blueScreenMask;
uint8_t GreenFieldPosition, RedFieldPosition, BlueFieldPosition;



int vh_set_mode(uint16_t mode){
    int r;
    struct minix_mem_range mr; 
    unsigned int vram_size;

    vbe_mode_info_t info;
    vbe_get_mode_info(mode, &info);
    res_y = info.YResolution;
    res_x = info.XResolution;
    bits_pixel = info.BitsPerPixel;
    bytes_pixel = (bits_pixel + 7)/8;
    redScreenMask = info.RedMaskSize;
    RedFieldPosition = info.RedFieldPosition;
    greenScreenMask = info.GreenMaskSize;
    GreenFieldPosition = info.GreenFieldPosition;
    blueScreenMask = info.BlueMaskSize;
    BlueFieldPosition = info.BlueFieldPosition;

    vram_size = res_y*res_x*bytes_pixel;


    mr.mr_base = (phys_bytes) info.PhysBasePtr;
    mr.mr_limit = mr.mr_base + vram_size;
    if( OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
        panic("sys_privctl (ADD_MEM) failed: %d\n", r);
    /* Map memory */
    video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);
    if(video_mem == MAP_FAILED)
        panic("couldn’t map video memory");

    reg86_t reg;
    memset(&reg, 0, sizeof(reg));
    reg.ax = 0x4F02; // VBE call, function 02 -- set VBE mode
    reg.bx = 1<<14|mode; // set bit 14: linear framebuffer
    reg.intno = 0x10;
    if( sys_int86(&reg) != OK ) {
        printf("set_vbe_mode: sys_int86() failed \n");
        return 1;
    }
    return 0;
}

int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color){
    for(int c = 0 ; c < height; c++){
        vg_draw_hline(x,y+c,width,color);
    }
    return 0;
}

int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color){
    for(int c = 0; c < len; c++){
        change_color(x+c,y,color);
    }
    return 0;
}

void change_color(uint16_t x, uint16_t y, uint32_t color){
    uint32_t pixel_mem = (uint32_t) video_mem;   
    pixel_mem += (y* res_x + x)*(bytes_pixel);
    memcpy((void *) pixel_mem,&color,bytes_pixel);
}

void pattern(uint16_t mode, uint8_t no_rectangles, uint32_t first, uint8_t step){
    uint16_t x=0, y=0;
    uint32_t color;
    uint16_t width, height;
    width = res_x % no_rectangles;
    width = res_x - width;
    height = res_y % no_rectangles;
    height = res_y - height;
    uint32_t red, green, blue;
    // red = (first << (32 - redScreenMask+RedFieldPosition)) >> (32-redScreenMask);
    // green = (first << (32- greenScreenMask+GreenFieldPosition))>> (32-greenScreenMask);
    // blue = (first << (32 - blueScreenMask+BlueFieldPosition)) >> (32-blueScreenMask);

    red = (first >> RedFieldPosition) & (BIT(redScreenMask) -1);
    green = (first >> GreenFieldPosition) & (BIT(greenScreenMask) -1);
    blue = (first >> BlueFieldPosition) & (BIT(blueScreenMask) -1);

    uint32_t R, G, B;


    for (int c = 0; c < no_rectangles; c++){
        for(int w = 0; w < no_rectangles; w++){
            if (mode == index_mode) {
                color = (first + (c * no_rectangles + w) *  step) % (1 << bits_pixel);
            }
            else {
                R = (red + w*step) % (1<<redScreenMask);
                G = (green + c*step) % (1<<greenScreenMask);
                B = (blue + (c+w)*step)%(1<<blueScreenMask);
                color = 0x00000000 | (R << RedFieldPosition) | (G << GreenFieldPosition) | B;
            }
            vg_draw_rectangle(x, y, width/no_rectangles, height/no_rectangles, color);
            x+= width/no_rectangles;
        }
        x=0;
        y+=height/no_rectangles;
    }
}

void vg_draw_pixmap(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t * bytes){
    int counter = 0;
    for(int h = y; h < height+y; h++){
        for(int w=x; w < width+x; w++){
            change_color(w, h, bytes[counter]);
            counter++;
        }
    }
}

void clear_screen(uint16_t x, uint16_t y, uint16_t width, uint16_t height){
    for(int h = y; h < height+y; h++){
        for(int w=x; w < width+x; w++){
            change_color(w, h, 0x00);
        }
    }
}
