# Sven11
Sven11 is a small graphics library based on X11

It can :  
- Draw a single pixel (using XDrawPoint)
- Draw a line (using XDrawLine)
- Draw a rectangle (using XDrawRectangle or XDrawFillRectangle)
- Draw a polygon (using XFillPolygon or multiple XDrawLine)
- Get keyboard and clic events

## Events

To get event you can use the following loop : 
```c
SvenDisplay *sd = sven_open(sizeX,sizeY,"title");

int end = 0;
while(!end)
{
  //check for end here is important if an event triggers the end 
  while( (!end) && sven_event_pending(sd) ) //will leave the event loop is update is set to 1 (chang in display)
  {
    SvenEvent ev = sven_get_event(sd);
    switch(ev.type)
    {
      case KEY_PRESSED : 
        sven_set_update(sd,1); //tell that there will be a change in display
        break;
      case MOUSE_PRESSED : break;
      // etc (full list of event.type at sven.h
    }
  }
  
  sven_set_update(sd,0);
}
```

As of today, the `sven_set_update` isn't very useful but I'm plannig to do something similar to SFML with all drawing operations staged and then a function to update the display all at once.

## Goals

My first goal is to finish the `sven` module in which I "adapt" a portion of X11 primitives. 
Then, I'll make a `geometry` module which will use the `sven` module to draw more complex shapes.
In the future I'll probably try to Implement a `physics` module adding `physics_objects` that can collide and/or respond to gravity.
And if I get crazy, I'll probably implement even more 2D-Game-Engine features.

### Sven Module

Functions : 
- [x] `void sven_open(SvenDisplay *sd)`
- [x] `void sven_close(SvenDisplay *sd)`
- [x] `void sven_fill_color(SvenDisplay *sd,SvenColor c)`
- [x] `void sven_draw_pixel(SvenDisplay *sd,SvenVect2,SvenColor c)`
- [x] `void sven_draw_line(SvenDisplay *sd, SvenVect2 p1, SvenVect2 p2, SvenColor color)`
- [] `void sven_draw_polygon(SvenDisplay *sd, SvenVect2 p[], int nbPoints, SvenColor color)`
- [x] `void sven_fill_polygon(SvenDisplay *sd, SvenVect2 p[], int nbPoints, SvenColor color)`
- [x] `void sven_draw_rectangle(SvenDisplay *sd, SvenVect2 p1, SvenVect2 p2, SvenColor color)`
- [x] `void sven_fill_rectangle(SvenDisplay *sd, SvenVect2 p1, SvenVect2 p2, SvenColor color)`
- [] `void sven_draw_circle(SvenDisplay *,SvenVect2 center, int radius, SvenColor color)`
- [] `void sven_fill_circle(SvenDisplay *,SvenVect2 center, int radius, SvenColor color)`
- [x] `int sven_event_pending(SvenDisplay *sd)`
- [x] `SvenEvent sven_get_event(SvenDisplay *sd)`
- [] `void sven_set_update(SvenDisplay *sd, int val)` will be changed
- [] `void sven_update(SvenDisplay *);
And maybe Ellipses and sprites

Data :
- [x] `SvenVect2`
- [x] `SvenColor`
- [x] `SvenEvent` I need to get rid of DUMMY
- [] `SvenDisplay` Will probably hold a pointer to a collection of drawings to be displayed when using `sven_update(SvenDisplay *)`

### Geometry Module
