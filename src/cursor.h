#ifndef _CURSOR_H_
#define _CURSOR_H_

#include "def.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_mixer.h"

#define CURSOR_COLOR_R 171
#define CURSOR_COLOR_G 143
#define CURSOR_COLOR_B 125
#define CURSOR_ALPHA_MAX 190
#define CURSOR_ALPHA_MIN 120
#define CURSOR_ALPHA_STEP 2
#define KEYHOLD_TIMER_FIRST 15
#define KEYHOLD_TIMER 4

class CCursor
{
   public:

   // Constructor
   CCursor(const unsigned int p_width);

   // Destructor
   virtual ~CCursor(void);

   // Handle key inputs
   void handle(const SDL_Event &event);

   // Update cursor
   void update(void);

   // Display cursor on screen
   void render(void);

   // Reset cursor
   void reset(void);

   // Get coordinates
   const unsigned int getX(void) const;
   const unsigned int getY(void) const;

   // Set position
   void goTo(const unsigned int p_x, const unsigned int p_y);

   // Adjust the camera
   void adjustCamera(void) const;

   private:

   // Forbidden
   CCursor(void);

   // Move the cursor 1 cell
   void move(const T_DIRECTION p_direction, const bool p_loop);

   // Cursor width
   unsigned int m_width;

   // Coordinates in the matrix
   unsigned int m_x;
   unsigned int m_y;

   // Timers for key hold
   unsigned int m_timer[4];

   // Alpha
   Uint8 m_alpha;
   Sint8 m_alphaStep;
};

#endif
