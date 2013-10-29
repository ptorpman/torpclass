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

#include "Trace.h"

#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>

namespace TorpClass {

   static const char* traceStrings[] = {
      "INFO",
      "WARNING",
      "ERROR",
      "FATAL",
      "ASSERT",
      "TRACE"
   };
   
	///-------------------------------------------------------------------------
	/// FUNCTION DEFINITIONS
	///-------------------------------------------------------------------------
	void
	Trace::trace(TraceLevel level, const char* file, int line,
                 const char* format, ...)
	{
		if (level > mLevel) {
			// Should not be printed
			return;
		}


		char tmp[2048];
		char tmp2[0xFF];
		va_list args;

		va_start(args, format);

		int len = vsprintf(tmp, format, args);

		(void) len;
  
		va_end(args);

		sprintf(tmp2, "%s:%d", file, line);
  
		fprintf(stderr, "%-7s :%-25s: %s\n", traceStrings[level], tmp2, tmp);
        fflush(stderr);
	}

} // namespace TorpClass
