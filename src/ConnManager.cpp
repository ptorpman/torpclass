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

#include "ConnManager.h"
#include "Platform.h"
#include <string.h>
#include "Trace.h"
#include "SocketManager.h"
#include "Address.h"
#include "Server.h"
#include "Client.h"

namespace TorpClass {

	static void clientSocketCallback(SOCKET sd, SocketType t);
	static void serverSocketCallback(SOCKET sd, SocketType t);

	
	///-------------------------------------------------------------------------
	/// IConnManager Interface
	///-------------------------------------------------------------------------
   int
   ConnManager::Init()
   {
      mServers.clear();
      mServerSockets.clear();
      mClients.clear();
      mClientSockets.clear();
      return 0;
   }
   

	Server*
	ConnManager::StartServer(char* addr, int port)
	{
		int res = 0;
	
		// Get an address
		Address* a = new Address(addr, port);

		if (!a->isValid()) {
			delete a;
			Trace::instance()->trace(TRACE_LEVEL_ERROR, __FILE__, __LINE__, "Bad address");
			return NULL;
		}
		
		// Open up a server socket
		Server* serv = new Server(a);

		mServers[serv] = serv->getSocket();
		
		if (serv->Start() != 0) {
			// Failed to start server
			goto error_cleanup;
		}

		mServerSockets[serv->getSocket()] = serv;
		
		// Add server socket for polling
		res = SocketManager::instance()->addSocket(SOCKET_TYPE_SERVER, serv->getSocket(), serverSocketCallback);

		if (res < 0) {
			Trace::instance()->trace(TRACE_LEVEL_ERROR, __FILE__, __LINE__, "Could not add socket");
			goto error_cleanup;
		}
	
		Trace::instance()->trace(TRACE_LEVEL_TRACE, __FILE__, __LINE__, "Socket is up and running.");

	
		return serv;

	error_cleanup:
		delete serv;
		return NULL;
	}



	int
	ConnManager::StopServer(Server* serv)
	{
		if (!serv) {
			return 1;
		}

		// Check if server is available
		map<Server*,SOCKET>::iterator i = mServers.find(serv);

		if (i == mServers.end()) {
			Trace::instance()->trace(TRACE_LEVEL_ERROR, __FILE__, __LINE__, "Could not find server");
			return 1;
		}

		Server* s = i->first;

		// Remove socket from polling
		SocketManager::instance()->removeSocket(s->getSocket());
		
		s->Stop();
		
		mServers.erase(s);
		delete s;
		
		return 0;
	}


	int
	ConnManager::HandleEvents(int msecs)
	{
		int res = 0;

		// Try to flush any messages that needs flushing
		map<Client*,SOCKET>::iterator cIter;

 		if (mClients.size() > 0) {
			for (cIter = mClients.begin(); cIter != mClients.end(); ++cIter) {
				cIter->first->Flush();
			}
		}
			 
		map<Server*,SOCKET>::iterator sIter;
		
 		if (mServers.size() > 0) {
			for (sIter = mServers.begin(); sIter != mServers.end(); ++sIter) {
				sIter->first->Flush();
			}
		}
		
		// Poll sockets
		res = SocketManager::instance()->pollSockets(msecs);

		return res;
	}

