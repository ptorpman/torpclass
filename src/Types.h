#ifndef TYPES_H
#define TYPES_H
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

namespace TorpClass {

   /// Predeclaration
   class Timer;
   
   /// Type for timer callbacks
   typedef void (*TimerCallback)(Timer* t, void* userData);
   
   /// Generic Callback
   typedef void (*GenericCallback)(void);
  
	/// Connection manager types
	typedef enum {
		SOCKET_TYPE_CLIENT = 0,
		SOCKET_TYPE_SERVER = 1
	} SocketType;




} // namespace TorpClass
 
#endif // TYPES_H
