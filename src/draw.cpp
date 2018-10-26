#include <iostream>
#include <sstream>
#include "SDL2/SDL.h"
#include "draw.h"
#include "resourceManager.h"
#include "sdlutils.h"

CDraw::CDraw(void) :
   m_cells(NULL),
   m_cursor(60)
{
}

CDraw::~CDraw(void)
{
   // Delete cells
   if (m_cells != NULL)
   {
      delete [] m_cells;
      m_cells = NULL;
   }
}

const bool CDraw::handle(const SDL_Event &event)
{
   unsigned char step(1);
   if (BUTTON_HELD_MOD)
      step = 2;
   // Move cursor
   m_cursor.handle(event);
   // Draw lines
   if (BUTTON_PRESSED_LINE_UP)
   {
      switchLine(T_DIRECTION_UP, step, true);
      return true;
   }
   if (BUTTON_PRESSED_LINE_DOWN)
   {
      switchLine(T_DIRECTION_DOWN, step, true);
      return true;
   }
   if (BUTTON_PRESSED_LINE_LEFT)
   {
      switchLine(T_DIRECTION_LEFT, step, true);
      return true;
   }
   if (BUTTON_PRESSED_LINE_RIGHT)
   {
      switchLine(T_DIRECTION_RIGHT, step, true);
      return true;
   }
   if (BUTTON_PRESSED_UNDO)
   {
      undo();
      return true;
   }
   return false;
}

void CDraw::switchLine(const T_DIRECTION p_direction, const unsigned char p_step, const bool p_recordAction)
{
   switch (p_direction)
   {
      case T_DIRECTION_UP:
         m_cells[m_cursor.getX() * g_nbCellsX + m_cursor.getY()].switchLine(T_DIRECTION_UP, p_step);
         if (m_cursor.getY() > 0) m_cells[m_cursor.getX() * g_nbCellsX + m_cursor.getY() - 1].switchLine(T_DIRECTION_DOWN, p_step);
         break;
      case T_DIRECTION_DOWN:
         m_cells[m_cursor.getX() * g_nbCellsX + m_cursor.getY()].switchLine(T_DIRECTION_DOWN, p_step);
         if (m_cursor.getY() < g_nbCellsY - 1) m_cells[m_cursor.getX() * g_nbCellsX + m_cursor.getY() + 1].switchLine(T_DIRECTION_UP, p_step);
         break;
     case T_DIRECTION_RIGHT:
         m_cells[m_cursor.getX() * g_nbCellsX + m_cursor.getY()].switchLine(T_DIRECTION_RIGHT, p_step);
         if (m_cursor.getX() < g_nbCellsX - 1) m_cells[(m_cursor.getX() + 1) * g_nbCellsX + m_cursor.getY()].switchLine(T_DIRECTION_LEFT, p_step);
         break;
     case T_DIRECTION_LEFT:
         m_cells[m_cursor.getX() * g_nbCellsX + m_cursor.getY()].switchLine(T_DIRECTION_LEFT, p_step);
         if (m_cursor.getX() > 0) m_cells[(m_cursor.getX() - 1) * g_nbCellsX + m_cursor.getY()].switchLine(T_DIRECTION_RIGHT, p_step);
         break;
     default:
         break;
   }
   if (p_recordAction)
   {
      // Add action to the history
      if (m_actions.size() == MAX_UNDO_ACTIONS)
         m_actions.pop_front();
      m_actions.push_back(CAction(m_cursor.getX(), m_cursor.getY(), p_direction, p_step));
      if (g_playSounds) Mix_PlayChannel(-1, g_resourceManager->getSound(T_SOUND_DRAW), 0);
   }
   else
   {
      if (g_playSounds) Mix_PlayChannel(-1, g_resourceManager->getSound(T_SOUND_UNDO), 0);
   }
}

void CDraw::undo(void)
{
   // Undo last action
   if (m_actions.size() > 0)
   {
      m_cursor.goTo(m_actions.back().m_x, m_actions.back().m_y);
      switchLine(m_actions.back().m_dir, 3 - m_actions.back().m_s, false);
      m_actions.pop_back();
   }
}

void CDraw::update(void)
{
   m_cursor.update();
}

void CDraw::reset(void)
{
   INHIBIT(std::cout << "CDraw::reset" << std::endl;)
   // Reset cursor
   m_cursor.reset();
   // Free cells
   if (m_cells != NULL)
   {
     delete [] m_cells;
     m_cells = NULL;
   }
   // Reset action history
   m_actions.clear();
}

const CCell * const CDraw::getCell(void) const
{
   return m_cells;
}

