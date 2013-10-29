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

#include "SocketManager.h"

namespace TorpClass {

	///-------------------------------------------------------------------------
	/// FUNCTION DECLARATIONS
	///-------------------------------------------------------------------------


	///-------------------------------------------------------------------------
	/// FUNCTION DEFINITIONS
	///-------------------------------------------------------------------------
	int
	SocketManager::Init()
	{
       return 0;
	}

	int
	SocketManager::addSocket(SocketType type, SOCKET sd, void (*callback)(SOCKET s, SocketType t))
	{	
		return Platform::instance()->AddSocketForPolling(sd, type, callback);
	}

	int
	SocketManager::removeSocket(SOCKET sd)
	{
		return Platform::instance()->RemoveSocketFromPolling(sd);
	}
	
	int
	SocketManager::pollSockets(int msecs)
	{
		return Platform::instance()->PollSockets(msecs);
	}

	SOCKET
	SocketManager::openSocket(int family, int type)
	{
		return socket(family, type, 0);
	}

	int
	SocketManager::closeSocket(SOCKET sd)
	{
		return Platform::instance()->CloseSocket(sd);
	}

	int
	SocketManager::setupSocket(SOCKET sd, bool nonBlock, bool reusable)
	{
		int res = 0;
		int flag = 1;
	
		if (reusable) {
			// Set socket as reusable
			res = setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, (char *)&flag, sizeof(flag));

			if (res < 0) {
				return 1;
			}
		}

		if (nonBlock) {
			res = Platform::instance()->SetNonBlock(sd);

			if (res < 0) {
	   
				return 1;
			}
		}
	
		return 0;
	}

	SOCKET
	SocketManager::acceptSocket(SOCKET sd, struct sockaddr* addr, int* addrLen)
	{
		return Platform::instance()->AcceptSocket(sd, addr, addrLen);
	}



} // namespace TorpClass
