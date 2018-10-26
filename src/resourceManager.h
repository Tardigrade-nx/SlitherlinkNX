#ifndef _RESOURCEMANAGER_H_
#define _RESOURCEMANAGER_H_

#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#define NB_TEXTURES 25
#define NB_SOUNDS 7

typedef enum
{
   T_TEXTURE_BACKGROUND = 0,
   T_TEXTURE_FRAME,
   T_TEXTURE_FRAME2,
   T_TEXTURE_TITLE,
   T_TEXTURE_GRID5,
   T_TEXTURE_GRID7,
   T_TEXTURE_GRID10,
   T_TEXTURE_LINEH,
   T_TEXTURE_LINEV,
   T_TEXTURE_CROSS,
   T_TEXTURE_CROSS2,
   T_TEXTURE_NUMBERS,
   T_TEXTURE_BORDER_TOP,
   T_TEXTURE_BORDER_LEFT,
   T_TEXTURE_BORDER_BOTTOM,
   T_TEXTURE_BORDER_RIGHT,
   T_TEXTURE_ARROW_UP,
   T_TEXTURE_ARROW_LEFT,
   T_TEXTURE_ARROW_DOWN,
   T_TEXTURE_ARROW_RIGHT,
   T_TEXTURE_LEVEL_SELECT,
   T_TEXTURE_MENU_PAUSE,
   T_TEXTURE_MENU_CURSOR,
   T_TEXTURE_LEVEL,
   T_TEXTURE_SOLVED
}
T_TEXTURE_ID;

typedef enum
{
   T_SOUND_NOSOUND = -1,
   T_SOUND_MOVE,
   T_SOUND_DRAW,
   T_SOUND_UNDO,
   T_SOUND_SOLVED,
   T_SOUND_START,
   T_SOUND_BACK,
   T_SOUND_NEXT
}
T_SOUND_ID;

class CResourceManager
{
   public:

   // Constructor
   CResourceManager(void);

   // Destructor
   virtual ~CResourceManager(void);

   // Get a texture (load it if necessary)
   SDL_Texture* getTexture(const T_TEXTURE_ID p_textureId);

   // Get a sound (load it if necessary)
   Mix_Chunk* getSound(const T_SOUND_ID p_soundId);

   private:

   // Texture collection
   SDL_Texture *m_textures[NB_TEXTURES];

   // Sounds effects collection
   Mix_Chunk *m_sounds[NB_SOUNDS];
};

#endif
