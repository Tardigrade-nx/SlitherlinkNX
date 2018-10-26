#ifndef _SDLUTILS_H_
#define _SDLUTILS_H_

#include "SDL2/SDL.h"

namespace SDL_utils
{
   // Render a texture to the screen
   void renderTexture(SDL_Texture * p_texture, const int p_x, const int p_y);

   // Print a number on the screen
   void renderNumber(const unsigned int p_number, int p_x, int p_y, const int p_colorIndex, const bool p_centered);
}

#endif
