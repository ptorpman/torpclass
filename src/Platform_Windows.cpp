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
//
// Thanks to Carl Staelin for clock_gettime implementation (stackoverflow.com)
//



#include "Platform.h"

#ifdef WINDOWS


namespace TorpClass {

	///-----------------------------------------------------------------------------
	/// FUNCTION DEFINITIONS
	///-----------------------------------------------------------------------------

	int
	Platform::GetIpFromString(char* str, int addrType, void* addr)
	{
		int sz = sizeof(struct sockaddr_in);
  
		return WSAStringToAddress(str, addrType, NULL, (struct sockaddr*) addr, &sz);
	}

	int
	Platform::SetNonBlock(SOCKET sd)
	{
		unsigned long mode = 1;
		return ioctlsocket(sd, FIONBIO, &mode);
	}

	int
	Platform::PollSockets(int msecTimeout)
	{
		if (mNumToPoll == 0) {
         // Nothing to to sleep the timeout, then call the TimerManager callback
         // to let it check timers...
         if (mTimeoutCallback) {
            Sleep(msecTimeout);
            
            mTimeoutCallback();
         }
         return 1;
		}

		struct timeval tv = { 0, msecTimeout * 1000 };

		int res = select(mNumToPoll, &mSetRead, &mSetWrite, NULL, &tv);

      // Let TimerManager check its timers
      if (mTimeoutCallback) {
         mTimeoutCallback();
      }
      
		if (res == 0) {
			// No socket activity. 
			return 0;
		}

		map<SOCKET, SocketHelper_t*>::iterator i;

		for (i = mSocketMap.begin(); i != mSocketMap.end(); ++i) {
			if (FD_ISSET(i->first, &mSetRead)) {
				// Socket activity!
				// Something happened.. Call upper layer
				SocketHelper_t* h = mSocketMap[i->first];
				h->callback(i->first, h->type);
			}
		}
  
		return 0;
	}

	int
	Platform::CloseSocket(SOCKET sd)
	{
		return closesocket(sd);
	}

	void
	Platform::rebuildPollStruct()
	{
		map<SOCKET, SocketHelper_t*>::iterator i;

		FD_ZERO(&mSetRead);
		FD_ZERO(&mSetWrite);

		mNumToPoll = 0;
  
		for (i = mSocketMap.begin(); i != mSocketMap.end(); ++i) {
			FD_SET(i->first, &mSetRead);
			FD_SET(i->first, &mSetWrite);
    
			mNumToPoll++;
		}
	}

	SOCKET
	Platform::AcceptSocket(SOCKET sd, struct sockaddr* addr, int* addrLen)
	{
		return accept(sd, addr, addrLen);
	}

	int
	Platform::ReadSocket(SOCKET sd, unsigned char* buffer, unsigned int length)
	{
		int retry = 4;
		int res = 0;
		int curr = 0;

		while (retry > 0) {
			res = recv(sd, (char*)(buffer + curr), length - curr, 0);

			if (res < 0) {
				switch (errno) {
				case WSAEFAULT:
				case WSAEINVAL:
				case EBADF:
				case WSAENOTSOCK:
					// Socket is broken!
					return -1;
				default:
					// Try again!
					retry--;
					break;
				}

				continue;
			}

			if (res == 0) {
				// Connection has been closed down
				return 0;
			}

			// We managed to read something
			return res;
		}

		return curr;
	}

	int
	Platform::WriteSocket(SOCKET sd, unsigned char* buffer, unsigned int length)
	{
		int retry = 4;
		int res = 0;
		int curr = 0;
  
		while (retry > 0) {
			res = send(sd, (char*)(buffer + curr), length - curr, 0);

			if (res < 0) {
				switch (errno) {
				case WSAEFAULT:
				case WSAEINVAL:
				case EBADF:
				case WSAENOTSOCK:
				case WSAECONNRESET:
				case EPIPE:
					// Socket is broken!
					return -1;
				default:
					// Try again!
					retry--;
					break;
				}

				continue;
			}

			if (res == 0) {
				// Could not write anything, but no error
				retry--;
				continue;
			}
    
			// We could write
			curr += res;
      
			if (curr >= (int) length) {
				// We have written everything
				return length;
			}
		}

		// Return number of bytes we could write
		return curr;
	}


	int
	Platform::ConnectSocket(SOCKET sd, struct sockaddr* addr, int addrLen)
	{
		int res = connect(sd, addr, addrLen);
		int lastErr = WSAGetLastError();
		
		if (res < 0 && lastErr == WSAEINPROGRESS) {
			// Make sure the connection is up by checking if socket is writable
        struct timeval tv;
        fd_set fds;

        FD_ZERO(&fds);

        tv.tv_sec = 1;			  // Wait one second max!
        tv.tv_usec = 0;

        res = select(1, NULL, &fds, NULL, &tv);

		  if (res < 0) {
			  // Connection is not up!
			  return res;
		  }

		  // Connection is up!
		  return 0;
		}

		return res;
	}

   uint64_t
   Platform::GetTime()
   {
      
   }


   LARGE_INTEGER
   getFILETIMEoffset()
   {
      SYSTEMTIME s;
      FILETIME f;
      LARGE_INTEGER t;

      s.wYear = 1970;
      s.wMonth = 1;
      s.wDay = 1;
      s.wHour = 0;
      s.wMinute = 0;
      s.wSecond = 0;
      s.wMilliseconds = 0;
      SystemTimeToFileTime(&s, &f);
      t.QuadPart = f.dwHighDateTime;
      t.QuadPart <<= 32;
      t.QuadPart |= f.dwLowDateTime;
      return (t);
   }

   int
   clock_gettime(int X, struct timeval *tv)
   {
      LARGE_INTEGER           t;
      FILETIME            f;
      double                  microseconds;
      static LARGE_INTEGER    offset;
      static double           frequencyToMicroseconds;
      static int              initialized = 0;
      static BOOL             usePerformanceCounter = 0;

      if (!initialized) {
         LARGE_INTEGER performanceFrequency;
         initialized = 1;
         usePerformanceCounter = QueryPerformanceFrequency(&performanceFrequency);
         if (usePerformanceCounter) {
            QueryPerformanceCounter(&offset);
            frequencyToMicroseconds = (double)performanceFrequency.QuadPart / 1000000.;
         } else {
            offset = getFILETIMEoffset();
            frequencyToMicroseconds = 10.;
         }
      }
      if (usePerformanceCounter) QueryPerformanceCounter(&t);
      else {
         GetSystemTimeAsFileTime(&f);
         t.QuadPart = f.dwHighDateTime;
         t.QuadPart <<= 32;
         t.QuadPart |= f.dwLowDateTime;
      }

      t.QuadPart -= offset.QuadPart;
      microseconds = (double)t.QuadPart / frequencyToMicroseconds;
      t.QuadPart = microseconds;
      tv->tv_sec = t.QuadPart / 1000000;
      tv->tv_usec = t.QuadPart % 1000000;
      return (0);
   }

   
   
   

} // namespace TorpClass
#endif  // WINDOWS

