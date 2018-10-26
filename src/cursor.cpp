#include <iostream>
#include "cursor.h"
#include "resourceManager.h"
#include "sdlutils.h"

CCursor::CCursor(const unsigned int p_width):
   m_width(p_width),
   m_x(0),
   m_y(0),
   m_alpha(CURSOR_ALPHA_MAX),
   m_alphaStep(-CURSOR_ALPHA_STEP)
{
   // Timers
   m_timer[0] = 0;
   m_timer[1] = 0;
   m_timer[2] = 0;
   m_timer[3] = 0;
}

CCursor::~CCursor(void)
{
}

void CCursor::render(void)
{
   if (m_x >= g_cameraX && m_x < g_cameraX + g_cellsOnScreenX && m_y >= g_cameraY && m_y < g_cameraY + g_cellsOnScreenY)
   {
      SDL_SetRenderDrawColor(g_renderer, CURSOR_COLOR_R, CURSOR_COLOR_G, CURSOR_COLOR_B, m_alpha);
      SDL_Rect rect;
      rect.x = g_offsetX + (m_x - g_cameraX) * g_cellStepX;
      rect.y = g_offsetY + (m_y - g_cameraY) * g_cellStepY;
      rect.w = m_width;
      rect.h = m_width;
      SDL_RenderFillRect(g_renderer, &rect);
   }
}

void CCursor::handle(const SDL_Event &event)
{
   // Move right
   if (BUTTON_PRESSED_RIGHT)
   {
      move(T_DIRECTION_RIGHT, true);
      return;
   }
   // Move left
   if (BUTTON_PRESSED_LEFT)
   {
      move(T_DIRECTION_LEFT, true);
      return;
   }
   // Move up
   if (BUTTON_PRESSED_UP)
   {
      move(T_DIRECTION_UP, true);
      return;
   }
   // Move down
   if (BUTTON_PRESSED_DOWN)
   {
      move(T_DIRECTION_DOWN, true);
      return;
   }
}

const unsigned int CCursor::getX(void) const
{
   return m_x;
}

const unsigned int CCursor::getY(void) const
{
   return m_y;
}

void CCursor::goTo(const unsigned int p_x, const unsigned int p_y)
{
   INHIBIT(std::cout << "CCursor::goTo(" << p_x << ", " << p_y << ")" << std::endl;)
   m_x = p_x;
   m_y = p_y;
   // Camera
   if (g_nbCellsX > g_cellsOnScreenX || g_nbCellsY > g_cellsOnScreenY)
      adjustCamera();
   // Reset transparency
   m_alpha = CURSOR_ALPHA_MAX;
   m_alphaStep = -CURSOR_ALPHA_STEP;
}

void CCursor::reset(void)
{
   INHIBIT(std::cout << "CCursor::reset" << std::endl;)
   m_x = 0;
   m_y = 0;
   m_timer[0] = 0;
   m_timer[1] = 0;
   m_timer[2] = 0;
   m_timer[3] = 0;
   g_cameraX = 0;
   g_cameraY = 0;
   m_alpha = CURSOR_ALPHA_MAX;
   m_alphaStep = -CURSOR_ALPHA_STEP;
}

void CCursor::move(const T_DIRECTION p_direction, const bool p_loop)
{
   bool moved(false);
   switch (p_direction)
   {
      case T_DIRECTION_UP:
         if (m_y == 0)
         {
            if (p_loop)
            {
               m_y = g_nbCellsY - 1;
               moved = true;
            }
         }
         else
         {
            --m_y;
            moved = true;
         }
         break;
      case T_DIRECTION_RIGHT:
         if (m_x >= g_nbCellsX - 1)
         {
            if (p_loop)
            {
               m_x = 0;
               moved = true;
            }
         }
         else
         {
            ++m_x;
            moved = true;
         }
         break;
      case T_DIRECTION_DOWN:
         if (m_y >= g_nbCellsY - 1)
         {
            if (p_loop)
            {
               m_y = 0;
               moved = true;
            }
         }
         else
         {
            ++m_y;
            moved = true;
         }
         break;
      case T_DIRECTION_LEFT:
         if (m_x == 0)
         {
            if (p_loop)
            {
               m_x = g_nbCellsX - 1;
               moved = true;
            }
         }
         else
         {
            --m_x;
            moved = true;
         }
         break;
      default:
         break;
   }
   if (moved)
   {
      // Camera
      if (g_nbCellsX > g_cellsOnScreenX || g_nbCellsY > g_cellsOnScreenY)
         adjustCamera();
      // Reset transparency
      m_alpha = CURSOR_ALPHA_MAX;
      m_alphaStep = -CURSOR_ALPHA_STEP;
      // Sound
      if (g_playSounds) Mix_PlayChannel(-1, g_resourceManager->getSound(T_SOUND_MOVE), 0);
   }
}

void CCursor::adjustCamera(void) const
{
   // Adjust x
   if (m_x < g_cameraX) g_cameraX = m_x;
   else if (m_x >= g_cameraX + g_cellsOnScreenX) g_cameraX = m_x - g_cellsOnScreenX + 1;
   // Adjust Y
   if (m_y < g_cameraY) g_cameraY = m_y;
   else if (m_y >= g_cameraY + g_cellsOnScreenY) g_cameraY = m_y - g_cellsOnScreenY + 1;
   INHIBIT(std::cout << "Cursor [ " << m_x << ", " << m_y << "]  -  Camera [" << g_cameraX << ", " << g_cameraY << "]" << std::endl;)
}

void CCursor::update(void)
{
   // Handle key hold
   bool keyHeld[] = {false, false, false, false};
   keyHeld[T_DIRECTION_UP] = BUTTON_HELD_UP;
   keyHeld[T_DIRECTION_DOWN] = BUTTON_HELD_DOWN;
   keyHeld[T_DIRECTION_RIGHT] = BUTTON_HELD_RIGHT;
   keyHeld[T_DIRECTION_LEFT] = BUTTON_HELD_LEFT;
   for (int direction = 0; direction <= 3; ++direction)
   {
      if (keyHeld[direction])
      {
         if (m_timer[direction])
         {
            // Timer running
            --m_timer[direction];
            if (m_timer[direction] == 0)
            {
               // Trigger!
               move(static_cast<T_DIRECTION>(direction), false);
               // Timer continues
               m_timer[direction] = KEYHOLD_TIMER;
            }
         }
         else
         {
            // Start timer
            m_timer[direction] = KEYHOLD_TIMER_FIRST;
         }
      }
      else
      {
         // Stop timer if necessary
         if (m_timer[direction]) m_timer[direction] = 0;
      }
   }

   // Update transparency
   m_alpha += m_alphaStep;
   if (m_alpha > CURSOR_ALPHA_MAX)
   {
      m_alpha = CURSOR_ALPHA_MAX;
      m_alphaStep = -CURSOR_ALPHA_STEP;
   }
   else if (m_alpha < CURSOR_ALPHA_MIN)
   {
      m_alpha = CURSOR_ALPHA_MIN;
      m_alphaStep = CURSOR_ALPHA_STEP;
   }
}
