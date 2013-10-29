#ifndef SERVER_H_
#define SERVER_H_
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


	/// This class represents a server.
	class Server
	{
	public:
		/// Constructor
		Server(Address* addr);
		/// Destructor
		~Server();

		/// Starts the server socket
		/// @return 0 if OK, error code if not
		int Start();

		/// Stops the server socket
		void Stop();

		/// Returns if server is operational
		bool isOperational() { return mOperational; }

		/// Returns the socket
		SOCKET getSocket() { return mSocket; }
		
		/// Handle socket events
		/// @return 0 if OK, -1 if connection should be removed,
		int HandleSocket();

		/// Return read buffer
		unsigned char* getReadBuffer(int* numBytes);

		/// Flush any messages that might be left
		void Flush();

		
  
	private:
		/// Server address
		Address* mAddress;
		/// Flag if server is operational
		bool mOperational;

		
		/// The socket
		SOCKET mSocket;
		/// Read buffer
		unsigned char mReadBuffer[CONN_BUFF_SIZE];
		/// Read buffer position
		int mReadBufferPos;
  
	};


} // namespace TorpClass







#endif  // SERVER_H_
