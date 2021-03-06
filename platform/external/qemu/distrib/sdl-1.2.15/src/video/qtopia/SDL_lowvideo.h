/*
    SDL - Simple DirectMedia Layer
    Copyright (C) 1997-2012 Sam Lantinga

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

    Sam Lantinga
    slouken@libsdl.org
*/
#include "SDL_config.h"

#ifndef _SDL_lowvideo_h
#define _SDL_lowvideo_h

#include "SDL_mouse.h"
#include "../SDL_sysvideo.h"

#define _THIS	SDL_VideoDevice *_this

struct SDL_PrivateVideoData {
  
  SDL_QWin *SDL_Win;

  
#define NUM_MODELISTS	4		
  int SDL_nummodes[NUM_MODELISTS];
  SDL_Rect **SDL_modelist[NUM_MODELISTS];

  
  WMcursor *BlankCursor;

  
  Uint32 last_buttons;
  QPoint last_point;

  
  int key_flip;
  
};
#define SDL_Win		(_this->hidden->SDL_Win)
#define saved_mode	(_this->hidden->saved_mode)
#define SDL_nummodes	(_this->hidden->SDL_nummodes)
#define SDL_modelist	(_this->hidden->SDL_modelist)
#define SDL_BlankCursor	(_this->hidden->BlankCursor)
#define last_buttons	(_this->hidden->last_buttons)
#define last_point	(_this->hidden->last_point)
#define key_flip	(_this->hidden->key_flip)
#define keyinfo		(_this->hidden->keyinfo)

#endif 
