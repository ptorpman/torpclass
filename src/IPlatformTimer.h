#ifndef IPLATFORM_TIMER_H
#define IPLATFORM_TIMER_H
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


#include "Types.h"

///-----------------------------------------------------------------------------
/// INTERFACE USED FOR PLATFORM INDEPENDENCY TIMERS
///-----------------------------------------------------------------------------
class IPlatformTimer
{
  public:
   
   /// Returns current time in milliseconds
   virtual uint64_t GetTime() = 0;

   /// Register a timeout callback
   virtual void RegisterTimeoutCallback(void (*callback)(void)) = 0;
};





#endif // IPLATFORM_TIMER_H
