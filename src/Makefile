#==============================================================================
#  Copyright (c) 2013  Peter R. Torpman (peter at torpman dot se)
#
#  This file is part of TorpClass (http://torpclass.sourceforge.net)
#
#  TorpClass is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation; either version 3 of the License, or
#  (at your option) any later version.
#
#  TorpClass is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program.  If not, see <http://www.gnu.or/licenses/>.
#==============================================================================
#
# To build for Linux, just type 'make'
# 
# To build for Windows, type 'make win=on'
#
#
# -DLINUX is used for building for Linux.
# -DWINDOWS is used for building for Windows.
#
#==============================================================================


# FLAGS
#------------------------------------------------------------------------------
ifneq ($(debug), off)
  CFLAGS += -g
else 
  CFLAGS += -O2
endif


ifeq ($(win), on)
  CPPFLAGS = -DWINDOWS -D_WIN32 -I/usr/x86_64-w64-mingw32/include
  CFLAGS += -DHAVE_W32API_H -O2
  CXX = i586-mingw32msvc-g++ -m32
  arch=win32
  LIBS = -lws2_32
else
  CPPFLAGS = -DLINUX
  CXX = g++
  CFLAGS += -Wall -Werror -Wno-write-strings 
  arch=x86
  LIBS = -lstdc++ -lrt
endif



# SOURCES
#------------------------------------------------------------------------------

SRC = \
	Address.cpp				\
	Client.cpp				\
	ConnManager.cpp			\
	Platform.cpp			\
	Platform_Linux.cpp		\
	Platform_Windows.cpp	\
	Server.cpp				\
	SocketManager.cpp		\
	StackTrace.cpp			\
	Timer.cpp				\
	TimerManager.cpp		\
	TorpClass.cpp 			\
	Trace.cpp

OBJ = $(SRC:.cpp=_$(arch).o)

HEADERS := $(shell ls *.h)

# RESULTS
#------------------------------------------------------------------------------
SERV     = test_server
SERV_SRC = test_server.cpp
SERV_OBJ = $(SERV_SRC:.cpp=_$(arch).o)

CLI     = test_client
CLI_SRC = test_client.cpp
CLI_OBJ = $(CLI_SRC:.cpp=_$(arch).o)


# TARGETS
#------------------------------------------------------------------------------
#$(OBJ): $(HEADERS)


all: $(OBJ) $(SERV) $(CLI)

$(SERV): $(SERV_OBJ) $(OBJ)
	$(CXX) $(CFLAGS) $(CPPFLAGS) $^ $(LIBS) -o $@

$(CLI): $(CLI_OBJ) $(OBJ)
	$(CXX) $(CFLAGS) $(CPPFLAGS) $^ $(LIBS) -o $@



clean:
	@rm -f $(OBJ) *~ $(SERV) $(CLI)

%_$(arch).o: %.cpp
	$(CXX) $(CFLAGS) $(CPPFLAGS) -c $^ -o $@ 
