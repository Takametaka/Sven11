#include "collections.h"
#include "sven.h"

#include <stdio.h>
#include <stdlib.h>

/**
 *  \brief opens a window
 *  \param sizeX the width of the window
 *  \param sizeY the height of the window
 *  \param title the title of the window
 *  \return the allocated window
 */
SvenDisplay *svenWindow_open(int sizeX, int sizeY, char *title)
{
    SvenDisplay *sd = malloc(sizeof(SvenDisplay));
    if( NULL == sd )
    {
        fprintf(stderr,"Failed to malloc SvenDisplay @sven_open\n");
        exit(1);
    }

    /* Opening display */
    if( NULL == (sd->display = XOpenDisplay(NULL)) )
    {
        fprintf(stderr,"Failed to open the display @sven_open\n");
        exit(1);
    }

    /* Creating window */
    int screen_number = DefaultScreen(sd->display);
    unsigned long black_pixel = BlackPixel(sd->display, screen_number);
    unsigned long white_pixel = WhitePixel(sd->display, screen_number);
    sd->window = XCreateSimpleWindow(sd->display, RootWindow(sd->display,screen_number), 0, 0, sizeX, sizeY, 0, black_pixel, black_pixel);

    /* Setting Window attributes */
    XSetWindowAttributes att;
    att.backing_store = Always;
    att.event_mask = StructureNotifyMask | ExposureMask | KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask;
    XChangeWindowAttributes(sd->display, sd->window, CWBackingStore | CWEventMask, &att);
    
    /* Setting GC (and default font) */
    XGCValues gcv;
    XFontStruct *fs = NULL;
    if( (fs = XLoadQueryFont(sd->display, "fixed")) == NULL)
    {
        fprintf(stderr,"Failed to load fixed font @sven_open\n");
        exit(1);
    }
    sd->fs = fs;
    gcv.font = fs->fid;
    gcv.foreground = white_pixel;
    sd->gc = XCreateGC(sd->display, sd->window, GCFont | GCForeground, &gcv);
    
    /* Displaying the window */
    XMapWindow(sd->display, sd->window);

    /* Setting the Window's name */
    XStoreName(sd->display, sd->window, title);
    
    /* Getting the colormap */
    sd->colormap = DefaultColormap(sd->display, screen_number);

    /* Getting the Visual (mostly to know how to display colors) */
    Visual *visual = DefaultVisual(sd->display, screen_number);
    sd->visualClass = (visual == NULL ? TrueColor : visual->class);
    
    /* init some display variables */
    int r = (white_pixel & 0xFF0000) >> 16;
    int g = (white_pixel & 0x00FF00) >> 8;
    int b = (white_pixel & 0x0000FF);
    sd->currentColor = (SvenColor){r,g,b};
    
    /* wait for the window to be displayed */
    XEvent xev;
    int canShow = 0;
    while(!canShow)
    {
        XNextEvent(sd->display,&xev);
        canShow = (xev.type == MapNotify);
    }

    return sd;
}

/**
 *  \brief closes and frees the window
 *  \param sd the window to close and free
 */
void svenWindow_close(SvenDisplay *sd)
{
    if( sd == NULL ) return;
    
    XFreeFont(sd->display,sd->fs);
    XFreeGC(sd->display, sd->gc);
    XCloseDisplay(sd->display);
    free(sd);
}

//inner
void _switch_color(SvenDisplay *sd, SvenColor color)
{
    if( sd == NULL ) return;

    //if color is the same, don't change it
    int boolR = sd->currentColor.r == color.r;
    int boolG = sd->currentColor.g == color.g;
    int boolB = sd->currentColor.b == color.b;
    if( boolR && boolG && boolB ) return;

    XColor xcolor;
    int vc = sd->visualClass;
    if( vc == TrueColor || vc == StaticColor || vc == StaticGray)
    {
        //allocate color
        xcolor.pixel = 0;
        xcolor.red = color.r << 8;
        xcolor.green = color.g << 8;
        xcolor.blue = color.b << 8;
        XAllocColor(sd->display, sd->colormap, &xcolor);
    }
    else //don't
        xcolor.pixel = ((color.r << 16) | (color.g << 8) | (color.b));

    sd->currentColor = color;
    XSetForeground(sd->display, sd->gc, xcolor.pixel);
}

