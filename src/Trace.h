#ifndef TRACE_H
#define TRACE_H
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

   /// Trace levels
   typedef enum {
      TRACE_LEVEL_INFO    = 0,
      TRACE_LEVEL_WARNING = 1,
      TRACE_LEVEL_ERROR   = 2,
      TRACE_LEVEL_FATAL   = 3,
      TRACE_LEVEL_ASSERT  = 4,
      TRACE_LEVEL_TRACE   = 5
   } TraceLevel;

   ///
   /// This class implements the Trace and Debug singleton
   ///
   class Trace
   {
     public:
      /// Singleton instance 
      static Trace* instance()
      {
         static Trace* inst = new Trace();
         return inst;
      }

      /// Set trace level
      void setLevel(TraceLevel l) { mLevel = l; }

      /// Print trace message
      void trace(TraceLevel level, const char* file, int line, const char* format, ...);

   
     private:
      /// Default constructor 
      Trace() { mLevel = TRACE_LEVEL_ERROR; }
      /// Default destructor 
      ~Trace() { }

      /// Trace level
      TraceLevel mLevel;
   };


} // namespace TorpClass

#endif  // TRACE_H
