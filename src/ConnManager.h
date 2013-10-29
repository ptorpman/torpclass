#ifndef CONNMANMAGER_H
#define CONNMANMAGER_H
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
#include "IConnManager.h"

namespace TorpClass {

	/// Predeclaration..
	class Client;

	/// Connection manager types
	typedef enum {
		CONN_MGR_NOT_SET = 0,
		CONN_MGR_CLIENT = 1,
		CONN_MGR_SERVER = 2
	} ConnManagerType;

	///
	/// This class implements the Connection Manager.
	///
	class ConnManager : public IConnManager
	{
	public:

		/// Returns singleton instance
		static IConnManager* instance()
		{
			static ConnManager* inst = new ConnManager();
			return dynamic_cast<IConnManager*>(inst);
		}

		///-------------------------------------------------------
		/// IConnManager methods
		///-------------------------------------------------------
        int Init();
		Client* StartClient(char* ipAddr, int port); 
		int StopClient(Client* client); 
		void RegisterClientCallback(ClientConnCallback c) { mClientCallback = c; }

		Server* StartServer(char* addr, int port); 
		int StopServer(Server* serv);
		void RegisterServerCallback(ServerConnCallback s) { mServerCallback = s; }
		int HandleEvents(int msecs);

		///-------------------------------------------------------
		/// Other methods
		///-------------------------------------------------------
  
		/// Check connections for events
		/// @param msecs   Timeout in milliseconds
		/// @return 0 if OK, error code otherwise
		int handleEvents(int msecs);

		void handleServerEvent(SOCKET sd);
		void handleClientEvent(SOCKET sd);
		
		
	private:

		/// Default constructor 
		ConnManager() { }
		/// Default destructor 
		virtual ~ConnManager() { }

		///-------------------------------------------------------
		/// Common attributes
		///-------------------------------------------------------

		/// Map of servers
		map<Server*, SOCKET> mServers;
		/// Map of servers (reverse)
		map<SOCKET,Server*> mServerSockets;
		/// Map of connected clients
		map<Client*,SOCKET> mClients;
		/// Map of connected clients (reverse)
		map<SOCKET,Client*> mClientSockets;

		///-------------------------------------------------------
		/// Server attributes
		///-------------------------------------------------------
		ServerConnCallback mServerCallback;
		ClientConnCallback mClientCallback;
	};


} // namespace TorpClass
#endif /* CONNMANMAGER_H */

