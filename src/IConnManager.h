#ifndef ICONNMANAGER_H
#define ICONNMANAGER_H
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

namespace TorpClass {

   // Predeclaration
   class Client;
   class Server;

   /// Client callback
   /// @param client  Pointer to client
   /// @param code    Status code. If code is 0xDEADDEAD the client is dead.
   typedef void (*ClientConnCallback)(Client* client, int code);

   /// Server callback
   /// @param serv    Pointer to server
   /// @param code    Status code. If code is 0xDEADDEAD the server is dead.
   typedef void (*ServerConnCallback)(Server* serv, int code);

   ///--------------------------------------------------------------------------
   /// INTERFACE USED FOR CONNMANAGER FUNCTIONALITY
   ///--------------------------------------------------------------------------

   class IConnManager
   {
     public:

      /// Initalize the ConnManager
      virtual int Init() = 0;
       
      /// Starts the Client services and connects to a remote address
      /// @return Pointer to Connection  if success, NULL if failure
		
      virtual Client* StartClient(char* ipAddr, int port) = 0; 

      /// Stops the Client services and disconnects from a remote address
      virtual int StopClient(Client* client) = 0; 

      /// Register a callback for the client
      virtual void RegisterClientCallback(ClientConnCallback c) = 0;

      /// Starts a Server and opens a server socket
      /// @param addr    Dotted IP address
      /// @param port    Port number
      /// @return Pointer to Server if OK, NULL otherwise
      virtual Server* StartServer(char* addr, int port) = 0; 

      /// Stops the Server services and closes the server socket
      virtual int StopServer(Server* serv) = 0;

      /// Register a callback for the server
      /// @param c    Callback
      virtual void RegisterServerCallback(ServerConnCallback c) = 0;

		
      /// Handle events (poll sockets etc.)
      /// @param msecs  Timeout in milliseconds
      /// @return 0 if OK, error code otherwise
      virtual int HandleEvents(int msecs) = 0;
   };


} // namespace TorpClass


#endif // ICONNMANAGER_H
