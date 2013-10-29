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

#include "TimerManager.h"
#include "SocketManager.h"
#include "Trace.h"
#include "Timer.h"

namespace TorpClass {

   ///-------------------------------------------------------------------------
   /// FUNCTION DECLARATIONS
   ///-------------------------------------------------------------------------
   void tm_timerCallback(SOCKET sd, SocketType t);

   ///-------------------------------------------------------------------------
   /// FUNCTION DEFINITIONS
   ///-------------------------------------------------------------------------
   void
   TimerManager::Init()
   {
      mTimers.clear();
      mFiredTimers.clear();
      mTime = 0;
   }

   void
   TimerManager::Start()
   {
      // Register a callback to be called when a timeout happens.
      Platform::instance()->RegisterTimeoutCallback(timeoutCallback);
   }

      
   void
   TimerManager::timeoutCallback(void)
   {
      // Check if timers have expired
      TimerManager::instance()->checkExpired();
   }

   void
   TimerManager::checkExpired()
   {
      uint64_t currTime = Platform::instance()->GetTime();

      // Loop through timers and find expired ones
      std::multimap<uint64_t,Timer*>::iterator i = mTimers.begin();
      std::multimap<uint64_t,Timer*>::iterator iTmp;
      
      Timer* tim = NULL;

      while (i != mTimers.end()) {
         if (i->first <= currTime) {
            Trace::instance()->trace(
               TRACE_LEVEL_TRACE, __FILE__, __LINE__,
               "Times is %lu - Found expired timer! %lu Diff: %lu",
               currTime, i->first, (currTime - i->first));

            // Fired timer
            tim = i->second;
            iTmp = i;
            ++i;
            mTimers.erase(iTmp);
            // Move to fired timers
            mFiredTimers[tim] = tim;
         }
         else {
            ++i;
         }
      }
   }

   Timer*
   TimerManager::CreateTimer(int msecs, TimerCallback cb, void* userData)
   {
      // Calculate when it should fire
      uint64_t currTime = Platform::instance()->GetTime();

      Trace::instance()->trace(TRACE_LEVEL_TRACE, __FILE__, __LINE__,
                               "Curr Time: %ld tmo: %d", currTime, msecs );


      
      // Create timer
      Timer* t = new Timer(currTime + msecs, cb, userData);
      
      // Remember it...
      mTimers.insert(pair<uint64_t,Timer*>(currTime + msecs,t));

      return t;
   }
   
   void
   TimerManager::DeleteTimer(Timer* tim)
   {

   }
   
   int
   TimerManager::HandleEvents(int msecs)
   {
      // Go through expired timers and call right callbacks
      map<Timer*,Timer*>::iterator i;

      for (i = mFiredTimers.begin(); i != mFiredTimers.end(); ++i) {
         i->second->fired();
      }

      mFiredTimers.clear();

      return 0;
   }
   


   


} // namespace TorpClass

