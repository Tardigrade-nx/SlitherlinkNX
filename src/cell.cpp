#include <iostream>
#include "cell.h"

CCell::CCell(void)
{
   m_lines[T_DIRECTION_UP] = 0;
   m_lines[T_DIRECTION_DOWN] = 0;
   m_lines[T_DIRECTION_RIGHT] = 0;
   m_lines[T_DIRECTION_LEFT] = 0;
}

CCell::~CCell(void)
{
}

const unsigned char CCell::getLineState(const T_DIRECTION p_direction) const
{
   return m_lines[p_direction];
}

void CCell::switchLine(const T_DIRECTION p_direction, const unsigned char p_step)
{
   m_lines[p_direction] = (m_lines[p_direction] + p_step) % 3;
}

const unsigned int CCell::getNbLines(void) const
{
   unsigned int l_retVal(0);
   if (m_lines[T_DIRECTION_UP] == 1) ++l_retVal;
   if (m_lines[T_DIRECTION_DOWN] == 1) ++l_retVal;
   if (m_lines[T_DIRECTION_RIGHT] == 1) ++l_retVal;
   if (m_lines[T_DIRECTION_LEFT] == 1) ++l_retVal;
   return l_retVal;
}

void CCell::setCellValue(const unsigned char p_val)
{
   if ((p_val >> 7) & 1)      m_lines[T_DIRECTION_UP] = 2;
   else if ((p_val >> 3) & 1) m_lines[T_DIRECTION_UP] = 1;
   else                       m_lines[T_DIRECTION_UP] = 0;
   if ((p_val >> 6) & 1)      m_lines[T_DIRECTION_RIGHT] = 2;
   else if ((p_val >> 2) & 1) m_lines[T_DIRECTION_RIGHT] = 1;
   else                       m_lines[T_DIRECTION_RIGHT] = 0;
   if ((p_val >> 5) & 1)      m_lines[T_DIRECTION_DOWN] = 2;
   else if ((p_val >> 1) & 1) m_lines[T_DIRECTION_DOWN] = 1;
   else                       m_lines[T_DIRECTION_DOWN] = 0;
   if ((p_val >> 4) & 1)      m_lines[T_DIRECTION_LEFT] = 2;
   else if (p_val & 1)        m_lines[T_DIRECTION_LEFT] = 1;
   else                       m_lines[T_DIRECTION_LEFT] = 0;
}

const bool CCell::operator ==(const CCell &p_right) const
{
   return linesEqual(m_lines[T_DIRECTION_UP], p_right.getLineState(T_DIRECTION_UP)) &&
          linesEqual(m_lines[T_DIRECTION_RIGHT], p_right.getLineState(T_DIRECTION_RIGHT)) &&
          linesEqual(m_lines[T_DIRECTION_DOWN], p_right.getLineState(T_DIRECTION_DOWN)) &&
          linesEqual(m_lines[T_DIRECTION_LEFT], p_right.getLineState(T_DIRECTION_LEFT));
}

const bool CCell::linesEqual(unsigned char p_l1, unsigned char p_l2) const
{
   // State 2 is the same as state 0 for this comparison
   if (p_l1 == 2) p_l1 = 0;
   if (p_l2 == 2) p_l2 = 0;
   return (p_l1 == p_l2);
}

const CCell &CCell::operator =(const CCell &p_source)
{
   m_lines[T_DIRECTION_UP] = p_source.getLineState(T_DIRECTION_UP);
   m_lines[T_DIRECTION_DOWN] = p_source.getLineState(T_DIRECTION_DOWN);
   m_lines[T_DIRECTION_LEFT] = p_source.getLineState(T_DIRECTION_LEFT);
   m_lines[T_DIRECTION_RIGHT] = p_source.getLineState(T_DIRECTION_RIGHT);
   return *this;
}

void CCell::clearXs(void)
{
   if (m_lines[T_DIRECTION_UP] == 2)
      m_lines[T_DIRECTION_UP] = 0;
   if (m_lines[T_DIRECTION_RIGHT] == 2)
      m_lines[T_DIRECTION_RIGHT] = 0;
   if (m_lines[T_DIRECTION_DOWN] == 2)
      m_lines[T_DIRECTION_DOWN] = 0;
   if (m_lines[T_DIRECTION_LEFT] == 2)
      m_lines[T_DIRECTION_LEFT] = 0;
}
