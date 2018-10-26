#ifndef _CELL_H_
#define _CELL_H_

#include "def.h"
#include "SDL2/SDL.h"

class CCell
{
   public:

   // Constructor
   CCell(void);

   // Destructor
   virtual ~CCell(void);

   // Get accessor
   const unsigned char getLineState(const T_DIRECTION p_direction) const;

   // Set accessor for one line
   void switchLine(const T_DIRECTION p_direction, const unsigned char p_step);

   // Set accessor for the cell: up/right/down/left = bit1/bit2/bit3/bit4 (rightmost bits)
   void setCellValue(const unsigned char p_val);

   // Get number of lines switched on
   const unsigned int getNbLines(void) const;

   // Clear x's from the cell
   void clearXs(void);

   // Compare two cells
   const bool operator ==(const CCell &p_right) const;

   // Assign a cell to another one
   const CCell &operator =(const CCell &p_source);

   private:

   // The 4 lines around the cell
   unsigned char m_lines[4];

   // Comare the state of 2 lines
   const bool linesEqual(unsigned char p_l1, unsigned char p_l2) const;

   // Forbidden
   CCell(const CCell &p_source);
};

#endif
