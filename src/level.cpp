#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "level.h"
#include "def.h"
#include "sdlutils.h"

CLevel::CLevel(void) :
   m_levelNumber(0),
   m_draw(),
   m_imageGrid(NULL),
   m_cells(NULL)
{
}

CLevel::~CLevel(void)
{
   // Delete cells
   if (m_cells != NULL)
   {
      delete [] m_cells;
      m_cells = NULL;
   }
}

void CLevel::update(void)
{
   m_draw.update();
}

const bool CLevel::handle(const SDL_Event &p_event)
{
   return m_draw.handle(p_event);
}

void CLevel::reset(void)
{
   INHIBIT(std::cout << "CLevel::reset" << std::endl;)
   // Reset drawing
   m_draw.reset();
   // Free cells
   if (m_cells != NULL)
   {
      delete [] m_cells;
      m_cells = NULL;
   }
   // Reset hints
   m_hints.clear();
   // Reset level number
   m_levelNumber = 0;
   // Reset grid image
   m_imageGrid = NULL;
}

const unsigned int CLevel::getLevelNumber(void) const
{
   return m_levelNumber;
}

void CLevel::loadLevel(const unsigned int p_number, const std::string &p_drawing)
{
   INHIBIT(std::cout << "CLevel::loadLevel(" << p_number << ")" << std::endl;)

   // Get level data from file
   std::ifstream l_file(std::string(PATH_PREFIX) + LEVELDATA_FILE);
   if (! l_file.is_open())
   {
      std::cerr << "CLevel::loadLevel: unable to read file '" << LEVELDATA_FILE << "'"<< std::endl;
      return;
   }
   unsigned int l_levelNumber(0);
   std::string l_line("");
   std::string l_level("");
   std::string l_hints("");
   unsigned int l_i(0);
   while (!l_file.eof())
   {
      std::getline(l_file, l_line);
      if (!l_line.empty() && l_line.at(0) != '#')
      {
         ++l_levelNumber;
         if (p_number == l_levelNumber)
         {
            // It's the requested level
            l_i = l_line.find(' ');
            l_level = l_line.substr(0, l_i);
            l_hints = l_line.substr(l_i + 1);
            break;
         }
      }
   }
   l_file.close();
   if (l_level.empty())
   {
      std::cerr << "CLevel::loadLevel: unable to find level '" << p_number << "' in file '" << LEVELDATA_FILE << "'"<< std::endl;
      return;
   }
   INHIBIT(std::cout << "Level data  : '" << l_level << "'" << std::endl;)
   INHIBIT(std::cout << "Level hints : '" << l_hints << "'" << std::endl;)

   // Update level variables
   switch (l_level.size())
   {
      case 25:
         // 5x5 puzzle
         g_nbCellsX = 5;
         g_nbCellsY = 5;
         g_offsetX = 484;
         g_offsetY = 204;
         m_imageGrid = g_resourceManager->getTexture(T_TEXTURE_GRID5);
         break;
      case 49:
         // 7x7 puzzle
         g_nbCellsX = 7;
         g_nbCellsY = 7;
         g_offsetX = 421;
         g_offsetY = 141;
         m_imageGrid = g_resourceManager->getTexture(T_TEXTURE_GRID7);
         break;
      case 100:
         // 10x10 puzzle
         g_nbCellsX = 10;
         g_nbCellsY = 10;
         g_offsetX = 326;
         g_offsetY = 46;
         m_imageGrid = g_resourceManager->getTexture(T_TEXTURE_GRID10);
         break;
      case 400:
         // 20x20 puzzle
         g_nbCellsX = 20;
         g_nbCellsY = 20;
         g_offsetX = 326;
         g_offsetY = 46;
         m_imageGrid = g_resourceManager->getTexture(T_TEXTURE_GRID10);
         break;
      default:
         std::cerr << "CLevel::loadLevel: Unexpected level length: " << l_level.size() << std::endl;
         return;
   }

   // Allocate cells
   if (m_cells != NULL)
   {
      delete [] m_cells;
      m_cells = NULL;
   }
   m_cells = new CCell[g_nbCellsX * g_nbCellsY];

   // Decode level
   unsigned int l_val(0);
   for (l_i = 0; l_i < g_nbCellsX * g_nbCellsY; ++l_i)
   {
      std::istringstream l_stream(l_level.substr(l_i, 1));
      l_stream >> std::hex;
      l_stream >> l_val;
      m_cells[l_i].setCellValue(l_val);
   }

   // Decode hints
   m_hints.clear();
   l_val = l_hints.size() - 2;
   for (l_i = 0; l_i <= l_val; l_i += 2)
      m_hints.push_back(CPoint(l_hints[l_i] - '0', l_hints[l_i + 1] - '0'));

   // New level number
   m_levelNumber = p_number;

   // Load saved drawing if any
   m_draw.loadLevel(p_drawing);
}

