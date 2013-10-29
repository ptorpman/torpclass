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

#include "Server.h"
#include "Address.h"
#include "SocketManager.h"
#include "Trace.h"

namespace TorpClass {

	///-----------------------------------------------------------------------------
	/// FUNCTION DECLARATIONS
	///-----------------------------------------------------------------------------

	///-----------------------------------------------------------------------------
	/// FUNCTION DEFINITIONS
	///-----------------------------------------------------------------------------

	Server::Server(Address* addr)
	{
		mAddress = addr;
		mOperational = false;
		mReadBufferPos = 0;
		mSocket = INVALID_SOCKET;
	}

	/// Destructor
	Server::~Server()
	{
		if (mAddress) {
			delete mAddress;
		}

		if (mSocket != INVALID_SOCKET) {
			SocketManager::instance()->closeSocket(mSocket);
			mSocket = INVALID_SOCKET;
		}
	}

	int
	Server::Start()
	{
		// Open up a server socket
		mSocket = SocketManager::instance()->openSocket(AF_INET, SOCK_STREAM);

		if (IS_INVALID_SOCKET(mSocket)) {
			Trace::instance()->trace(TRACE_LEVEL_ERROR, __FILE__, __LINE__, "Could not open socket");
			return 1;
		}

		// Set socket reusable and non-blocking
		int res = SocketManager::instance()->setupSocket(mSocket, true, true);

		if (res < 0) {
			Trace::instance()->trace(TRACE_LEVEL_ERROR, __FILE__, __LINE__, "Could not setup socket");
			goto error_cleanup;
		}
	
		// Bind to address and port
		res = bind(mSocket, mAddress->getSockAddr(), mAddress->getAddrLength());

		if (res != 0) {
			Trace::instance()->trace(TRACE_LEVEL_ERROR, __FILE__, __LINE__, "Could not bind socket (res %d errno %d)",
											 res, errno);
			goto error_cleanup;
		}
	
		// Listen
		res = listen(mSocket, 10);

		if (res != 0) {
			Trace::instance()->trace(TRACE_LEVEL_ERROR, __FILE__, __LINE__, "Could not listen on socket");
			goto error_cleanup;
		}
		
		Trace::instance()->trace(TRACE_LEVEL_TRACE, __FILE__, __LINE__, "Server socket %d is up!", mSocket);

		return 0;

	error_cleanup:
		SocketManager::instance()->closeSocket(mSocket);
		mSocket = INVALID_SOCKET;
		return 1;
	}
	
	void
	Server::Stop()
	{
		SocketManager::instance()->closeSocket(mSocket);
		mSocket = INVALID_SOCKET;
	}

	
	int
	Server::HandleSocket()
	{
		//

		
		int res = Platform::instance()->ReadSocket(mSocket, mReadBuffer, CONN_BUFF_SIZE - mReadBufferPos);
  
		if (res == 0) {
			// Socket has been closed
			TorpClass::Trace::instance()->trace(TRACE_LEVEL_TRACE, __FILE__, __LINE__, "Socket is closed!");
			return -1;
		}

		if (res < 0) {
			// An error occured
			// FIXME!
			TorpClass::Trace::instance()->trace(TRACE_LEVEL_TRACE, __FILE__, __LINE__, "Socket error!");
			return 0;
		}

		TorpClass::Trace::instance()->trace(TRACE_LEVEL_TRACE, __FILE__, __LINE__, "Server read %d bytes", res);

		
		// Some bytes were read
		mReadBufferPos += res;
 
  
		return 0;
	}

	unsigned char*
	Server::getReadBuffer(int* numBytes)
	{
		*numBytes = mReadBufferPos;
		return mReadBuffer;
	}

	void
	Server::Flush()
	{
		// FIXME!
	} 
	


} // namespace TorpClass
