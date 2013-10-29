#ifndef ADDRESS_H_
#define ADDRESS_H_
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

	/// This class represents an address 
	class Address
	{
	public:
		/// Constructor
		Address(char* ipAddress, int port);
		/// Constructor (IPv4)
		Address(struct sockaddr_in* addr, int addrLen);
		/// Constructor (IPv6)
		Address(struct sockaddr_in6* addr, int addrLen);

		/// Destructor
		~Address();

		/// Returns if address is valid
		bool isValid() { return mIsValid; }

		/// Returns sockaddr
		struct sockaddr* getSockAddr();

		// Returns address length
		int getAddrLength() { return mLength; }

		/// Returns address family
		int getFamily() { return mFamily; }
		

	private:
		/// IP address in string format
		char mIpString[0xFF];
		/// Port number
		int mPortNum;
		/// Address family
		int mFamily;
		/// Address length
		int mLength;
	
		/// IP socket address (IPv4)
		struct sockaddr_in mIpAddress4;
		/// IP socket address (IPv6)
		struct sockaddr_in6 mIpAddress6;

		/// Flag if valid address
		bool mIsValid;
	};


} // namespace TorpClass







#endif  // CONNECTION_H_
