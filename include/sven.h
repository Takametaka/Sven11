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

typedef struct _Mouse 
{
    SvenVect2 coordinates;
    int mouse_button; //0 1 2 lmb middle rmb
} MouseEvent;

typedef struct
{
    enum event_type {
        KEY_DOWN,
        KEY_UP,
        MOUSE_DOWN,
        MOUSE_UP,
        EXPOSE,
        KILL,
        DUMMY
    } type;
    union event_union {
        MouseEvent mouse;
        int key; // use xev to get keycodes
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
} SvenDisplay;

/**
 *  \brief opens a window
 *  \param sizeX the width of the window
 *  \param sizeY the height of the window
 *  \param title the title of the window
 *  \return the allocated window
 */
SvenDisplay *svenWindow_open(int sizeX, int sizeY, char *title);

/**
 *  \brief closes and frees the window
 *  \param sd the window to close and free
 */
void svenWindow_close(SvenDisplay *sd);


void svenWindow_fillColor(SvenDisplay *sd, SvenColor color);


void svenWindow_drawPixel(SvenDisplay *sd, SvenVect2 pixel, SvenColor color);

void svenWindow_drawLine(SvenDisplay *sd, SvenVect2 p1, SvenVect2 p2, SvenColor color);

void svenWindow_drawPolygon(SvenDisplay *sd, SvenVect2 p[], int nbPoints, SvenColor color);
void svenWindow_fillPolygon(SvenDisplay *sd, SvenVect2 p[], int nbPoints, SvenColor color);

void svenWindow_drawRectangle(SvenDisplay *sd, SvenVect2 p1, SvenVect2 p2, SvenColor color);
void svenWindow_fillRectangle(SvenDisplay *sd, SvenVect2 p1, SvenVect2 p2, SvenColor color);


int svenWindow_eventPending(SvenDisplay *sd);

SvenEvent svenWindow_getEvent(SvenDisplay *sd);

#endif
