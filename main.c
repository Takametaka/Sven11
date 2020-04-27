#include "sven.h"

#include <unistd.h>
#include <stdio.h>

#define SIZE_X 900
#define SIZE_Y 700

#define NB_POINTS 8

int main()
{
    SvenDisplay *sd = sven_open(SIZE_X,SIZE_Y,"sven - window");
    
    SvenVect2 pts[NB_POINTS];
    int nbClic = 0;

    int end = 0; 
    while(!end)
    {
        /* /!\ First time this loop is used, sven_get_event don't init ev */
        while( (!end) && sven_event_pending(sd) )
        {
            SvenEvent ev = sven_get_event(sd);
            switch(ev.type) 
            {
                case KEY_PRESSED :
                {
                    if(ev.data.key == 9) //Esc
                        end = 1;
                    break;
                }
                case MOUSE_PRESSED : 
                {
                    nbClic = (nbClic >= NB_POINTS ? 0 : nbClic + 1);

                    int cx = ev.data.clic.x;
                    int cy = ev.data.clic.y;

                    SvenVect2 sv1 = (SvenVect2){cx,cy};
                    SvenColor sc1 = (SvenColor){cx*255/SIZE_X,cy*255/SIZE_Y,0};
                    SvenColor sc2 = (SvenColor){255 - cx*255/SIZE_X,255 - cy*255/SIZE_Y,255};

                    sven_fill_color(sd,sc1);
                    pts[nbClic - 1] = sv1;
                    for(int i = 0; i < nbClic; i++)
                        sven_draw_pixel(sd,pts[i],sc2);
                    sven_fill_polygon(sd,pts,nbClic,sc2);
                    sven_set_update(sd,1);
                    break;
                }
                case KILL :
                {
                    end = 1;
                }
                default :
                    break;
            }
        }
        sven_set_update(sd,0);
    }

    sven_close(sd);

    return 0;
}
