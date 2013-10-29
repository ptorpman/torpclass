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
#include "TorpClass.h"
#include "StackTrace.h"

namespace TorpClass 
{
   // Initialize TorpClass
   int
   Init()
   {
#ifdef LINUX
      InstallStackTrace();
#endif
      
      // Initialize our singletons
      Platform::instance()->Init();
      SocketManager::instance()->Init();
      TimerManager::instance()->Init();
      ConnManager::instance()->Init();
      
      return 0;
   }

   void
   HandleEvents(int msecs)
   {
      // Handle timers first
      TimerManager::instance()->HandleEvents(msecs);

      // Handle sockets
      ConnManager::instance()->HandleEvents(msecs);
   }
   

   
}

     


