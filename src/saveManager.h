#ifndef _SAVEMANAGER_H_
#define _SAVEMANAGER_H_

#include <string>
#include "level.h"

#define SAVE_FILE "SlitherlinkNX.sav"
#define SAVE_STATE_FILE "SlitherlinkNX_state.sav"
#define NB_LEVELS 456

class CSaveManager
{
   public:

   // Constructor
   CSaveManager(void);

   // Destructor
   virtual ~CSaveManager(void);

   // Get status of a the given level
   const bool getLevelIsSolved(const unsigned int p_nb) const;

   // Set status of a the given level
   void setLevelIsSolved(const unsigned int p_nb);

   // Get first unsolved level
   const unsigned int getFirstUnsolvedLevel(void) const;

   // Save state for the given level
   void saveState(CLevel *p_level) const;

   // Get save state information
   void getSaveState(unsigned int &p_level, std::string &p_draw) const;

   // Delete save state file
   void clearSaveState(void) const;

   private:

   // The levels beaten (or not)
   std::string m_save;
};

#endif
