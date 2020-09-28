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
while(!end) //maybe a isOpen
{
  //check for end here is important if an event triggers the end  
  while( (!end) && sven_event_pending(sd) ) //will leave the event loop is update is set to 1 (change in display)
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
  
}
```

As of today, the `sven_set_update` isn't very useful but I'm planning to do something similar to SFML with all drawing operations staged and then a function to update the display all at once.

## Goals

My first goal is to finish the `sven` module in which I "adapt" a portion of X11 primitives.  
Then, I'll make a `geometry` module which will use the `sven` module to draw more complex shapes.
In the future I'll probably try to Implement a `physics` module adding `physics_objects` that can collide and/or respond to gravity.

I just realised it will look a lot like SFML  

## Modules

First module, the sven Module will contain all drawings primitives, Window functions, and event handling.  

Second Module, geometry, will contain classes for shapes and functions using primitives to draw them.  
It will also add a new function to draw primitives but with a color set at each points (see `sfRenderWindow_drawPrimitive(sfRenderWindow *,VertexArray *, . . .)`)  

### Sven Module

Functions :  

- [x] `void svenWindow_open(SvenDisplay *sd)`
- [x] `void svenWindow_close(SvenDisplay *sd)`
- [x] `int  svenWindow_eventPending(SvenDisplay *sd)`
- [x] `SvenEvent sven_getEvent(SvenDisplay *sd)`
- [ ] `void svenWindow_update(SvenDisplay *);`

- [x] `void svenWindow_fillColor(SvenDisplay *sd,SvenColor c)`
- [x] `void svenWindow_drawPixel(SvenDisplay *sd,SvenVect2,SvenColor c)`
- [x] `void svenWindow_drawLine(SvenDisplay *sd, SvenVect2 p1, SvenVect2 p2, SvenColor color)`
- [ ] `void svenWindow_drawPolygon(SvenDisplay *sd, SvenVect2 p[], int nbPoints, SvenColor color)`
- [x] `void svenWindow_fillPolygon(SvenDisplay *sd, SvenVect2 p[], int nbPoints, SvenColor color)`
- [x] `void svenWindow_drawRectangle(SvenDisplay *sd, SvenVect2 p1, SvenVect2 p2, SvenColor color)`
- [x] `void svenWindow_fillRectangle(SvenDisplay *sd, SvenVect2 p1, SvenVect2 p2, SvenColor color)`
- [ ] `void svenWindow_drawCircle(SvenDisplay *,SvenVect2 center, int radius, SvenColor color)`
- [ ] `void svenWindow_fillCircle(SvenDisplay *,SvenVect2 center, int radius, SvenColor color)`

Data :

- [x] `SvenVect2`
- [x] `SvenColor`
- [x] `SvenEvent` I need to get rid of DUMMY
- [ ] `SvenDisplay` Will probably hold a pointer to a collection of drawings to be displayed when using `sven_update(SvenDisplay *)`
- [ ] A collection, like `std::vector<E>`
