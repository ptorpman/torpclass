#ifndef TIMER_H
#define TIMER_H
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

using namespace std;

namespace TorpClass {

   ///
   /// This class implements the Timer 
   ///
   class Timer
   {
     public:
      /// Constructor
      Timer(uint64_t fireTime, TimerCallback cb, void* userData);
      /// Destructor
      ~Timer();

      /// Returns if active
      bool isActive() { return mActive; }

      /// Set user data
      void setUserData(void* userData) { mUserData = userData; }

      /// Timer has fired!
      void fired();
      
     private:
      /// Flag if active/running
      bool mActive;
      /// Time when to fire
      uint64_t mFireTime;
      /// Timer callback
      TimerCallback mCallback;
      /// User data
      void* mUserData;
   };

} // namespace TorpClass


    
#endif  // TIMER_H
