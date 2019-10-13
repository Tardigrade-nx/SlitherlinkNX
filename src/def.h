#ifndef _DEF_H_
#define _DEF_H_

#include <SDL2/SDL.h>
#include "resourceManager.h"

//~ #define INHIBIT(X) X
#define INHIBIT(X) /* X */

#define APP_NAME "SlitherlinkNX"
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#ifdef __SWITCH__
#define PATH_PREFIX "romfs:/"
#else
#define PATH_PREFIX "res/"
#endif

// Directions
typedef enum
{
   T_DIRECTION_UP = 0,
   T_DIRECTION_DOWN,
   T_DIRECTION_RIGHT,
   T_DIRECTION_LEFT
}
T_DIRECTION;

// Globals
extern SDL_Window* g_window;
extern SDL_Renderer* g_renderer;
extern SDL_Joystick* g_joystick;
extern CResourceManager* g_resourceManager;
// Number of cells in the grid
extern unsigned int g_nbCellsX;
extern unsigned int g_nbCellsY;
// Step used to move from cell to cell
extern unsigned int g_cellStepX;
extern unsigned int g_cellStepY;
// Number of cells displayed on screen at the same time
extern unsigned int g_cellsOnScreenX;
extern unsigned int g_cellsOnScreenY;
// Offset of the grid
extern unsigned int g_offsetX;
extern unsigned int g_offsetY;
// "Camera" coordinates, for grids larger than the screen
extern unsigned int g_cameraX;
extern unsigned int g_cameraY;
// Play sounds or not
extern bool g_playSounds;

