
#ifndef _Xinerama_h
#define _Xinerama_h

#include "SDL_name.h"

typedef struct {
   int   screen_number;
   short x_org;
   short y_org;
   short width;
   short height;
} SDL_NAME(XineramaScreenInfo);

Bool SDL_NAME(XineramaQueryExtension) (
   Display *dpy,
   int     *event_base,
   int     *error_base
);

Status SDL_NAME(XineramaQueryVersion)(
   Display *dpy,
   int     *major,
   int     *minor
);

Bool SDL_NAME(XineramaIsActive)(Display *dpy);



SDL_NAME(XineramaScreenInfo) * 
SDL_NAME(XineramaQueryScreens)(
   Display *dpy,
   int     *number
);

#endif 

