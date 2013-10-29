#ifndef SOCKET_MANAGER_H
#define SOCKET_MANAGER_H
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
#include <map>

using namespace std;

namespace TorpClass {


	///
	/// This class implements the Socket Manager
	///
	class SocketManager
	{
	public:

		/// Singleton instance 
		static SocketManager* instance()
		{
			static SocketManager* inst = new SocketManager();
			return inst;
		}

		/// Initialize the SocketManager
		int Init();

		/// Add socket for polling
		int addSocket(SocketType type, SOCKET sd, void (*callback)(SOCKET s, SocketType t));

		/// Remove socket from polling
		int removeSocket(SOCKET sd);
	
		/// Poll sockets
		int pollSockets(int msecs);

		/// Open up a new socket
		SOCKET openSocket(int family, int type);
		/// Close socket
		int closeSocket(SOCKET sd);
		/// Setup socket
		int setupSocket(SOCKET sd, bool nonBlock, bool reusable);
		/// Accept connection
		/// @return socket or INVALID_SOCKET
		SOCKET acceptSocket(SOCKET sd, struct sockaddr* addr, int* addrLen);
	
	private:

		/// Default constructor 
		SocketManager() { }
		/// Default destructor 
		~SocketManager() { }
	};

} // namespace TorpClass


#endif // SOCKET_MANAGER