const bool CLevel::solutionFound(void)
{
   INHIBIT(std::cout << "CLevel::solutionFound" << std::endl;)
   bool l_ret(true);
   if (m_levelNumber == 0)
      return false;
   unsigned int l_i(0);
   unsigned int l_j(0);
   for (l_i = 0; l_ret && (l_i < g_nbCellsX); ++l_i)
   {
      for (l_j = 0; l_ret && (l_j < g_nbCellsY); ++l_j)
      {
         if (!(m_draw.getCell()[l_i * g_nbCellsX + l_j] == m_cells[l_i * g_nbCellsX + l_j]))
         l_ret = false;
      }
   }
   return l_ret;
}

void CLevel::render(const bool p_renderCursor)
{
   // Render background & grid
   SDL_RenderCopy(g_renderer, g_resourceManager->getTexture(T_TEXTURE_BACKGROUND), NULL, NULL);
   SDL_RenderCopy(g_renderer, g_resourceManager->getTexture(g_nbCellsX >= 10 ? T_TEXTURE_FRAME2 : T_TEXTURE_FRAME), NULL, NULL);
   SDL_RenderCopy(g_renderer, m_imageGrid, NULL, NULL);

   // Level number
   SDL_utils::renderTexture(g_resourceManager->getTexture(T_TEXTURE_LEVEL), 113, 95);
   SDL_utils::renderNumber(m_levelNumber, 186, 197, 0, true);

   // Draw arrows and borders for levels >= 10
   if (g_nbCellsX >= g_cellsOnScreenX || g_nbCellsY >= g_cellsOnScreenY)
   {
      if (g_cameraX + g_cellsOnScreenX >= g_nbCellsX)
         SDL_utils::renderTexture(g_resourceManager->getTexture(T_TEXTURE_BORDER_RIGHT), 953, 43);
      else
         SDL_utils::renderTexture(g_resourceManager->getTexture(T_TEXTURE_ARROW_RIGHT), 960, 330);
      if (g_cameraX == 0)
         SDL_utils::renderTexture(g_resourceManager->getTexture(T_TEXTURE_BORDER_LEFT), 322, 43);
      else
         SDL_utils::renderTexture(g_resourceManager->getTexture(T_TEXTURE_ARROW_LEFT), 289, 330);
      if (g_cameraY + g_cellsOnScreenY >= g_nbCellsY)
         SDL_utils::renderTexture(g_resourceManager->getTexture(T_TEXTURE_BORDER_BOTTOM), 322, 673);
      else
         SDL_utils::renderTexture(g_resourceManager->getTexture(T_TEXTURE_ARROW_DOWN), 610, 680);
      if (g_cameraY == 0)
         SDL_utils::renderTexture(g_resourceManager->getTexture(T_TEXTURE_BORDER_TOP), 322, 42);
      else
         SDL_utils::renderTexture(g_resourceManager->getTexture(T_TEXTURE_ARROW_UP), 610, 9);
   }

   // Render drawing
   m_draw.render(p_renderCursor);

   // Render hints
   unsigned int nbLinesDrawing(0);
   unsigned int nbLinesLevel(0);
   for (std::vector<CPoint>::const_iterator l_it = m_hints.begin(); l_it != m_hints.end(); ++l_it)
   {
      if (l_it->m_x >= g_cameraX && l_it->m_x < g_cameraX + g_cellsOnScreenX && l_it->m_y >= g_cameraY && l_it->m_y < g_cameraY + g_cellsOnScreenY)
      {
         // The cell is inside the camera => we draw it
         nbLinesDrawing = m_draw.getCell()[l_it->m_x * g_nbCellsX + l_it->m_y].getNbLines();
         nbLinesLevel = m_cells[l_it->m_x * g_nbCellsX + l_it->m_y].getNbLines();
         // Source rectange
         SDL_Rect srcRect;
         srcRect.x = nbLinesLevel * 21;
         if (nbLinesDrawing == nbLinesLevel)
            srcRect.y = 26;
         else
            srcRect.y = 0;
         srcRect.w = 21;
         srcRect.h = 26;
         // Destination rectangle
         SDL_Rect destRect;
         destRect.x = g_offsetX + (l_it->m_x - g_cameraX) * g_cellStepX + 19;
         destRect.y = g_offsetY + (l_it->m_y - g_cameraY) * g_cellStepY + 17;
         destRect.w = 21;
         destRect.h = 26;
         // Render hint
         SDL_RenderCopy(g_renderer, g_resourceManager->getTexture(T_TEXTURE_NUMBERS), &srcRect, &destRect);
      }
   }
}

CDraw* CLevel::getDrawing(void)
{
   return &m_draw;
}
