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
#include "IPlatformSocket.h"
#include "Trace.h"

#ifdef LINUX

#include <errno.h>
#include <time.h>
#include <signal.h>
///-----------------------------------------------------------------------------
/// FUNCTION DEFINITIONS
///-----------------------------------------------------------------------------
namespace TorpClass {

   int
   Platform::GetIpFromString(char* str, int addrType, void* addr)
   {
      if (addrType == AF_INET) {
         struct sockaddr_in* inAddr = (struct sockaddr_in*) addr;
        
         int res = inet_pton(addrType, str, &(inAddr->sin_addr));

         if (res == 1) {
            // Success!
            return 0;
         }

         // Fail
         return 1;
      }

      if (addrType == AF_INET6) {
         struct sockaddr_in6* inAddr = (struct sockaddr_in6*) addr;
         int res = inet_pton(addrType, str, &(inAddr->sin6_addr));

         if (res == 1) {
            // Success!
            return 0;
         }

         // Fail
         return 1;
      }
     

     
      // Not supported
      return 1;
   }

   int
   Platform::SetNonBlock(SOCKET sd)
   {
      int flags = fcntl(sd, F_GETFL, 0);

      if (flags < 0) {
         return 1;
      }

      flags = (flags | O_NONBLOCK);

      return fcntl(sd, F_SETFL, flags);
   }


   int
   Platform::PollSockets(int msecTimeout)
   {
      int res = 0;

      if (mNumToPoll == 0) {
         // Nothing to to sleep the timeout, then call the TimerManager callback
         // to let it check timers...
         if (mTimeoutCallback) {
            struct timespec ts = { msecTimeout / 1000,
                                   msecTimeout * 1000 };
            nanosleep(&ts, NULL);
            
            mTimeoutCallback();
         }
         return 0;
      }
      

      res = poll(mPollFD, mNumToPoll, msecTimeout);
      
      // Let TimerManager check its timers
      if (mTimeoutCallback) {
         mTimeoutCallback();
      }
      
      if (res <= 0) {
         // Error or timeout. Let TimerManager check its timers
         return 1;
      }
      
      for (int i = 0; i < mNumToPoll; i++) {
         SocketHelper_t* h = mSocketMap[mPollFD[i].fd];

         if ((mPollFD[i].revents & POLLIN) != POLLIN) {
            // Nothing for us
            continue;
         }
         
         // Something happened.. Call upper layer
         
         Trace::instance()->trace(TRACE_LEVEL_TRACE, __FILE__, __LINE__,
                                  "Stuff on %d", h->sd);
         
         h->callback(mPollFD[i].fd, h->type);
      }
  
  
      return 0;
   }

   int
   Platform::CloseSocket(SOCKET sd)
   {
      return close(sd);
   }

   void
   Platform::rebuildPollStruct()
   {
      map<SOCKET, SocketHelper_t*>::iterator i;

      mNumToPoll = 0;
      
      for (i = mSocketMap.begin(); i != mSocketMap.end(); ++i) {
         mPollFD[mNumToPoll].fd = i->first;
         mPollFD[mNumToPoll].events = POLLIN;
    
         mNumToPoll++;
      }
   }

   SOCKET
   Platform::AcceptSocket(SOCKET sd, struct sockaddr* addr, int* addrLen)
   {
      socklen_t* len = (socklen_t*) addrLen;

      return accept(sd, addr, len);
   }

   int
   Platform::ReadSocket(SOCKET sd, unsigned char* buffer, unsigned int length)
   {
      int retry = 4;
      int res = 0;
      int curr = 0;

      while (retry > 0) {
         TorpClass::Trace::instance()->trace(TRACE_LEVEL_TRACE, __FILE__, __LINE__, "Try to read bytes %d", length - curr);
         res = read(sd, buffer + curr, length - curr);

         if (res < 0) {
            switch (errno) {
               case EFAULT:
               case EINVAL:
               case EBADF:
               case ENOTSOCK:
               case ECONNRESET:
               case EPIPE:
                  // Socket is broken!
                  return -1;
               default:
                  // Try again!
                  TorpClass::Trace::instance()->trace(TRACE_LEVEL_TRACE, __FILE__, __LINE__, "Read failed! errno=%d", errno);
                  retry--;
                  break;
            }

            continue;
         }

         if (res == 0) {
            TorpClass::Trace::instance()->trace(TRACE_LEVEL_TRACE, __FILE__, __LINE__, "Socket is closed! errno=%d", errno);
            // Connection has been closed down
            return 0;
         }

         // We managed to read something
         TorpClass::Trace::instance()->trace(TRACE_LEVEL_TRACE, __FILE__, __LINE__, "Read %d bytes!", res);
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
         res = write(sd, buffer + curr, length - curr);

         if (res < 0) {
            switch (errno) {
               case EFAULT:
               case EINVAL:
               case EBADF:
               case ENOTSOCK:
               case ECONNRESET:
               case EPIPE:
               case ECONNREFUSED:
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

      if (res < 0 && errno == EINPROGRESS) {
         // Make sure the connection is up by checking if socket is writable
         struct pollfd fd[1];
			
         fd[0].fd = sd;
         fd[0].events = POLLOUT;
			
         res = poll(fd, 1, 1000); // Wait one second max.
			
         if (!(res == 1 && ((fd[0].revents & POLLOUT) == POLLOUT))) {
            // Could not connect. 
            return -1;
         }

         // Connection is up!
         return 0;
      }

      return res;
   }


   uint64_t
   Platform::GetTime()
   {
      struct timespec t;
      clock_gettime(CLOCK_REALTIME, &t);

      return (t.tv_sec * 1000 + (t.tv_nsec / 1000000));
   }


   
} // namespace TorpClass

#endif  // LINUX
