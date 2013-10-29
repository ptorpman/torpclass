#ifndef PLATFORM_H
#define PLATFORM_H
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

#ifdef WINDOWS
#include "Platform_Windows.h"
#endif

#ifdef LINUX
#include "Platform_Linux.h"
#endif

#include "IPlatformSocket.h"
#include "IPlatformTimer.h"

#include <map>
using namespace std;


///-----------------------------------------------------------------------------
/// PLATFORM INDEPENDENT TYPES
///-----------------------------------------------------------------------------

///-----------------------------------------------------------------------------
/// PLATFORM INDEPENDENT FUNCTIONS
///-----------------------------------------------------------------------------

namespace TorpClass {

   // Socket Helper struct
   typedef struct SocketHelper_t {
      /// Socket descriptor
      SOCKET sd;
      /// Socket type
      SocketType type;
      /// Socket callback
      void (*callback)(SOCKET sd, SocketType type);
   } SocketHelper_t;
   
   
   class Platform : public IPlatformSocket, public IPlatformTimer
   {
     public:
		/// Singleton instance 
		static Platform* instance()
		{
			static Platform* inst = new Platform();
			return inst;
		}

      /// Initialize
      int Init();
        
		//----------------------------------------------------------------------
		// IPlatformSocket methods
		//----------------------------------------------------------------------
		int GetIpFromString(char* str, int addrType, void* addr); 
		int SetNonBlock(SOCKET sd);
		int AddSocketForPolling(SOCKET sd, SocketType type,
                              void (*callback)(SOCKET sd, SocketType t));
		int RemoveSocketFromPolling(SOCKET sd);
		int PollSockets(int msecTimeout);
		int CloseSocket(SOCKET sd);
		SOCKET AcceptSocket(SOCKET sd, struct sockaddr* addr, int* addrLen);
		int ReadSocket(SOCKET sd, unsigned char* buffer, unsigned int length);
		int WriteSocket(SOCKET sd, unsigned char* buffer, unsigned int length);
		int ConnectSocket(SOCKET sd, struct sockaddr* addr, int addrLen);
  
		//----------------------------------------------------------------------
		// IPlatformTimer methods
		//----------------------------------------------------------------------
      uint64_t GetTime();
      void RegisterTimeoutCallback(void (*callback)(void));
      
      // Calls the tick timer callback
      void callTickCallback();

     private:

		/// Default constructor 

		Platform() {};
      Platform(Platform const&); // Don't implement
      void operator=(Platform const&); // Don't implement            
		/// Default destructor 
		virtual ~Platform() {}

      /// Timeout callback
      GenericCallback mTimeoutCallback;

#ifdef LINUX
		/// Poll structure
		pollfd mPollFD[1024];
      /// Tick Timer
      timer_t mTickTimer;
#endif

#ifdef WINDOWS
		fd_set mSetRead;
		fd_set mSetWrite;
#endif

		/// Number of sockets to poll
		int mNumToPoll;           
  
		/// Map of all sockets
      map<SOCKET, SocketHelper_t*> mSocketMap;

		// Rebuild sockets for polling
		void rebuildPollStruct();

	};

} // namespace TorpClass
#endif  /* PLATFORM_H */