#ifdef __SWITCH__
// Button events for Switch
//
// Switch               SDL
// A                    SDL_CONTROLLER_BUTTON_A
// B                    SDL_CONTROLLER_BUTTON_B
// X                    SDL_CONTROLLER_BUTTON_X
// Y                    SDL_CONTROLLER_BUTTON_Y
// d-pad up             SDL_CONTROLLER_BUTTON_DPAD_LEFT
// d-pad right          SDL_CONTROLLER_BUTTON_DPAD_RIGHT
// d-pad down           SDL_CONTROLLER_BUTTON_MAX
// d-pad left           SDL_CONTROLLER_BUTTON_DPAD_DOWN
// L                    SDL_CONTROLLER_BUTTON_START
// R                    SDL_CONTROLLER_BUTTON_LEFTSTICK
// ZL                   SDL_CONTROLLER_BUTTON_RIGHTSTICK
// ZR                   SDL_CONTROLLER_BUTTON_LEFTSHOULDER
// right stick click    SDL_CONTROLLER_BUTTON_GUIDE
// left stick click     ?
#define BUTTON_PRESSED_VALIDATE     event.type == SDL_JOYBUTTONDOWN && event.jbutton.button == SDL_CONTROLLER_BUTTON_A
#define BUTTON_PRESSED_BACK         event.type == SDL_JOYBUTTONDOWN && event.jbutton.button == SDL_CONTROLLER_BUTTON_B
#define BUTTON_PRESSED_MENU         event.type == SDL_JOYBUTTONDOWN && event.jbutton.button == SDL_CONTROLLER_BUTTON_RIGHTSHOULDER
#define BUTTON_PRESSED_UP           event.type == SDL_JOYBUTTONDOWN && event.jbutton.button == SDL_CONTROLLER_BUTTON_DPAD_LEFT
#define BUTTON_PRESSED_RIGHT        event.type == SDL_JOYBUTTONDOWN && event.jbutton.button == SDL_CONTROLLER_BUTTON_DPAD_RIGHT
#define BUTTON_PRESSED_DOWN         event.type == SDL_JOYBUTTONDOWN && event.jbutton.button == SDL_CONTROLLER_BUTTON_MAX
#define BUTTON_PRESSED_LEFT         event.type == SDL_JOYBUTTONDOWN && event.jbutton.button == SDL_CONTROLLER_BUTTON_DPAD_DOWN
#define BUTTON_PRESSED_LINE_UP      event.type == SDL_JOYBUTTONDOWN && event.jbutton.button == SDL_CONTROLLER_BUTTON_X
#define BUTTON_PRESSED_LINE_RIGHT   event.type == SDL_JOYBUTTONDOWN && event.jbutton.button == SDL_CONTROLLER_BUTTON_A
#define BUTTON_PRESSED_LINE_DOWN    event.type == SDL_JOYBUTTONDOWN && event.jbutton.button == SDL_CONTROLLER_BUTTON_B
#define BUTTON_PRESSED_LINE_LEFT    event.type == SDL_JOYBUTTONDOWN && event.jbutton.button == SDL_CONTROLLER_BUTTON_Y
#define BUTTON_PRESSED_UNDO         event.type == SDL_JOYBUTTONDOWN && (event.jbutton.button == SDL_CONTROLLER_BUTTON_LEFTSTICK || event.jbutton.button == SDL_CONTROLLER_BUTTON_LEFTSHOULDER)
#define BUTTON_HELD_UP              SDL_JoystickGetButton(g_joystick, SDL_CONTROLLER_BUTTON_DPAD_LEFT)
#define BUTTON_HELD_RIGHT           SDL_JoystickGetButton(g_joystick, SDL_CONTROLLER_BUTTON_DPAD_RIGHT)
#define BUTTON_HELD_DOWN            SDL_JoystickGetButton(g_joystick, SDL_CONTROLLER_BUTTON_MAX)
#define BUTTON_HELD_LEFT            SDL_JoystickGetButton(g_joystick, SDL_CONTROLLER_BUTTON_DPAD_DOWN)
#define BUTTON_HELD_MOD             SDL_JoystickGetButton(g_joystick, SDL_CONTROLLER_BUTTON_START) || SDL_JoystickGetButton(g_joystick, SDL_CONTROLLER_BUTTON_RIGHTSTICK)
#else
// Button events for PC
#define BUTTON_PRESSED_VALIDATE     event.type == SDL_KEYDOWN && event.key.repeat == 0 && event.key.keysym.sym == SDLK_RETURN
#define BUTTON_PRESSED_BACK         event.type == SDL_KEYDOWN && event.key.repeat == 0 && event.key.keysym.sym == SDLK_ESCAPE
#define BUTTON_PRESSED_MENU         event.type == SDL_KEYDOWN && event.key.repeat == 0 && event.key.keysym.sym == SDLK_RETURN
#define BUTTON_PRESSED_UP           event.type == SDL_KEYDOWN && event.key.repeat == 0 && event.key.keysym.sym == SDLK_UP
#define BUTTON_PRESSED_RIGHT        event.type == SDL_KEYDOWN && event.key.repeat == 0 && event.key.keysym.sym == SDLK_RIGHT
#define BUTTON_PRESSED_DOWN         event.type == SDL_KEYDOWN && event.key.repeat == 0 && event.key.keysym.sym == SDLK_DOWN
#define BUTTON_PRESSED_LEFT         event.type == SDL_KEYDOWN && event.key.repeat == 0 && event.key.keysym.sym == SDLK_LEFT
#define BUTTON_PRESSED_LINE_UP      event.type == SDL_KEYDOWN && event.key.repeat == 0 && event.key.keysym.sym == SDLK_r
#define BUTTON_PRESSED_LINE_RIGHT   event.type == SDL_KEYDOWN && event.key.repeat == 0 && event.key.keysym.sym == SDLK_g
#define BUTTON_PRESSED_LINE_DOWN    event.type == SDL_KEYDOWN && event.key.repeat == 0 && event.key.keysym.sym == SDLK_f
#define BUTTON_PRESSED_LINE_LEFT    event.type == SDL_KEYDOWN && event.key.repeat == 0 && event.key.keysym.sym == SDLK_d
#define BUTTON_PRESSED_UNDO         event.type == SDL_KEYDOWN && event.key.repeat == 0 && event.key.keysym.sym == SDLK_u
#define BUTTON_HELD_UP              SDL_GetKeyboardState(NULL)[SDL_SCANCODE_UP]
#define BUTTON_HELD_RIGHT           SDL_GetKeyboardState(NULL)[SDL_SCANCODE_RIGHT]
#define BUTTON_HELD_DOWN            SDL_GetKeyboardState(NULL)[SDL_SCANCODE_DOWN]
#define BUTTON_HELD_LEFT            SDL_GetKeyboardState(NULL)[SDL_SCANCODE_LEFT]
#define BUTTON_HELD_MOD             SDL_GetKeyboardState(NULL)[SDL_SCANCODE_L]
#endif

#endif
