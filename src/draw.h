#ifndef _DRAW_H_
#define _DRAW_H_

#include <string>
#include <deque>
#include "def.h"
#include "cell.h"
#include "cursor.h"

#define MAX_UNDO_ACTIONS 128

// Used for 'undo'
class CAction
{
   public:
   CAction(void) : m_x(0), m_y(0), m_dir(T_DIRECTION_UP), m_s(1) {}
   CAction(const unsigned int p_x, const unsigned int p_y, const T_DIRECTION p_dir, const unsigned int p_s) : m_x(p_x), m_y(p_y), m_dir(p_dir), m_s(p_s) {}
   CAction(const CAction &p_src) : m_x(p_src.m_x), m_y(p_src.m_y), m_dir(p_src.m_dir), m_s(p_src.m_s) {}
   const CAction &operator =(const CAction &p_src) { m_x = p_src.m_x; m_y = p_src.m_y; m_dir = p_src.m_dir; m_s = p_src.m_s; return *this; }
   virtual ~CAction(void) {}
   unsigned int m_x;
   unsigned int m_y;
   T_DIRECTION m_dir;
   unsigned int m_s;
};

// Class representing a slitherlink drawing with lines and crosses
class CDraw
{
   public:

   // Constructor
   CDraw(void);

   // Destructor
   virtual ~CDraw(void);

   // Handle key inputs
   // Returns true if the drawing has changed
   const bool handle(const SDL_Event &event);

   // Update
   void update(void);

   // Display drawn elements on the given surface
   void render(const bool p_renderCursor);

   // Reset object
   void reset(void);

   // Clear the drawing
   void clear(void);

   // Clear all x's from the cells
   void clearXs(void);

   // Allocate cells and sets current drawing
   void loadLevel(const std::string &p_drawing);

   // Get the cell array
   const CCell * const getCell(void) const;

   private:

   // Switch a line
   void switchLine(const T_DIRECTION p_direction, const unsigned char p_step, const bool p_recordAction);

   // Undo last action
   void undo(void);

   // The cells
   CCell *m_cells;

   // The cursor
   CCursor m_cursor;

   // List of past actions, for "undo"
   std::deque<CAction> m_actions;
};

#endif
