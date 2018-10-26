#include "pauseMenu.h"
#include "def.h"
#include "sdlutils.h"

CPauseMenu::CPauseMenu(void):
   m_selection(0)
{
}

CPauseMenu::~CPauseMenu(void)
{
}

void CPauseMenu::handle(const SDL_Event &event)
{
   // Move up
   if (BUTTON_PRESSED_UP)
   {
      m_selection = m_selection ? m_selection - 1 : 2;
      if (g_playSounds) Mix_PlayChannel(-1, g_resourceManager->getSound(T_SOUND_MOVE), 0);
      return;
   }
   // Move down
   if (BUTTON_PRESSED_DOWN)
   {
      m_selection = (m_selection + 1) % 3;
      if (g_playSounds) Mix_PlayChannel(-1, g_resourceManager->getSound(T_SOUND_MOVE), 0);
      return;
   }
}

void CPauseMenu::reset(void)
{
   m_selection = 0;
}

void CPauseMenu::render(void)
{
   switch (g_nbCellsX)
   {
      case  5 :
         SDL_utils::renderTexture(g_resourceManager->getTexture(T_TEXTURE_MENU_PAUSE), 904, 183);
         SDL_utils::renderTexture(g_resourceManager->getTexture(T_TEXTURE_MENU_CURSOR), 927, 279 + 60 * m_selection);
         break;
      case  7 :
         SDL_utils::renderTexture(g_resourceManager->getTexture(T_TEXTURE_MENU_PAUSE), 936, 183);
         SDL_utils::renderTexture(g_resourceManager->getTexture(T_TEXTURE_MENU_CURSOR), 959, 279 + 60 * m_selection);
         break;
      default :
         SDL_utils::renderTexture(g_resourceManager->getTexture(T_TEXTURE_MENU_PAUSE), 981, 183);
         SDL_utils::renderTexture(g_resourceManager->getTexture(T_TEXTURE_MENU_CURSOR), 1004, 279 + 60 * m_selection);
         break;
   }
}

const unsigned int CPauseMenu::getSelection(void) const
{
   return m_selection;
}
