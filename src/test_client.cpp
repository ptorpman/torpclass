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

//
// A test client
//
//
#include "TorpClass.h"

using namespace TorpClass;

static void connCallback(Client* cli, int code);

// Send a message to the server
unsigned char buff[0xFF];

int
main(int argc, char** argv)
{
   // Initialize TorpClass
   int res = TorpClass::Init();

   if (res != 0) {
      // Failure!
      fprintf(stderr, "Failed to initialized TorpClass!\n");
      return 1;
   }

   // Set a high trace level
   Trace::instance()->setLevel(TRACE_LEVEL_TRACE);

   // Register a callback
   ConnManager::instance()->RegisterClientCallback(connCallback);

   // Connect to the server
   Client* cli = ConnManager::instance()->StartClient((char*)"127.0.0.1", 7777);

   if (cli == NULL) {
	  fprintf(stderr, "Failed to connect to server!\n");
	  return 1;
   }

   strcpy((char*) buff, "Hello, World!");
  
   cli->Send(buff, strlen((char*) buff) + 1);

   while (1) {
      // Handle socket events
      TorpClass::HandleEvents(10);
   }

   return 0;
}


static void
connCallback(Client* cli, int code)
{
   TorpClass::Trace::instance()->trace(TRACE_LEVEL_TRACE, __FILE__, __LINE__, "Client callback! code=%d", code);

   int res = cli->HandleSocket();

   if (res == -1) {
      TorpClass::Trace::instance()->trace(TRACE_LEVEL_TRACE, __FILE__, __LINE__, "Socket to be removed!");
      ConnManager::instance()->StopClient(cli);
      return;
   }

	
   int numBytes = 0;
   unsigned char* buff = cli->getReadBuffer(&numBytes);

   if (numBytes != 0) {
      TorpClass::Trace::instance()->trace(TRACE_LEVEL_TRACE, __FILE__, __LINE__, "Stuff on connection (%d)", numBytes);
		
      fprintf(stderr, "Read: %s\n", (char*) buff);

      cli->Send(buff, strlen((char*) buff) + 1);
   }

   // Mark all read bytes handled
   cli->setReadBufferPos(0);
}


