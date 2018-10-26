#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <cstdio>
#include "saveManager.h"
#include "def.h"
#include "cell.h"

CSaveManager::CSaveManager(void):
   m_save("")
{
   // Load save file
   std::ifstream l_file(SAVE_FILE);
   if (l_file.is_open())
   {
      INHIBIT(std::cout << "Reading file '" << SAVE_FILE << "'..." << std::endl;)
      std::getline(l_file, m_save);
      l_file.close();
   }
   else
   {
      INHIBIT(std::cout << "Unable to open file " << SAVE_FILE << " for reading" << std::endl;)
      // Create save string with zeros
      m_save.insert(0, NB_LEVELS, '0');
   }
}

CSaveManager::~CSaveManager(void)
{
}

const bool CSaveManager::getLevelIsSolved(const unsigned int p_nb) const
{
   return m_save.at(p_nb - 1) == '1';
}

void CSaveManager::setLevelIsSolved(const unsigned int p_nb)
{
   // Set level to "solved"
   m_save.at(p_nb - 1) = '1';
   // Write save file
   INHIBIT(std::cout << "Writing file '" << SAVE_FILE << "'..." << std::endl;)
   std::ofstream l_file(SAVE_FILE);
   if (!l_file.is_open())
   {
      std::cerr << "Unable to write file '" << SAVE_FILE << "'" << std::endl;
      return;
   }
   l_file << m_save;
   l_file.close();
}

const unsigned int CSaveManager::getFirstUnsolvedLevel(void) const
{
   unsigned int l_i(0);
   while (l_i < NB_LEVELS && m_save[l_i] != '0')
      ++l_i;
   if (l_i == NB_LEVELS)
      // All levels are solved
      return 1;
   return l_i + 1;
}

void CSaveManager::saveState(CLevel *p_level) const
{
   INHIBIT(std::cout << "Saving state for level " << p_level->getLevelNumber() << std::endl;)
   std::ostringstream oss;
   oss << p_level->getLevelNumber() << std::endl;
   oss << std::hex;
   int nbCellsTotal = g_nbCellsX * g_nbCellsY;
   const CCell *cells = p_level->getDrawing()->getCell();
   // Build string representing the current drawing
   for (int index = 0; index < nbCellsTotal; ++index)
      oss << std::setfill('0') << std::setw(2)
          << (int)(((cells[index].getLineState(T_DIRECTION_UP) == 2) << 7) | ((cells[index].getLineState(T_DIRECTION_RIGHT) == 2) << 6) | ((cells[index].getLineState(T_DIRECTION_DOWN) == 2) << 5) | ((cells[index].getLineState(T_DIRECTION_LEFT) == 2) << 4) |
                   ((cells[index].getLineState(T_DIRECTION_UP) == 1) << 3) | ((cells[index].getLineState(T_DIRECTION_RIGHT) == 1) << 2) | ((cells[index].getLineState(T_DIRECTION_DOWN) == 1) << 1) | (cells[index].getLineState(T_DIRECTION_LEFT) == 1));
   // Write state in file
   std::ofstream ofs(SAVE_STATE_FILE);
   if (ofs.is_open())
   {
      ofs << oss.str();
      ofs.close();
   }
   INHIBIT(else std::cout << "Unable to open file " << SAVE_STATE_FILE << " for writing" << std::endl;)
}

void CSaveManager::getSaveState(unsigned int &p_level, std::string &p_draw) const
{
   p_level = 0;
   p_draw = "";
   std::ifstream ifs(SAVE_STATE_FILE);
   if (ifs.is_open())
   {
      // Get level number
      std::string line("");
      std::getline(ifs, line);
      std::istringstream l_stream(line);
      l_stream >> p_level;
      // Get drawing
      std::getline(ifs, p_draw);
      // Close
      ifs.close();
   }
   INHIBIT(std::cout << "CSave::getSaveStateInfo " << p_level << " " << p_draw << std::endl;)
}

void CSaveManager::clearSaveState(void) const
{
   INHIBIT(std::cout << "CSave::clearSaveState" << std::endl;)
   // Remove file if necessary
   std::ifstream ifs(SAVE_STATE_FILE);
   if (ifs.is_open())
   {
      // File exists
      ifs.close();
      if (remove(SAVE_STATE_FILE))
         perror("Error deleting file");
   }
}
