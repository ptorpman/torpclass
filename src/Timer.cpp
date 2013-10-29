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

#include "Timer.h"
#include "Trace.h"

namespace TorpClass {


   Timer::Timer(uint64_t fireTime, TimerCallback cb, void* userData)
   {
      mActive = true;
      mFireTime = fireTime;
      mCallback = cb;
      mUserData = userData;

      Trace::instance()->trace(TRACE_LEVEL_TRACE, __FILE__, __LINE__,
                               "Timer will fire at %ld", mFireTime);
   }

   /// Destructor
   Timer::~Timer()
   {

   }
   
   void
   Timer::fired()
   {
      mActive = false;

      if (mCallback) {
         mCallback(this, mUserData);
      }
   }
   
   

}
