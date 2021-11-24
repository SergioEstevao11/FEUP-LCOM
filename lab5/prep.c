void *video_mem;

uint8_t RedSize, GreenSize, BlueSize;
uint8_t RedPos, GreenPos, BluePos;

int vh_set_mode(uint16_t mode){
    struct minix_mem_range mr;
    unsigned int vram_base;
    unsigned int vram_size;
    void *video_mem;
    vbe_mode_info info;

    vbe_get_mode_info(mode, &info);
    uint16_t Yres = info.YResolution;
    uint16_t Xres = info.XResolution;
    uint8_t bytesPerPixel = (info.BitsPerPixel + 7) /8;
    uint8_t RedMaskSize;		/* size of direct color red mask in bits */
    uint8_t RedFieldPosition;	/* bit position of lsb of red mask */
    uint8_t GreenMaskSize;	/* size of direct color green mask in bits */
    uint8_t GreenFieldPosition;	/* bit position of lsb of green mask */
    uint8_t BlueMaskSize; 	/* size of direct color blue mask in bits */
    uint8_t BlueFieldPosition;



    mr.mr_base = (phys_bytes) vram_base;
    mr.mr_limit = mr.mr_base + vram_size;
    if( OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
        panic("sys_privctl (ADD_MEM) failed: %d\n", r);
    /* Map memory */
    video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);
    if(video_mem == MAP_FAILED)
        panic("couldn’t map video memory");


    reg86_t r;
    r.ax = 0x4F02; // VBE call, function 02 -- set VBE mode
    r.bx = 1<<14|0x105; // set bit 14: linear framebuffer
    r.intno = 0x10;
    if( sys_int86(&r) != OK ) {
        printf("set_vbe_mode: sys_int86() failed \n");
        return 1;
    }
}