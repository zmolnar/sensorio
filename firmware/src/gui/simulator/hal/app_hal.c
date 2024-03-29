#include <unistd.h>
#include <stdio.h>
#define SDL_MAIN_HANDLED        /*To fix SDL's "undefined reference to WinMain" issue*/
#include <SDL2/SDL.h>

#include "../drivers/monitor.h"
#include "../drivers/keyboard.h"

#include "gui/Sensorio.h"

/**
 * A task to measure the elapsed time for LittlevGL
 * @param data unused
 * @return never return
 */
static int tick_thread(void * data)
{
    (void)data;

    while(1) {
        SDL_Delay(5);   /*Sleep for 5 millisecond*/
        lv_tick_inc(5); /*Tell LittelvGL that 5 milliseconds were elapsed*/
    }

    return 0;
}

extern lv_group_t *enc_group;
void focus_cb(lv_group_t *);

void hal_setup(void)
{
    /* Add a display
     * Use the 'monitor' driver which creates window on PC's monitor to simulate a display*/
    monitor_init();

    static lv_disp_buf_t disp_buf;
    static lv_color_t buf[LV_HOR_RES_MAX * 10];                     /*Declare a buffer for 10 lines*/
    lv_disp_buf_init(&disp_buf, buf, NULL, LV_HOR_RES_MAX * 10);    /*Initialize the display buffer*/

    lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);            /*Basic initialization*/
    disp_drv.flush_cb = monitor_flush;    /*Used when `LV_VDB_SIZE != 0` in lv_conf.h (buffered drawing)*/
    disp_drv.buffer = &disp_buf;
    //disp_drv.disp_fill = monitor_fill;      /*Used when `LV_VDB_SIZE == 0` in lv_conf.h (unbuffered drawing)*/
    //disp_drv.disp_map = monitor_map;        /*Used when `LV_VDB_SIZE == 0` in lv_conf.h (unbuffered drawing)*/
    lv_disp_drv_register(&disp_drv);

#if 1
    keyboard_init();
    encgroup = lv_group_create();

    lv_indev_drv_t indev_enc_drv;
    lv_indev_drv_init(&indev_enc_drv);
    indev_enc_drv.type = LV_INDEV_TYPE_ENCODER;
    indev_enc_drv.read_cb = encoder_read;         /*This function will be called periodically (by the library) to get the mouse position and state*/
    lv_indev_t *enc_indev = lv_indev_drv_register(&indev_enc_drv);
    lv_indev_set_group(enc_indev, encgroup);
#endif

    /* Tick init.
     * You have to call 'lv_tick_inc()' in periodically to inform LittelvGL about how much time were elapsed
     * Create an SDL thread to do this*/
    SDL_CreateThread(tick_thread, "tick", NULL);
}

void hal_loop(void)
{
    while(1) {
        SDL_Delay(5);
        lv_task_handler();
    }
}
