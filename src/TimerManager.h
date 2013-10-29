#ifndef TIMER_MANAGER_H
#define TIMER_MANAGER_H
//==============================================================================
//  Copyright (c) 2013  Peter R. Torpman (peter at torpman dot se)
//
//  This file is part of TorpClass (http://torpclass.sourceforge.net)
//
//  TorpClass is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 3 of the License, or
//  (at your option) any later version.
//
//  TorpClass is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.or/licenses/>.
//==============================================================================

#include "Platform.h"
#include <map>

using namespace std;

namespace TorpClass {

   /// Predeclaration
   class Timer;

   
   ///
   /// This class implements the Timer Manager
   ///
   class TimerManager
   {
      
     public:

      /// Singleton instance 
      static TimerManager* instance()
      {
         static TimerManager* inst = new TimerManager();
         return inst;
      }

      /// Initialize the TimerManager
      void Init();
      
      /// Start Timer Manager functionality
      void Start();

      /// Handle timer events
      int HandleEvents(int msecs);

      
      /// Create (and start) a timer
      /// @param msecs    Timeout in milliseconds
      /// @param cb       Timer callback
      /// @param userData Pointer to user data
      /// @return Pointer to timer or NULL if failure
      Timer* CreateTimer(int msecs, TimerCallback cb, void* userData);

      /// Delete (and cancel) a timer
      /// @param tim    Pointer to timer
      void DeleteTimer(Timer* tim);
      
      /// Handle timer callback
      static void timeoutCallback(void);
        
        
     private:

      /// Default constructor 
      TimerManager() {};
      TimerManager(TimerManager const&); // Don't implement
      void operator=(TimerManager const&); // Don't implement
     
      
      /// Default destructor 
      ~TimerManager() { }

      /// Current time
      uint64_t mTime;

      /// Map of all timers (indexed on fire time)
      multimap<uint64_t,Timer*> mTimers;

      /// Map of all expired timers
      map<Timer*,Timer*> mFiredTimers;

      /// Check expired timers
      void checkExpired();
   };
    

    
} // namespace TorpClass

#endif // TIMERMANAGER_H
