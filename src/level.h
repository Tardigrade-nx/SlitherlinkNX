#ifndef _LEVEL_H_
#define _LEVEL_H_

#include <vector>
#include <SDL2/SDL.h>
#include "cell.h"
#include "draw.h"

#define LEVELDATA_FILE "levels.dat"

class CPoint
{
   public:
   CPoint(void) : m_x(0), m_y(0) {}
   CPoint(const unsigned int p_x, const unsigned int p_y) : m_x(p_x), m_y(p_y) {}
   CPoint(const CPoint &p_source) : m_x(p_source.m_x), m_y(p_source.m_y) {}
   const CPoint &operator =(const CPoint &p_source) { m_x = p_source.m_x; m_y = p_source.m_y; return *this;}
   virtual ~CPoint(void) {}
   unsigned int m_x;
   unsigned int m_y;
};

class CLevel
{
   public:

   // Constructor
   CLevel(void);

   // Destructor
   virtual ~CLevel(void);

   // Handle key inputs
   // Returns true if the drawing has changed
   const bool handle(const SDL_Event &p_event);

   // Update
   void update(void);

   // Display on screen
   void render(const bool p_renderCursor);

   // Reset level
   void reset(void);

   // Load a level
   void loadLevel(const unsigned int p_number, const std::string &p_drawing);

   // Check if the given drawing is the answer
   const bool solutionFound(void);

   // Get level number
   const unsigned int getLevelNumber(void) const;

   // Get CDraw object
   CDraw* getDrawing(void);

   private:

   // Current level number
   unsigned int m_levelNumber;

   // The drawing
   CDraw m_draw;

   // Grid image
   SDL_Texture *m_imageGrid;

   // Solution of the current level
   CCell *m_cells;

   // Number hints of the current levels
   std::vector<CPoint> m_hints;
};

#endif