void svenWindow_fillColor(SvenDisplay *sd, SvenColor color)
{    
    _switch_color(sd,color);

    unsigned int w, h, b, d;
    int x, y;
    Window root;
    XGetGeometry(sd->display, sd->window, &root, &x, &y, &w, &h, &b, &d);
    (void)b; (void)d; (void)x; (void)y; (void)root;
    XFillRectangle(sd->display, sd->window, sd->gc, 0, 0, w, h);
}

void svenWindow_drawPixel(SvenDisplay *sd, SvenVect2 pixel, SvenColor color)
{
    _switch_color(sd,color);
    XDrawPoint(sd->display, sd->window, sd->gc, pixel.x, pixel.y);
}

void svenWindow_drawLine(SvenDisplay *sd, SvenVect2 p1, SvenVect2 p2, SvenColor color)
{
    _switch_color(sd,color);
    XDrawLine(sd->display, sd->window, sd->gc, p1.x, p1.y, p2.x, p2.y);
}

void svenWindow_drawPolygon(SvenDisplay *sd, SvenVect2 p[], int nbPoints, SvenColor color)
{
    if(nbPoints < 2) 
        return;

    _switch_color(sd,color);
    for(int i = 0; i < nbPoints - 1; i++)
        XDrawLine(sd->display, sd->window, sd->gc, p[i].x, p[i].y, p[i+1].x, p[i+1].y);
    XDrawLine(sd->display, sd->window, sd->gc, p[nbPoints - 1].x, p[nbPoints - 1].y, p[0].x, p[0].y);
}

void svenWindow_fillPolygon(SvenDisplay *sd, SvenVect2 p[], int nbPoints, SvenColor color)
{
    if(nbPoints < 2) 
        return;

    _switch_color(sd,color);

    XPoint pts[nbPoints];
    for(int i = 0; i < nbPoints; i++)
    {
        pts[i].x = p[i].x;
        pts[i].y = p[i].y;
    }
    XFillPolygon(sd->display,sd->window,sd->gc,pts,nbPoints,Complex,CoordModeOrigin);
}

void svenWindow_drawRectangle(SvenDisplay *sd, SvenVect2 p1, SvenVect2 p2, SvenColor color)
{
    _switch_color(sd,color);
    int x = (p1.x < p2.x ? p1.x : p2.x);
    int y = (p1.y < p2.y ? p1.y : p2.y);
    int w = (p1.x < p2.x ? p2.x - p1.x : p1.x - p2.x);
    int h = (p1.y < p2.y ? p2.y - p1.y : p1.y - p2.y);
    XDrawRectangle(sd->display, sd->window, sd->gc, x, y, w, h);
}

void svenWindow_fillRectangle(SvenDisplay *sd, SvenVect2 p1, SvenVect2 p2, SvenColor color)
{
    _switch_color(sd,color);
    int x = (p1.x < p2.x ? p1.x : p2.x);
    int y = (p1.y < p2.y ? p1.y : p2.y);
    int w = (p1.x < p2.x ? p2.x - p1.x : p1.x - p2.x);
    int h = (p1.y < p2.y ? p2.y - p1.y : p1.y - p2.y);
    XFillRectangle(sd->display, sd->window, sd->gc, x, y, w, h);
}

int svenWindow_eventPending(SvenDisplay *sd)
{
    if(sd == NULL) return 0;
    //wait for events only if there are some events to wait, of if there is no change in the display
    return XPending(sd->display);
}

SvenEvent svenWindow_getEvent(SvenDisplay *sd)
{
    if(sd == NULL)
    {
        fprintf(stderr,"Display not allocated @sven_get_event\n");
        exit(1);
    }
    
    /* get XEvent */
    SvenEvent event;
    XEvent xev;
    XNextEvent(sd->display, &xev);
    
    /* make it a SvenEvent */
    switch(xev.type)
    {
        case KeyPress :
            event.type = KEY_DOWN;
            event.data.key = xev.xkey.keycode;
            break;
        case KeyRelease :
            event.type = KEY_UP;
            event.data.key = xev.xkey.keycode;
            break;
        case ButtonPress :
            event.type = MOUSE_DOWN;
            event.data.mouse.coordinates = (SvenVect2){xev.xbutton.x,xev.xbutton.y};
            break;
        case ButtonRelease :
            event.type = MOUSE_UP;
            event.data.mouse.coordinates = (SvenVect2){xev.xbutton.x,xev.xbutton.y};
            break;
        case Expose :
            event.type = EXPOSE; 
            break;
        case DestroyNotify:
            event.type = KILL;
            break;
        default :
            event.type = DUMMY;
    }

    return event;
}
