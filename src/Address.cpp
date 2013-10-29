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

#include "Address.h"

namespace TorpClass {

	Address::Address(char* ipAddress, int port)
	{
		mIsValid = true;

		if (!(ipAddress == NULL || port < 0 || port > 0xFFFF)) {
			memset(&mIpAddress4, 0, sizeof(struct sockaddr_in));
			memset(&mIpAddress6, 0, sizeof(struct sockaddr_in6));

			
			int res = Platform::instance()->GetIpFromString(ipAddress,
                                                         AF_INET, &mIpAddress4);

			if (res == 0) {
				mFamily = AF_INET;
				mIpAddress4.sin_family = mFamily;
				mIpAddress4.sin_port = htons(port);
				strcpy(mIpString, ipAddress);
				mPortNum = port;
				mLength = sizeof(mIpAddress4);
			}
			else {
            // See if it is an IPv6 address
            res = Platform::instance()->GetIpFromString(ipAddress,
                                                        AF_INET6, &mIpAddress6);

            if (res == 0) {
               mFamily = AF_INET6;
               mIpAddress6.sin6_family = mFamily;
               mIpAddress6.sin6_port = htons(port);
               strcpy(mIpString, ipAddress);
               mPortNum = port;
               mLength = sizeof(mIpAddress6);
            }
            else {
               mIsValid = false;
            }
			}
		}
		else {
			mIsValid = false;
		}
	}

	Address::Address(struct sockaddr_in* addr, int addrLen)
	{
		memcpy(&mIpAddress4, addr, addrLen);

		mFamily = AF_INET;
		mLength = addrLen;
		mPortNum = ntohs(addr->sin_port);

		mIsValid = true;
	}
   
   Address::Address(struct sockaddr_in6* addr, int addrLen)
   {
		memcpy(&mIpAddress6, addr, addrLen);

		mFamily = AF_INET6;
		mLength = addrLen;
		mPortNum = ntohs(addr->sin6_port);

		mIsValid = true;
   }



	Address::~Address()
	{
	}

	struct sockaddr*
	Address::getSockAddr()
	{
		if (mFamily == AF_INET) {
			return (struct sockaddr*)&mIpAddress4;
		}
		else {
			return (struct sockaddr*)&mIpAddress6;
		}
	}
	

} // namespace TorpClass
