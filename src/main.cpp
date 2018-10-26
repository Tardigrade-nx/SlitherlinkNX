#include <iostream>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include "def.h"
#include "resourceManager.h"
#include "levelSelector.h"
#include "level.h"
#include "draw.h"
#include "pauseMenu.h"
#include "saveManager.h"
#include "sdlutils.h"
#ifdef __SWITCH__
#include "switch.h"
#endif

//------------------------------------------------------------------------------

// Globals
SDL_Window* g_window = NULL;
SDL_Renderer* g_renderer = NULL;
SDL_Joystick* g_joystick = NULL;
CResourceManager* g_resourceManager = NULL;
unsigned int g_nbCellsX = 0;
unsigned int g_nbCellsY = 0;
unsigned int g_cellStepX = 0;
unsigned int g_cellStepY = 0;
unsigned int g_cellsOnScreenX = 0;
unsigned int g_cellsOnScreenY = 0;
unsigned int g_offsetX = 0;
unsigned int g_offsetY = 0;
unsigned int g_cameraX = 0;
unsigned int g_cameraY = 0;
bool g_playSounds = true;

//------------------------------------------------------------------------------

typedef enum
{
    T_GAME_STATE_TITLE = 0,
    T_GAME_STATE_LEVEL_SELECT,
    T_GAME_STATE_PLAY,
    T_GAME_STATE_PAUSE,
    T_GAME_STATE_SOLVED,
    T_GAME_STATE_QUIT
}
T_GAME_STATE;

//------------------------------------------------------------------------------

// Game objects
T_GAME_STATE gameState(T_GAME_STATE_TITLE);
CSaveManager saveManager;
CLevelSelector levelSelector(&saveManager);
CLevel level;
CPauseMenu pauseMenu;

//------------------------------------------------------------------------------

void cleanup()
{
   INHIBIT(std::cout << "cleanup()" << std::endl;)
   // Renderer
   if (g_renderer != NULL)
   {
      SDL_DestroyRenderer(g_renderer);
      g_renderer = NULL;
   }
   // Window
   if (g_window != NULL)
   {
      SDL_DestroyWindow(g_window);
      g_window = NULL;
   }
   // Joystick
   if (g_joystick != NULL)
   {
      SDL_JoystickClose(g_joystick);
      g_joystick = NULL;
   }
   // SDL subsystems
   Mix_CloseAudio();
   Mix_Quit();
   IMG_Quit();
   SDL_Quit();
   // Resource manager
   if (g_resourceManager != NULL)
   {
      delete g_resourceManager;
      g_resourceManager = NULL;
   }
   // romfs
   #ifdef __SWITCH__
   romfsExit();
   #endif
}

//------------------------------------------------------------------------------

void goToState(const T_GAME_STATE p_newState, const T_SOUND_ID p_sound)
{
   switch (p_newState)
   {
      // Go to level selector
      case T_GAME_STATE_LEVEL_SELECT:
         g_nbCellsX = 8;
         g_nbCellsY = 57;
         g_cellStepX = 131;
         g_cellStepY = 75;
         g_cellsOnScreenX = 8;
         g_cellsOnScreenY = 7;
         g_offsetX = 128;
         g_offsetY = 115;
         if (gameState == T_GAME_STATE_TITLE)
            levelSelector.setLevelNumber(saveManager.getFirstUnsolvedLevel());
         levelSelector.getCursor()->adjustCamera();
         break;
      // Go to play
      case T_GAME_STATE_PLAY:
         g_cellStepX = 63;
         g_cellStepY = 63;
         g_cellsOnScreenX = 10;
         g_cellsOnScreenY = 10;
         break;
      // Go to game solved
      case T_GAME_STATE_SOLVED:
         level.getDrawing()->clearXs();
         break;
      // Go to pause menu
      case T_GAME_STATE_PAUSE:
         pauseMenu.reset();
         break;
      default:
         break;
   }
   // Play sound if requested
   if (p_sound != -1 && g_playSounds)
      Mix_PlayChannel(-1, g_resourceManager->getSound(p_sound), 0);
   // Change game state
   gameState = p_newState;
}

//------------------------------------------------------------------------------

