#include <string>
#include <iostream>
#include <SDL2/SDL_image.h>
#include "resourceManager.h"
#include "def.h"

// Constructor
CResourceManager::CResourceManager(void)
{
   INHIBIT(std::cout << "Resource Manager: create" << std::endl;)
   int index(0);
   // Init texture collection
   for (index = 0; index < NB_TEXTURES; ++index)
   {
      m_textures[index] = NULL;
   }
   // Init sounds collections, all sounds are preloaded
   for (index = 0; index < NB_SOUNDS; ++index)
   {
      m_sounds[index] = NULL;
      getSound(static_cast<T_SOUND_ID>(index));
   }
}

// Destructor
CResourceManager::~CResourceManager(void)
{
   INHIBIT(std::cout << "Resource Manager: clean up" << std::endl;)
   // Free all textures
   int index(0);
   for (index = 0; index < NB_TEXTURES; ++index)
   {
      if (m_textures[index] != NULL)
      {
         SDL_DestroyTexture(m_textures[index]);
         m_textures[index] = NULL;
      }
   }
   // Free all sounds
   for (index = 0; index < NB_SOUNDS; ++index)
   {
      if (m_sounds[index] != NULL)
      {
         Mix_FreeChunk(m_sounds[index]);
         m_sounds[index] = NULL;
      }
   }
}

// Load a texture
SDL_Texture *CResourceManager::getTexture(const T_TEXTURE_ID p_textureId)
{
   // If the texture is already loaded, return it
   if (m_textures[p_textureId] != NULL)
      return m_textures[p_textureId];

   // Load the surface
   std::string texturePath(PATH_PREFIX);
   switch (p_textureId)
   {
      case T_TEXTURE_BACKGROUND:       texturePath += "background.png";    break;
      case T_TEXTURE_FRAME:            texturePath += "frame.png";         break;
      case T_TEXTURE_FRAME2:           texturePath += "frame2.png";        break;
      case T_TEXTURE_TITLE:            texturePath += "title.png";         break;
      case T_TEXTURE_GRID5:            texturePath += "grid5.png";         break;
      case T_TEXTURE_GRID7:            texturePath += "grid7.png";         break;
      case T_TEXTURE_GRID10:           texturePath += "grid10.png";        break;
      case T_TEXTURE_LINEH:            texturePath += "lineH.png";         break;
      case T_TEXTURE_LINEV:            texturePath += "lineV.png";         break;
      case T_TEXTURE_CROSS:            texturePath += "cross.png";         break;
      case T_TEXTURE_CROSS2:           texturePath += "cross2.png";        break;
      case T_TEXTURE_NUMBERS:          texturePath += "numbers.png";       break;
      case T_TEXTURE_BORDER_TOP:       texturePath += "borderTop.png";     break;
      case T_TEXTURE_BORDER_LEFT:      texturePath += "borderLeft.png";    break;
      case T_TEXTURE_BORDER_BOTTOM:    texturePath += "borderBottom.png";  break;
      case T_TEXTURE_BORDER_RIGHT:     texturePath += "borderRight.png";   break;
      case T_TEXTURE_ARROW_UP:         texturePath += "arrowUp.png";       break;
      case T_TEXTURE_ARROW_LEFT:       texturePath += "arrowLeft.png";     break;
      case T_TEXTURE_ARROW_DOWN:       texturePath += "arrowDown.png";     break;
      case T_TEXTURE_ARROW_RIGHT:      texturePath += "arrowRight.png";    break;
      case T_TEXTURE_LEVEL_SELECT:     texturePath += "levelSelect.png";   break;
      case T_TEXTURE_MENU_PAUSE:       texturePath += "menuPause.png";     break;
      case T_TEXTURE_MENU_CURSOR:      texturePath += "menuCursor.png";    break;
      case T_TEXTURE_LEVEL:            texturePath += "level.png";         break;
      case T_TEXTURE_SOLVED:           texturePath += "solved.png";        break;
      default: break;
   }
   INHIBIT(std::cout << "Resource Manager: loading texture '" << texturePath << "'" << std::endl;)
   SDL_Surface* surface = IMG_Load(texturePath.c_str());
   if (surface == NULL)
   {
      std::cerr << "Unable to load image '" << texturePath << "'! SDL_Error: " << SDL_GetError() << std::endl;
      return NULL;
   }

   // Create the texture from the surface
   SDL_Texture* texture = SDL_CreateTextureFromSurface(g_renderer, surface);
   if (texture == NULL)
   {
      std::cerr << "Unable to create texture from '" << texturePath << "'! SDL_Error: " << SDL_GetError() << std::endl;
      SDL_FreeSurface(surface);
      return NULL;
   }
   SDL_FreeSurface(surface);

   // Store the texture for next time
   m_textures[p_textureId] = texture;

   // Return the texture
   return texture;
}

// Get a sound (load it if necessary)
Mix_Chunk* CResourceManager::getSound(const T_SOUND_ID p_soundId)
{
   // If the sound is already loaded, return it
   if (m_sounds[p_soundId] != NULL)
      return m_sounds[p_soundId];

   // Load the sound
   std::string soundPath(PATH_PREFIX);
   switch (p_soundId)
   {
      case T_SOUND_MOVE:   soundPath += "soundMove.dat"; break;
      case T_SOUND_DRAW:   soundPath += "soundDraw.dat"; break;
      case T_SOUND_UNDO:   soundPath += "soundUndo.dat"; break;
      case T_SOUND_SOLVED: soundPath += "soundSolved.dat"; break;
      case T_SOUND_START:  soundPath += "soundStart.dat"; break;
      case T_SOUND_BACK:   soundPath += "soundBack.dat"; break;
      case T_SOUND_NEXT:   soundPath += "soundNext.dat"; break;
      default: break;
   }
   INHIBIT(std::cout << "Resource Manager: loading sound '" << soundPath << "'" << std::endl;)
   Mix_Chunk *sound = Mix_LoadWAV(soundPath.c_str());
   if (sound == NULL)
   {
      std::cerr << "Unable to load sound '" << soundPath << "'! Mix_GetError: " << Mix_GetError() << std::endl;
      return NULL;
   }

   // Store the sound for next time
   m_sounds[p_soundId] = sound;

   return sound;
}
