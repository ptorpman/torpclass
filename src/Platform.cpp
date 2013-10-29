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

///-----------------------------------------------------------------------------
/// FUNCTION DEFINITIONS
///-----------------------------------------------------------------------------
namespace TorpClass {

   int
   Platform::Init()
   {
      mSocketMap.clear();
      return 0;
   }
   
   void
   Platform::RegisterTimeoutCallback(void (*callback)(void))
   {
      mTimeoutCallback = callback;
   }


   
   int
   Platform::AddSocketForPolling(SOCKET sd, SocketType type, void (*callback)(SOCKET sd, SocketType type))
   {
      map<SOCKET, SocketHelper_t*>::iterator i = mSocketMap.find(sd);

      if (i != mSocketMap.end()) {
         // Already there
         return 1;
      }

      SocketHelper_t* sh = new SocketHelper_t;

      sh->sd = sd;
      sh->type = type;
      sh->callback = callback;


      mSocketMap.insert(pair<SOCKET,SocketHelper_t*>(sd,sh));

      // Rebuild poll struct
      rebuildPollStruct();

      return 0;
   }

   int
   Platform::RemoveSocketFromPolling(SOCKET sd)
   {
      map<SOCKET, SocketHelper_t*>::iterator i = mSocketMap.find(sd);

      if (i == mSocketMap.end()) {
         // Not there
         return 1;
      }

      SocketHelper_t* sh = i->second;

      mSocketMap.erase(sd);

      delete sh;

      // Rebuild poll struct
      rebuildPollStruct();

      return 0;
   }


} // namspace TorpClass