   void
	ConnManager::handleServerEvent(SOCKET sd)
	{
		// Stuff on server socket. We should accept a new connection!
		int addrLen = 0;
		struct sockaddr_in addr;

		SOCKET cliSock = SocketManager::instance()->acceptSocket(sd, (struct sockaddr*) &addr, &addrLen);

		if (cliSock == INVALID_SOCKET) {
			Trace::instance()->trace(TRACE_LEVEL_ERROR, __FILE__, __LINE__, "Could not accept new client!");
			return;
		}

		Trace::instance()->trace(TRACE_LEVEL_TRACE, __FILE__, __LINE__, "Accepted new client on sock %d!", cliSock);

		Address* a = new Address(&addr, addrLen);

		Client* cli = new Client(a);

		cli->setSocket(cliSock);

		mClients[cli] = cli->getSocket();
		mClientSockets[cli->getSocket()] = cli;
		
		// Add socket for polling
		int res = SocketManager::instance()->addSocket(SOCKET_TYPE_SERVER, cli->getSocket(), clientSocketCallback);

		if (res < 0) {
			Trace::instance()->trace(TRACE_LEVEL_ERROR, __FILE__, __LINE__, "Could not add client");
			delete cli;
			return;
		}
	
		Trace::instance()->trace(TRACE_LEVEL_TRACE, __FILE__, __LINE__, "Accepted client is up and running.");
	}

	
	void
	ConnManager::handleClientEvent(SOCKET sd)
	{
		// Find right client
		map<SOCKET,Client*>::iterator i = mClientSockets.find(sd);

		Trace::instance()->trace(TRACE_LEVEL_TRACE, __FILE__, __LINE__, "Looking for client callback");
		
		if (i != mClientSockets.end()) {
			Trace::instance()->trace(TRACE_LEVEL_TRACE, __FILE__, __LINE__, "Calling client callback");

			if (mClientCallback) {
				mClientCallback(i->second, 0);
			}
			else {
				Trace::instance()->trace(TRACE_LEVEL_TRACE, __FILE__, __LINE__, "No client callback to call");
			}
		}
	}


	static void
	serverSocketCallback(SOCKET sd, SocketType t)
	{
		Trace::instance()->trace(TRACE_LEVEL_TRACE, __FILE__, __LINE__, "Stuff on server socket");
		
		ConnManager* cm = dynamic_cast<ConnManager*>(ConnManager::instance());
		cm->handleServerEvent(sd);
	}


	Client*
	ConnManager::StartClient(char* ipAddr, int port)
	{
		int res = 0;
		
		// Get an address
		Address* a = new Address(ipAddr, port);

		if (!a->isValid()) {
			delete a;
			Trace::instance()->trace(TRACE_LEVEL_ERROR, __FILE__, __LINE__, "Bad address");
			return NULL;
		}
		
		// Connect client to a server
		Client* cli = new Client(a);

		
		if (cli->Start() != 0) {
			// Failed to connect client to server
			goto error_cleanup;
		}

		mClients[cli] = cli->getSocket();
		mClientSockets[cli->getSocket()] = cli;

		// Add socket for polling
		res = SocketManager::instance()->addSocket(SOCKET_TYPE_SERVER, cli->getSocket(), clientSocketCallback);

		if (res < 0) {
			Trace::instance()->trace(TRACE_LEVEL_ERROR, __FILE__, __LINE__, "Could not add socket");
			goto error_cleanup;
		}
	
		Trace::instance()->trace(TRACE_LEVEL_TRACE, __FILE__, __LINE__, "Client is up and running.");

		return cli;

	error_cleanup:
		delete cli;
		return NULL;
	}

	int
	ConnManager::StopClient(Client* cli)
	{
		if (!cli) {
			return 1;
		}

		// Check if server is available
		map<Client*,SOCKET>::iterator i = mClients.find(cli);

		if (i == mClients.end()) {
			Trace::instance()->trace(TRACE_LEVEL_ERROR, __FILE__, __LINE__,
                                     "Could not find client");
			return 1;
		}

		Client* c = i->first;

		// Remove socket from polling
		SocketManager::instance()->removeSocket(c->getSocket());
		
		c->Stop();
		
		mClients.erase(c);
		delete c;
		
		return 0;
	}

	static void
	clientSocketCallback(SOCKET sd, SocketType t)
	{
		Trace::instance()->trace(TRACE_LEVEL_TRACE, __FILE__, __LINE__, "Stuff on client socket");

		ConnManager* cm = dynamic_cast<ConnManager*>(ConnManager::instance());
		cm->handleClientEvent(sd);

	}



} // namespace TorpClass
