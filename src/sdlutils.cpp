#include <sstream>
#include "sdlutils.h"
#include "def.h"
#include "resourceManager.h"

void SDL_utils::renderTexture(SDL_Texture * p_texture, const int p_x, const int p_y)
{
   // Render texture
   SDL_Rect destRect;
   destRect.x = p_x;
   destRect.y = p_y;
   SDL_QueryTexture(p_texture, NULL, NULL, &destRect.w, &destRect.h);
   SDL_RenderCopy(g_renderer, p_texture, NULL, &destRect);
}

void SDL_utils::renderNumber(const unsigned int p_number, int p_x, int p_y, const int p_colorIndex, const bool p_centered)
{
   std::ostringstream oss;
   oss << p_number;
   std::string numberString(oss.str());
   unsigned int numberLength = numberString.length();
   // Centered text
   if (p_centered)
      p_x -= numberLength * 21 / 2;
   SDL_Rect srcRect;
   srcRect.y = p_colorIndex * 26;
   srcRect.w = 21;
   srcRect.h = 26;
   SDL_Rect destRect;
   destRect.x = p_x;
   destRect.y = p_y;
   destRect.w = 21;
   destRect.h = 26;
   for (unsigned int index = 0; index < numberLength; ++index)
   {
      srcRect.x = 21 * (numberString[index] - '0');
      SDL_RenderCopy(g_renderer, g_resourceManager->getTexture(T_TEXTURE_NUMBERS), &srcRect, &destRect);
      destRect.x += 21;
   }
}
