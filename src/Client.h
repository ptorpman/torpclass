#ifndef CLIENT_H_
#define CLIENT_H_
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

namespace TorpClass {

#define CONN_BUFF_SIZE 0xFFFF

	// Predeclaration
	class Address;


	class Client
	{
	public:
		/// Constructor
		Client(Address* addr);
		/// Destructor
		~Client();

		/// Starts client connection to server
		int Start();
		/// Stops client connection to server
		void Stop();
		/// Sends a message to the server
		int Send(unsigned char* msg, int length);
		/// Flush any messages that might be left
		void Flush();

		
		
		
		/// Handle socket events
		/// @return 0 if OK, -1 if connection should be removed,
		int HandleSocket();

		/// Return read buffer
		unsigned char* getReadBuffer(int* numBytes);

		/// Set new position in read buffer
		void setReadBufferPos(int pos) { mReadBufferPos = pos; }
		
		/// Returns the socket
		SOCKET getSocket() { return mSocket; }
		/// Sets the socket
		void setSocket(SOCKET s) { mSocket = s; }
		
  
	private:
		/// Remote address for client (i.e the remote server address)
		Address* mAddress;
		

		/// The socket
		SOCKET mSocket;
		/// Read buffer
		unsigned char mReadBuffer[CONN_BUFF_SIZE];
		/// Read buffer position
		int mReadBufferPos;
		/// Write buffer
		unsigned char mWriteBuffer[CONN_BUFF_SIZE];
		/// Write buffer position
		int mWriteBufferPos;
  
	};


} // namespace TorpClass







#endif  // CLIENT_H_
