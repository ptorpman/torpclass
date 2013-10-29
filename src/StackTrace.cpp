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
// Thanks to jschmier at stackoverflow.com for inspiration!
//
#include "StackTrace.h"

#ifdef LINUX

#include <execinfo.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ucontext.h>
#include <unistd.h>
#include <cxxabi.h>

namespace TorpClass {
   
   

   //-------------------------------------------------------------------
   // VARIABLES
   //-------------------------------------------------------------------
   // This structure mirrors the one found in /usr/include/asm/ucontext.h 
   typedef struct _sig_ucontext {
      unsigned long     uc_flags;
      struct ucontext   *uc_link;
      stack_t           uc_stack;
      struct sigcontext uc_mcontext;
      sigset_t          uc_sigmask;
   } sig_ucontext_t;

   //-------------------------------------------------------------------
   // FUNCTIONS
   //-------------------------------------------------------------------
   static void crit_err_hdlr(int sig_num, siginfo_t * info, void * ucontext);

   void InstallStackTrace()
   {
      struct sigaction sigact;

      sigact.sa_sigaction = crit_err_hdlr;
      sigact.sa_flags = SA_RESTART | SA_SIGINFO;
      
      if (sigaction(SIGSEGV, &sigact, (struct sigaction *)NULL) != 0) {
         fprintf(stderr, "error setting signal handler for %d (%s)\n",
                 SIGSEGV, strsignal(SIGSEGV));      
      }
   }

   void
   crit_err_hdlr(int sig_num, siginfo_t * info, void * ucontext)
   {
      sig_ucontext_t * uc = (sig_ucontext_t *)ucontext;

#ifdef _LP64
      void * caller_address = (void *) uc->uc_mcontext.rip; // x86_64 specific
#else
      void * caller_address = (void *) uc->uc_mcontext.eip; // x86 specific
#endif
      
      fprintf(stderr, "\nsignal %d (%s), address is 0x%p from 0x%p\n",
              sig_num, strsignal(sig_num), info->si_addr, caller_address);
      

      void * array[50];
      int size = backtrace(array, 50);

      array[1] = caller_address;

      char ** messages = backtrace_symbols(array, size);    

      // skip first stack frame (points here)
      for (int i = 1; i < size && messages != NULL; ++i)
      {
         char *mangled_name = 0, *offset_begin = 0, *offset_end = 0;

         // find parantheses and +address offset surrounding mangled name
         for (char *p = messages[i]; *p; ++p)
         {
            if (*p == '(') 
            {
               mangled_name = p; 
            }
            else if (*p == '+') 
            {
               offset_begin = p;
            }
            else if (*p == ')')
            {
               offset_end = p;
               break;
            }
         }

         // if the line could be processed, attempt to demangle the symbol
         if (mangled_name && offset_begin && offset_end && 
             mangled_name < offset_begin)
         {
            *mangled_name++ = '\0';
            *offset_begin++ = '\0';
            *offset_end++ = '\0';

            int status;
            char * real_name = abi::__cxa_demangle(mangled_name, 0, 0, &status);

            // if demangling is successful, output the demangled function name
            if (status == 0)
            {    
               fprintf(stderr, "[bt]: (%d) %s : %s+%s [%s]\n",
                       i, messages[i], real_name, offset_begin, offset_end);

            }
            // otherwise, output the mangled function name
            else
            {
               fprintf(stderr, "[bt]: (%d) %s : %s+%s [%s]\n",
                       i, messages[i], mangled_name, offset_begin, offset_end);
            }
            free(real_name);
         }
         // otherwise, print the whole line
         else
         {
            fprintf(stderr, "[bt]: (%d) %s\n",  i, messages[i]);
         }
      }
      fprintf(stderr, "\n");

      free(messages);
      exit(1);
   }
   
   

} // namespace TorpClass
#endif  // LINUX

  
#ifdef WINDOWS
namespace TorpClass {

   void InstallStackTrace()
   {
      // FIXME!
   }

} // namespace TorpClass
#endif  // WINDOWS


