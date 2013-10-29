#ifndef IPLATFORM_SOCKET_H
#define IPLATFORM_SOCKET_H
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


#include "Types.h"

///-----------------------------------------------------------------------------
/// INTERFACE USED FOR PLATFORM INDEPENDENCY
///-----------------------------------------------------------------------------
class IPlatformSocket
{
public:
  
  /// Returns an IP address from a string
  /// @param str        IP address string
  /// @param addrType   IP address type, AF_INET or AF_INET6
  /// @param addr       Pointer to address which will be returned, either 'struct
  ///                   sockaddr_in*' or 'struct sockaddr_in6*'
  /// @return 0 if OK, error code if not
  virtual int GetIpFromString(char* str, int addrType, void* addr) = 0; 

  /// Sets a socket as non-blocking
  /// @return 0 if OK, error code if not
  virtual int SetNonBlock(SOCKET sd) = 0;

  /// Perform polling
  /// @param msecTimeout    Socket timeout in milliseconds
  /// @return 0 if OK, error code if not
  virtual int PollSockets(int msecTimeout) = 0;

  /// Close a socket
  /// @param sd       Socket descriptor
  virtual int CloseSocket(SOCKET sd) = 0;
  
  /// Accept a connection on a socket
  /// @param sd       Socket descriptor
  /// @param addr     Socket address
  /// @param addrLen  Socket address length
  virtual SOCKET AcceptSocket(SOCKET sd, struct sockaddr* addr, int* addrLen) = 0;

  /// Read from a socket
  /// @param sd       Socket descriptor
  /// @param buffer   Pointer to read buffer
  /// @param length   Number of bytes available in buffer
  /// @return Number of bytes read, 0 if connection closed, -1 if socket error
  virtual int ReadSocket(SOCKET sd, unsigned char* buffer, unsigned int length) = 0;
  
  /// Write to a socket
  /// @param sd       Socket descriptor
  /// @param buffer   Pointer to write buffer
  /// @param length   Number of bytes to write
  /// @return Number of bytes written
  virtual int WriteSocket(SOCKET sd, unsigned char* buffer, unsigned int length) = 0;


  /// Connect a socket
  /// @param sd       Socket descriptor
  /// @param addr     Socket address
  /// @param addrLen  Socket address length
  /// @return 0 if OK, error code if not
  virtual int ConnectSocket(SOCKET sd, struct sockaddr* addr, int addrLen) = 0;

};





#endif // IPLATFORM_SOCKET_H