void CDraw::render(const bool p_renderCursor)
{
   if (m_cells == NULL)
   {
      std::cerr << "CDraw::render: no cells created!" << std::endl;
      return;
   }
   // Draw cursor if asked
   if (p_renderCursor)
      m_cursor.render();
   // Draw lines
   unsigned int l_i(0);
   unsigned int l_j(0);
   unsigned int l_limitX = g_cameraX + g_cellsOnScreenX - 1;
   if (l_limitX + 1 > g_nbCellsX)
      l_limitX = g_nbCellsX - 1;
   unsigned int l_limitY = g_cameraY + g_cellsOnScreenY - 1;
   if (l_limitY + 1 > g_nbCellsY)
      l_limitY = g_nbCellsY - 1;
   for (l_i = g_cameraX; l_i <= l_limitX; ++l_i)
   {
      for (l_j = g_cameraY; l_j <= l_limitY; ++l_j)
      {
         switch (m_cells[l_i * g_nbCellsX + l_j].getLineState(T_DIRECTION_UP))
         {
            case 1: SDL_utils::renderTexture(g_resourceManager->getTexture(T_TEXTURE_LINEH), g_offsetX + (l_i - g_cameraX) * g_cellStepX - 6, g_offsetY + (l_j - g_cameraY) * g_cellStepY - 6); break;
            case 2: SDL_utils::renderTexture(g_resourceManager->getTexture(T_TEXTURE_CROSS), g_offsetX + (l_i - g_cameraX) * g_cellStepX + 17, g_offsetY + (l_j - g_cameraY) * g_cellStepY - 15); break;
            default : break;
         }
         switch (m_cells[l_i * g_nbCellsX + l_j].getLineState(T_DIRECTION_LEFT))
         {
            case 1: SDL_utils::renderTexture(g_resourceManager->getTexture(T_TEXTURE_LINEV), g_offsetX + (l_i - g_cameraX) * g_cellStepX - 6, g_offsetY + (l_j - g_cameraY) * g_cellStepY - 6); break;
            case 2: SDL_utils::renderTexture(g_resourceManager->getTexture(T_TEXTURE_CROSS), g_offsetX + (l_i - g_cameraX) * g_cellStepX - 15, g_offsetY + (l_j - g_cameraY) * g_cellStepY + 17); break;
            default: break;
         }
         // Draw right line only for the right-most column
         if (l_i == l_limitX)
         {
            switch (m_cells[l_i * g_nbCellsX + l_j].getLineState(T_DIRECTION_RIGHT))
            {
               case 1: SDL_utils::renderTexture(g_resourceManager->getTexture(T_TEXTURE_LINEV), g_offsetX + (l_i - g_cameraX) * g_cellStepX + 57, g_offsetY + (l_j - g_cameraY) * g_cellStepY - 6); break;
               case 2: SDL_utils::renderTexture(g_resourceManager->getTexture(T_TEXTURE_CROSS), g_offsetX + (l_i - g_cameraX) * g_cellStepX + 48, g_offsetY + (l_j - g_cameraY) * g_cellStepY + 17); break;
               default: break;
            }
         }
         // Draw bottom line only for the bottom row
         if (l_j == l_limitY)
         {
            switch (m_cells[l_i * g_nbCellsX + l_j].getLineState(T_DIRECTION_DOWN))
            {
               case 1: SDL_utils::renderTexture(g_resourceManager->getTexture(T_TEXTURE_LINEH), g_offsetX + (l_i - g_cameraX) * g_cellStepX - 6, g_offsetY + (l_j - g_cameraY) * g_cellStepY + 57); break;
               case 2: SDL_utils::renderTexture(g_resourceManager->getTexture(T_TEXTURE_CROSS), g_offsetX + (l_i - g_cameraX) * g_cellStepX + 17, g_offsetY + (l_j - g_cameraY) * g_cellStepY + 48); break;
               default: break;
            }
         }
      }
   }
}

void CDraw::clear(void)
{
   // Clear cells
   if (m_cells != NULL)
   {
      const int l_nbTotal(g_nbCellsX * g_nbCellsY);
      for (int l_i = 0; l_i < l_nbTotal; ++l_i)
         m_cells[l_i].setCellValue(0);
   }
   // Clear action history
   m_actions.clear();
}

void CDraw::clearXs(void)
{
   if (m_cells != NULL)
   {
      const int l_nbTotal(g_nbCellsX * g_nbCellsY);
      for (int l_i = 0; l_i < l_nbTotal; ++l_i)
         m_cells[l_i].clearXs();
   }
}

void CDraw::loadLevel(const std::string &p_drawing)
{
   INHIBIT(std::cout << "CDraw::loadLevel(" << p_drawing << ")" << std::endl;)
   // Allocate cells
   const int l_nbTotal(g_nbCellsX * g_nbCellsY);
   if (m_cells != NULL)
   {
      delete [] m_cells;
      m_cells = NULL;
   }
   m_cells = new CCell[l_nbTotal];
   // Sets current drawing if necessary
   if (!p_drawing.empty())
   {
      // Decode level
      int l_val(0);
      int l_index(0);
      for (int l_i = 0; l_i < l_nbTotal; ++l_i)
      {
         std::istringstream l_stream(p_drawing.substr(l_index, 2));
         l_stream >> std::hex;
         l_stream >> l_val;
         m_cells[l_i].setCellValue(l_val);
         l_index += 2;
     }
   }
}