int main(int argc, char* args[])
{
   // Initialize romfs
   #ifdef __SWITCH__
   romfsInit();
   #endif
   
   // Initialize SDL
   if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_JOYSTICK) < 0)
   {
      std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
      cleanup();
      return 1;
   }
   INHIBIT(std::cout << "SDL_Init OK" << std::endl;)

   // Initialize SDL_image
   int flags = IMG_INIT_PNG;
   if ((IMG_Init(flags) & flags) != flags)
   {
      std::cerr << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << std::endl;
      cleanup();
      return 1;
   }
   INHIBIT(std::cout << "IMG_Init OK" << std::endl;)

   // Initialize SDL_Mixer
   if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 512) < 0)
   {
      std::cerr << "Mix_OpenAudio failed: " << Mix_GetError() << std::endl;
      cleanup();
      return 1;
   }
   INHIBIT(std::cout << "Mix_OpenAudio OK" << std::endl;)

   // Initialize joystick
   INHIBIT(std::cout << "SDL_NumJoysticks: '" << SDL_NumJoysticks() << "'" << std::endl;)
   if (SDL_NumJoysticks() >= 1)
   {
      g_joystick = SDL_JoystickOpen(0);
      if (g_joystick == NULL)
      {
         std::cerr << "Unable to open joystick." << std::endl;
         cleanup();
         return 1;
      }
      INHIBIT(std::cout << "SDL_JoystickOpen OK" << std::endl;)
   }

   // Create window
   #ifdef __SWITCH__
   g_window = SDL_CreateWindow(APP_NAME, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_FULLSCREEN);
   #else
   g_window = SDL_CreateWindow(APP_NAME, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
   #endif
   if (g_window == NULL)
   {
      std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
      cleanup();
      return 1;
   }
   INHIBIT(std::cout << "SDL_CreateWindow OK" << std::endl;)

   // Create renderer
   g_renderer = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
   if (g_renderer == NULL)
   {
      std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
      cleanup();
      return 1;
   }
   INHIBIT(std::cout << "SDL_CreateRenderer OK" << std::endl;)

   // Set renderer's alpha mode
   SDL_SetRenderDrawBlendMode(g_renderer, SDL_BLENDMODE_BLEND);

   // Create resource manager
   g_resourceManager = new CResourceManager();

   // Main loop
   SDL_Event event;
   while (gameState != T_GAME_STATE_QUIT)
   {
      switch (gameState)
      {
//------------------------------------------------------------------------------
         case T_GAME_STATE_TITLE:
            // Events
            while (SDL_PollEvent(&event) != 0)
            {
               // Quit game
               if ((event.type == SDL_QUIT) || (BUTTON_PRESSED_BACK))
               {
                  goToState(T_GAME_STATE_QUIT, T_SOUND_NOSOUND);
               }
               // Go to level selector or saved level
               else if (BUTTON_PRESSED_VALIDATE)
               {
                  unsigned int savedLevelNumber = 0;
                  std::string savedDrawing = "";
                  saveManager.getSaveState(savedLevelNumber, savedDrawing);
                  if (savedLevelNumber)
                  {
                     // Go to saved level
                     goToState(T_GAME_STATE_PLAY, T_SOUND_START);
                     level.reset();
                     level.loadLevel(savedLevelNumber, savedDrawing);
                     saveManager.clearSaveState();
                  }
                  else
                  {
                     // Go to level selector
                     goToState(T_GAME_STATE_LEVEL_SELECT, T_SOUND_START);
                  }
               }
            }
            // Render
            if (gameState == T_GAME_STATE_TITLE)
            {
               SDL_RenderClear(g_renderer);
               SDL_RenderCopy(g_renderer, g_resourceManager->getTexture(T_TEXTURE_BACKGROUND), NULL, NULL);
               SDL_RenderCopy(g_renderer, g_resourceManager->getTexture(T_TEXTURE_FRAME), NULL, NULL);
               SDL_RenderCopy(g_renderer, g_resourceManager->getTexture(T_TEXTURE_TITLE), NULL, NULL);
               SDL_RenderPresent(g_renderer);
            }
            break;
//------------------------------------------------------------------------------
         case T_GAME_STATE_PLAY:
            // Events
            while (SDL_PollEvent(&event) != 0)
            {
               // Quit game
               if (event.type == SDL_QUIT)
               {
                  goToState(T_GAME_STATE_QUIT, T_SOUND_NOSOUND);
               }
               // Go to pause menu
               else if (BUTTON_PRESSED_MENU)
               {
                  goToState(T_GAME_STATE_PAUSE, T_SOUND_SOLVED);
               }
               // Pass event to level
               else
               {
                  if (level.handle(event))
                  {
                     // Check wether the level is solved
                     if (level.solutionFound())
                     {
                        saveManager.setLevelIsSolved(level.getLevelNumber());
                        goToState(T_GAME_STATE_SOLVED, T_SOUND_SOLVED);
                     }
                  }
               }
            }
            // Update & Render
            if (gameState == T_GAME_STATE_PLAY)
            {
               level.update();
               SDL_RenderClear(g_renderer);
               level.render(true);
               SDL_RenderPresent(g_renderer);
            }
            break;
//------------------------------------------------------------------------------
         case T_GAME_STATE_LEVEL_SELECT:
            while (SDL_PollEvent(&event) != 0)
            {
               // Quit game
               if (event.type == SDL_QUIT)
               {
                  goToState(T_GAME_STATE_QUIT, T_SOUND_NOSOUND);
               }
               // Go to title
               else if (BUTTON_PRESSED_BACK)
               {
                  goToState(T_GAME_STATE_TITLE, T_SOUND_BACK);
               }
               // Go to level
               else if (BUTTON_PRESSED_VALIDATE)
               {
                  goToState(T_GAME_STATE_PLAY, T_SOUND_START);
                  level.reset();
                  level.loadLevel(levelSelector.getLevelNumber(), "");
               }
               // Pass event to level selector
               else
               {
                  levelSelector.handle(event);
               }
            }
            // Update & Render
            if (gameState == T_GAME_STATE_LEVEL_SELECT)
            {
               levelSelector.update();
               SDL_RenderClear(g_renderer);
               levelSelector.render();
               SDL_RenderPresent(g_renderer);
            }
            break;
//------------------------------------------------------------------------------
         case T_GAME_STATE_SOLVED:
            // Events
            while (SDL_PollEvent(&event) != 0)
            {
               // Quit game
               if (event.type == SDL_QUIT)
               {
                  goToState(T_GAME_STATE_QUIT, T_SOUND_NOSOUND);
               }
               // Go to level selector
               else if ((BUTTON_PRESSED_VALIDATE) || (BUTTON_PRESSED_BACK))
               {
                  goToState(T_GAME_STATE_LEVEL_SELECT, T_SOUND_UNDO);
               }
            }
            // Render
            if (gameState == T_GAME_STATE_SOLVED)
            {
               SDL_RenderClear(g_renderer);
               level.render(false);
               SDL_utils::renderTexture(g_resourceManager->getTexture(T_TEXTURE_SOLVED), 125, 320);
               SDL_RenderPresent(g_renderer);
            }
            break;
//------------------------------------------------------------------------------
         case T_GAME_STATE_PAUSE:
            // Events
            while (SDL_PollEvent(&event) != 0)
            {
               // Quit game
               if (event.type == SDL_QUIT)
               {
                  goToState(T_GAME_STATE_QUIT, T_SOUND_NOSOUND);
               }
               // Go back to gameplay
               else if (BUTTON_PRESSED_BACK)
               {
                  gameState = T_GAME_STATE_PLAY;
                  if (g_playSounds) Mix_PlayChannel(-1, g_resourceManager->getSound(T_SOUND_SOLVED), 0);
               }
               // Validate menu item
               else if (BUTTON_PRESSED_VALIDATE)
               {
                  switch (pauseMenu.getSelection())
                  {
                     // Retry
                     case 0 :
                        level.getDrawing()->clear();
                        if (g_playSounds) Mix_PlayChannel(-1, g_resourceManager->getSound(T_SOUND_START), 0);
                        gameState = T_GAME_STATE_PLAY;
                        break;
                     // Save
                     case 1 :
                        saveManager.saveState(&level);
                        goToState(T_GAME_STATE_TITLE, T_SOUND_BACK);
                        break;
                     // Quit
                     case 2 :
                        goToState(T_GAME_STATE_LEVEL_SELECT, T_SOUND_BACK);
                        break;
                     default:
                        break;
                  }
               }
               // Pass event to pause menu
               else
               {
                  pauseMenu.handle(event);
               }
            }
            // Render
            if (gameState == T_GAME_STATE_PAUSE)
            {
               SDL_RenderClear(g_renderer);
               level.render(false);
               pauseMenu.render();
               SDL_RenderPresent(g_renderer);
            }
            break;
//------------------------------------------------------------------------------
         default:
            break;
      }
   }

   // Clean up and quit
   cleanup();
   return 0;
}
