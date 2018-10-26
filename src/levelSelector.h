#ifndef _LEVELSELECTOR_H_
#define _LEVELSELECTOR_H_

#include <SDL2/SDL.h>
#include "cursor.h"
#include "saveManager.h"

class CLevelSelector
{
   public:

   // Constructor
   CLevelSelector(CSaveManager *p_saveManager);

   // Destructor
   virtual ~CLevelSelector(void);

   // Handle key inputs
   void handle(const SDL_Event &p_event);

   // Update
   void update(void);

   // Display on screen
   void render(void);

   // Reset level
   void reset(void);

   // Set selected level number
   void setLevelNumber(const unsigned int p_number);

   // Get selected level number
   const unsigned int getLevelNumber(void) const;

   // Get the cursor
   CCursor *getCursor();

   private:

   // The cursor
   CCursor m_cursor;

   // Pointer to the Save Manager
   CSaveManager *m_saveManager;
};

#endif
