#ifndef _PAUSEMENU_H_
#define _PAUSEMENU_H_

#include <SDL2/SDL.h>

class CPauseMenu
{
   public:

   // Constructor
   CPauseMenu(void);

   // Destructor
   virtual ~CPauseMenu(void);

   // Handle key inputs
   void handle(const SDL_Event &event);

   // Display on screen
   void render(void);

   // Reset level
   void reset(void);

   // Get index of selected item
   const unsigned int getSelection(void) const;

   private:

   // The cursor
   unsigned int m_selection;
};

#endif
