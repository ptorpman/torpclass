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

#include "Client.h"
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

	Client::Client(Address* addr)
	{
		mAddress = addr;
		mSocket = INVALID_SOCKET;
		mReadBufferPos = 0;
		mWriteBufferPos = 0;
	}

	/// Destructor
	Client::~Client()
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
	Client::Start()
	{
		// Open socket
		mSocket = SocketManager::instance()->openSocket(AF_INET, SOCK_STREAM);
	

		if (mSocket < 0) {
			Trace::instance()->trace(TRACE_LEVEL_ERROR, __FILE__, __LINE__, "Could not open socket");
			return 1;
		}

		// Set socket reusable and non-blocking
		int res = SocketManager::instance()->setupSocket(mSocket, true, true);
	
		if (res != 0) {
			Trace::instance()->trace(TRACE_LEVEL_ERROR, __FILE__, __LINE__, "Could not setup socket");
			goto error_cleanup;
		}

		// Connect to remote address
		res = Platform::instance()->ConnectSocket(mSocket, mAddress->getSockAddr(), mAddress->getAddrLength());

		if (res < 0) {
			Trace::instance()->trace(TRACE_LEVEL_ERROR, __FILE__, __LINE__,
											 "Could not connect (errno=%d)", errno);
			goto error_cleanup;
		}

		return 0;

	error_cleanup:
		SocketManager::instance()->closeSocket(mSocket);
		mSocket = INVALID_SOCKET;
		return 1;
	}

	void
	Client::Stop()
	{
		SocketManager::instance()->closeSocket(mSocket);
		mSocket = INVALID_SOCKET;
	}

	int
	Client::HandleSocket()
	{
		int res = Platform::instance()->ReadSocket(mSocket, mReadBuffer, CONN_BUFF_SIZE - mReadBufferPos);
  
		if (res == 0) {
			// Socket has been closed
			return -1;
		}

		if (res < 0) {
			// An error occured
			// FIXME!
			return 0;
		}

		// Some bytes were read
		mReadBufferPos += res;
 
  
		return 0;
	}

	unsigned char*
	Client::getReadBuffer(int* numBytes)
	{
		*numBytes = mReadBufferPos;
		return mReadBuffer;
	}

	int
	Client::Send(unsigned char* msg, int length)
	{
		memcpy(mWriteBuffer + mWriteBufferPos, msg, length);
		mWriteBufferPos += length;

		int sent = Platform::instance()->WriteSocket(mSocket, mWriteBuffer, mWriteBufferPos);

		if (sent > 0) {
			if (mWriteBufferPos != sent) {
				// Adjust buffer
				memmove(mWriteBuffer, mWriteBuffer + sent, CONN_BUFF_SIZE - sent);
				mWriteBufferPos -= sent;
			}
			else {
				mWriteBufferPos = 0;
			}
		}

		Trace::instance()->trace(TRACE_LEVEL_INFO, __FILE__, __LINE__,
										 "Sent %d bytes (%d left) errno=%d", sent, mWriteBufferPos, errno);
		return 0;
	}

	void
	Client::Flush()
	{
		if (mWriteBufferPos == 0) {
			return;
		}

		
		int sent = Platform::instance()->WriteSocket(mSocket, mWriteBuffer, mWriteBufferPos);

		Trace::instance()->trace(TRACE_LEVEL_INFO, __FILE__, __LINE__,
										 "Flushed %d bytes", sent);
		
		if (sent > 0) {
			if (mWriteBufferPos != sent) {
				// Adjust buffer
				memmove(mWriteBuffer, mWriteBuffer + sent, CONN_BUFF_SIZE - sent);
				mWriteBufferPos -= sent;
			}
			else {
				mWriteBufferPos = 0;
			}
		}

		
	} 
	

} // namespace TorpClass
