#include "levelSelector.h"
#include "def.h"
#include "sdlutils.h"

CLevelSelector::CLevelSelector(CSaveManager *p_saveManager):
   m_cursor(40),
   m_saveManager(p_saveManager)
{
}

CLevelSelector::~CLevelSelector(void)
{
}

void CLevelSelector::handle(const SDL_Event &p_event)
{
   // Move cursor
   m_cursor.handle(p_event);
 }

void CLevelSelector::update(void)
{
   m_cursor.update();
}

void CLevelSelector::reset(void)
{
   m_cursor.reset();
}

void CLevelSelector::render(void)
{
   SDL_RenderCopy(g_renderer, g_resourceManager->getTexture(T_TEXTURE_BACKGROUND), NULL, NULL);
   SDL_RenderCopy(g_renderer, g_resourceManager->getTexture(T_TEXTURE_FRAME), NULL, NULL);
   SDL_RenderCopy(g_renderer, g_resourceManager->getTexture(T_TEXTURE_LEVEL_SELECT), NULL, NULL);
   m_cursor.render();
   // Display arrows
   if (g_cameraY > 0)
      SDL_utils::renderTexture(g_resourceManager->getTexture(T_TEXTURE_ARROW_UP), 610, 71);
   if (g_cameraY < 50)
      SDL_utils::renderTexture(g_resourceManager->getTexture(T_TEXTURE_ARROW_DOWN), 610, 619);
   // Display level numbers and ticks
   unsigned int firstNumber = g_cameraY * g_cellsOnScreenX + 1;
   unsigned int lastNumber = firstNumber + g_cellsOnScreenX * g_cellsOnScreenY - 1;
   unsigned int limitX = g_offsetX + (g_cellsOnScreenX - 1) * g_cellStepX;
   unsigned int x = g_offsetX;
   unsigned int y = g_offsetY;
   bool solved(false);
   for (unsigned int nb = firstNumber; nb <= lastNumber; ++nb)
   {
      solved = m_saveManager->getLevelIsSolved(nb);
      SDL_utils::renderNumber(nb, x + 48, y + 6, 0, false);
      if (solved)
         SDL_utils::renderTexture(g_resourceManager->getTexture(T_TEXTURE_CROSS2), x + 5, y + 5);
      x += g_cellStepX;
      if (x > limitX)
      {
         x = g_offsetX;
         y += g_cellStepY;
      }
   }
}

void CLevelSelector::setLevelNumber(const unsigned int p_number)
{
   unsigned int number = p_number - 1;
   m_cursor.goTo(number % g_cellsOnScreenX, number / g_cellsOnScreenX);
}

const unsigned int CLevelSelector::getLevelNumber(void) const
{
   return m_cursor.getX() + m_cursor.getY() * g_nbCellsX + 1;
}

CCursor *CLevelSelector::getCursor()
{
   return &m_cursor;
}
