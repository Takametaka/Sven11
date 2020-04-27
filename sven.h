#ifndef _SVEN_H
#define _SVEN_H

#include <X11/Xlib.h>
#include <stdint.h>

typedef struct
{
    int x;
    int y;
} SvenVect2;

typedef struct 
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
} SvenColor;

typedef struct
{
    enum event_type {
        KEY_PRESSED,
        KEY_RELEASED,
        MOUSE_PRESSED,
        MOUSE_RELEASED,
        EXPOSE,
        KILL,
        DUMMY
    } type;
    union event_union {
        SvenVect2 clic; //coordinates
        int key; // use xev
    } data;
} SvenEvent;

typedef struct
{
    Display *display;
    Window window;
    GC gc;
    Colormap colormap;
    int visualClass;
    SvenColor currentColor;
    XFontStruct *fs;
    int update_display; //true if you want to update display
} SvenDisplay;

SvenDisplay *sven_open(int sizeX, int sizeY, char *title);

void sven_close(SvenDisplay *sd);


void sven_fill_color(SvenDisplay *sd, SvenColor color);


void sven_draw_pixel(SvenDisplay *sd, SvenVect2 pixel, SvenColor color);

void sven_draw_line(SvenDisplay *sd, SvenVect2 p1, SvenVect2 p2, SvenColor color);

void sven_draw_polygon(SvenDisplay *sd, SvenVect2 p[], int nbPoints, SvenColor color);
void sven_fill_polygon(SvenDisplay *sd, SvenVect2 p[], int nbPoints, SvenColor color);

void sven_draw_rectangle(SvenDisplay *sd, SvenVect2 p1, SvenVect2 p2, SvenColor color);
void sven_fill_rectangle(SvenDisplay *sd, SvenVect2 p1, SvenVect2 p2, SvenColor color);


int sven_event_pending(SvenDisplay *sd);

SvenEvent sven_get_event(SvenDisplay *sd);


void sven_set_update(SvenDisplay *sd, int val);

#endif
