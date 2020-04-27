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
