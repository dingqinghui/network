# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list

# Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The program to use to edit the cache.
CMAKE_EDIT_COMMAND = /usr/bin/ccmake

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /mnt/hgfs/linux_share/redis-net2.0

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /mnt/hgfs/linux_share/redis-net2.0

# Include any dependencies generated for this target.
include server/CMakeFiles/server.dir/depend.make

# Include the progress variables for this target.
include server/CMakeFiles/server.dir/progress.make

# Include the compile flags for this target's objects.
include server/CMakeFiles/server.dir/flags.make

server/CMakeFiles/server.dir/__/anet/src/connction.c.o: server/CMakeFiles/server.dir/flags.make
server/CMakeFiles/server.dir/__/anet/src/connction.c.o: anet/src/connction.c
	$(CMAKE_COMMAND) -E cmake_progress_report /mnt/hgfs/linux_share/redis-net2.0/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object server/CMakeFiles/server.dir/__/anet/src/connction.c.o"
	cd /mnt/hgfs/linux_share/redis-net2.0/server && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/server.dir/__/anet/src/connction.c.o   -c /mnt/hgfs/linux_share/redis-net2.0/anet/src/connction.c

server/CMakeFiles/server.dir/__/anet/src/connction.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/server.dir/__/anet/src/connction.c.i"
	cd /mnt/hgfs/linux_share/redis-net2.0/server && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /mnt/hgfs/linux_share/redis-net2.0/anet/src/connction.c > CMakeFiles/server.dir/__/anet/src/connction.c.i

server/CMakeFiles/server.dir/__/anet/src/connction.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/server.dir/__/anet/src/connction.c.s"
	cd /mnt/hgfs/linux_share/redis-net2.0/server && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /mnt/hgfs/linux_share/redis-net2.0/anet/src/connction.c -o CMakeFiles/server.dir/__/anet/src/connction.c.s

server/CMakeFiles/server.dir/__/anet/src/connction.c.o.requires:
.PHONY : server/CMakeFiles/server.dir/__/anet/src/connction.c.o.requires

server/CMakeFiles/server.dir/__/anet/src/connction.c.o.provides: server/CMakeFiles/server.dir/__/anet/src/connction.c.o.requires
	$(MAKE) -f server/CMakeFiles/server.dir/build.make server/CMakeFiles/server.dir/__/anet/src/connction.c.o.provides.build
.PHONY : server/CMakeFiles/server.dir/__/anet/src/connction.c.o.provides

server/CMakeFiles/server.dir/__/anet/src/connction.c.o.provides.build: server/CMakeFiles/server.dir/__/anet/src/connction.c.o

server/CMakeFiles/server.dir/__/anet/src/epollmp.c.o: server/CMakeFiles/server.dir/flags.make
server/CMakeFiles/server.dir/__/anet/src/epollmp.c.o: anet/src/epollmp.c
	$(CMAKE_COMMAND) -E cmake_progress_report /mnt/hgfs/linux_share/redis-net2.0/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object server/CMakeFiles/server.dir/__/anet/src/epollmp.c.o"
	cd /mnt/hgfs/linux_share/redis-net2.0/server && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/server.dir/__/anet/src/epollmp.c.o   -c /mnt/hgfs/linux_share/redis-net2.0/anet/src/epollmp.c

server/CMakeFiles/server.dir/__/anet/src/epollmp.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/server.dir/__/anet/src/epollmp.c.i"
	cd /mnt/hgfs/linux_share/redis-net2.0/server && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /mnt/hgfs/linux_share/redis-net2.0/anet/src/epollmp.c > CMakeFiles/server.dir/__/anet/src/epollmp.c.i

server/CMakeFiles/server.dir/__/anet/src/epollmp.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/server.dir/__/anet/src/epollmp.c.s"
	cd /mnt/hgfs/linux_share/redis-net2.0/server && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /mnt/hgfs/linux_share/redis-net2.0/anet/src/epollmp.c -o CMakeFiles/server.dir/__/anet/src/epollmp.c.s

server/CMakeFiles/server.dir/__/anet/src/epollmp.c.o.requires:
.PHONY : server/CMakeFiles/server.dir/__/anet/src/epollmp.c.o.requires

server/CMakeFiles/server.dir/__/anet/src/epollmp.c.o.provides: server/CMakeFiles/server.dir/__/anet/src/epollmp.c.o.requires
	$(MAKE) -f server/CMakeFiles/server.dir/build.make server/CMakeFiles/server.dir/__/anet/src/epollmp.c.o.provides.build
.PHONY : server/CMakeFiles/server.dir/__/anet/src/epollmp.c.o.provides

server/CMakeFiles/server.dir/__/anet/src/epollmp.c.o.provides.build: server/CMakeFiles/server.dir/__/anet/src/epollmp.c.o

server/CMakeFiles/server.dir/__/anet/src/iomp.c.o: server/CMakeFiles/server.dir/flags.make
server/CMakeFiles/server.dir/__/anet/src/iomp.c.o: anet/src/iomp.c
	$(CMAKE_COMMAND) -E cmake_progress_report /mnt/hgfs/linux_share/redis-net2.0/CMakeFiles $(CMAKE_PROGRESS_3)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object server/CMakeFiles/server.dir/__/anet/src/iomp.c.o"
	cd /mnt/hgfs/linux_share/redis-net2.0/server && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/server.dir/__/anet/src/iomp.c.o   -c /mnt/hgfs/linux_share/redis-net2.0/anet/src/iomp.c

server/CMakeFiles/server.dir/__/anet/src/iomp.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/server.dir/__/anet/src/iomp.c.i"
	cd /mnt/hgfs/linux_share/redis-net2.0/server && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /mnt/hgfs/linux_share/redis-net2.0/anet/src/iomp.c > CMakeFiles/server.dir/__/anet/src/iomp.c.i

server/CMakeFiles/server.dir/__/anet/src/iomp.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/server.dir/__/anet/src/iomp.c.s"
	cd /mnt/hgfs/linux_share/redis-net2.0/server && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /mnt/hgfs/linux_share/redis-net2.0/anet/src/iomp.c -o CMakeFiles/server.dir/__/anet/src/iomp.c.s

server/CMakeFiles/server.dir/__/anet/src/iomp.c.o.requires:
.PHONY : server/CMakeFiles/server.dir/__/anet/src/iomp.c.o.requires

server/CMakeFiles/server.dir/__/anet/src/iomp.c.o.provides: server/CMakeFiles/server.dir/__/anet/src/iomp.c.o.requires
	$(MAKE) -f server/CMakeFiles/server.dir/build.make server/CMakeFiles/server.dir/__/anet/src/iomp.c.o.provides.build
.PHONY : server/CMakeFiles/server.dir/__/anet/src/iomp.c.o.provides

server/CMakeFiles/server.dir/__/anet/src/iomp.c.o.provides.build: server/CMakeFiles/server.dir/__/anet/src/iomp.c.o

server/CMakeFiles/server.dir/__/anet/src/netapi.c.o: server/CMakeFiles/server.dir/flags.make
server/CMakeFiles/server.dir/__/anet/src/netapi.c.o: anet/src/netapi.c
	$(CMAKE_COMMAND) -E cmake_progress_report /mnt/hgfs/linux_share/redis-net2.0/CMakeFiles $(CMAKE_PROGRESS_4)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object server/CMakeFiles/server.dir/__/anet/src/netapi.c.o"
	cd /mnt/hgfs/linux_share/redis-net2.0/server && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/server.dir/__/anet/src/netapi.c.o   -c /mnt/hgfs/linux_share/redis-net2.0/anet/src/netapi.c

server/CMakeFiles/server.dir/__/anet/src/netapi.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/server.dir/__/anet/src/netapi.c.i"
	cd /mnt/hgfs/linux_share/redis-net2.0/server && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /mnt/hgfs/linux_share/redis-net2.0/anet/src/netapi.c > CMakeFiles/server.dir/__/anet/src/netapi.c.i

server/CMakeFiles/server.dir/__/anet/src/netapi.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/server.dir/__/anet/src/netapi.c.s"
	cd /mnt/hgfs/linux_share/redis-net2.0/server && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /mnt/hgfs/linux_share/redis-net2.0/anet/src/netapi.c -o CMakeFiles/server.dir/__/anet/src/netapi.c.s

server/CMakeFiles/server.dir/__/anet/src/netapi.c.o.requires:
.PHONY : server/CMakeFiles/server.dir/__/anet/src/netapi.c.o.requires

server/CMakeFiles/server.dir/__/anet/src/netapi.c.o.provides: server/CMakeFiles/server.dir/__/anet/src/netapi.c.o.requires
	$(MAKE) -f server/CMakeFiles/server.dir/build.make server/CMakeFiles/server.dir/__/anet/src/netapi.c.o.provides.build
.PHONY : server/CMakeFiles/server.dir/__/anet/src/netapi.c.o.provides

server/CMakeFiles/server.dir/__/anet/src/netapi.c.o.provides.build: server/CMakeFiles/server.dir/__/anet/src/netapi.c.o

server/CMakeFiles/server.dir/__/anet/src/tcpclient.c.o: server/CMakeFiles/server.dir/flags.make
server/CMakeFiles/server.dir/__/anet/src/tcpclient.c.o: anet/src/tcpclient.c
	$(CMAKE_COMMAND) -E cmake_progress_report /mnt/hgfs/linux_share/redis-net2.0/CMakeFiles $(CMAKE_PROGRESS_5)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object server/CMakeFiles/server.dir/__/anet/src/tcpclient.c.o"
	cd /mnt/hgfs/linux_share/redis-net2.0/server && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/server.dir/__/anet/src/tcpclient.c.o   -c /mnt/hgfs/linux_share/redis-net2.0/anet/src/tcpclient.c

server/CMakeFiles/server.dir/__/anet/src/tcpclient.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/server.dir/__/anet/src/tcpclient.c.i"
	cd /mnt/hgfs/linux_share/redis-net2.0/server && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /mnt/hgfs/linux_share/redis-net2.0/anet/src/tcpclient.c > CMakeFiles/server.dir/__/anet/src/tcpclient.c.i

server/CMakeFiles/server.dir/__/anet/src/tcpclient.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/server.dir/__/anet/src/tcpclient.c.s"
	cd /mnt/hgfs/linux_share/redis-net2.0/server && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /mnt/hgfs/linux_share/redis-net2.0/anet/src/tcpclient.c -o CMakeFiles/server.dir/__/anet/src/tcpclient.c.s

server/CMakeFiles/server.dir/__/anet/src/tcpclient.c.o.requires:
.PHONY : server/CMakeFiles/server.dir/__/anet/src/tcpclient.c.o.requires

server/CMakeFiles/server.dir/__/anet/src/tcpclient.c.o.provides: server/CMakeFiles/server.dir/__/anet/src/tcpclient.c.o.requires
	$(MAKE) -f server/CMakeFiles/server.dir/build.make server/CMakeFiles/server.dir/__/anet/src/tcpclient.c.o.provides.build
.PHONY : server/CMakeFiles/server.dir/__/anet/src/tcpclient.c.o.provides

server/CMakeFiles/server.dir/__/anet/src/tcpclient.c.o.provides.build: server/CMakeFiles/server.dir/__/anet/src/tcpclient.c.o

server/CMakeFiles/server.dir/__/anet/src/tcpserver.c.o: server/CMakeFiles/server.dir/flags.make
server/CMakeFiles/server.dir/__/anet/src/tcpserver.c.o: anet/src/tcpserver.c
	$(CMAKE_COMMAND) -E cmake_progress_report /mnt/hgfs/linux_share/redis-net2.0/CMakeFiles $(CMAKE_PROGRESS_6)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object server/CMakeFiles/server.dir/__/anet/src/tcpserver.c.o"
	cd /mnt/hgfs/linux_share/redis-net2.0/server && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/server.dir/__/anet/src/tcpserver.c.o   -c /mnt/hgfs/linux_share/redis-net2.0/anet/src/tcpserver.c

server/CMakeFiles/server.dir/__/anet/src/tcpserver.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/server.dir/__/anet/src/tcpserver.c.i"
	cd /mnt/hgfs/linux_share/redis-net2.0/server && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /mnt/hgfs/linux_share/redis-net2.0/anet/src/tcpserver.c > CMakeFiles/server.dir/__/anet/src/tcpserver.c.i

server/CMakeFiles/server.dir/__/anet/src/tcpserver.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/server.dir/__/anet/src/tcpserver.c.s"
	cd /mnt/hgfs/linux_share/redis-net2.0/server && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /mnt/hgfs/linux_share/redis-net2.0/anet/src/tcpserver.c -o CMakeFiles/server.dir/__/anet/src/tcpserver.c.s

server/CMakeFiles/server.dir/__/anet/src/tcpserver.c.o.requires:
.PHONY : server/CMakeFiles/server.dir/__/anet/src/tcpserver.c.o.requires

server/CMakeFiles/server.dir/__/anet/src/tcpserver.c.o.provides: server/CMakeFiles/server.dir/__/anet/src/tcpserver.c.o.requires
	$(MAKE) -f server/CMakeFiles/server.dir/build.make server/CMakeFiles/server.dir/__/anet/src/tcpserver.c.o.provides.build
.PHONY : server/CMakeFiles/server.dir/__/anet/src/tcpserver.c.o.provides

server/CMakeFiles/server.dir/__/anet/src/tcpserver.c.o.provides.build: server/CMakeFiles/server.dir/__/anet/src/tcpserver.c.o

server/CMakeFiles/server.dir/main.c.o: server/CMakeFiles/server.dir/flags.make
server/CMakeFiles/server.dir/main.c.o: server/main.c
	$(CMAKE_COMMAND) -E cmake_progress_report /mnt/hgfs/linux_share/redis-net2.0/CMakeFiles $(CMAKE_PROGRESS_7)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object server/CMakeFiles/server.dir/main.c.o"
	cd /mnt/hgfs/linux_share/redis-net2.0/server && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/server.dir/main.c.o   -c /mnt/hgfs/linux_share/redis-net2.0/server/main.c

server/CMakeFiles/server.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/server.dir/main.c.i"
	cd /mnt/hgfs/linux_share/redis-net2.0/server && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /mnt/hgfs/linux_share/redis-net2.0/server/main.c > CMakeFiles/server.dir/main.c.i

server/CMakeFiles/server.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/server.dir/main.c.s"
	cd /mnt/hgfs/linux_share/redis-net2.0/server && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /mnt/hgfs/linux_share/redis-net2.0/server/main.c -o CMakeFiles/server.dir/main.c.s

server/CMakeFiles/server.dir/main.c.o.requires:
.PHONY : server/CMakeFiles/server.dir/main.c.o.requires

server/CMakeFiles/server.dir/main.c.o.provides: server/CMakeFiles/server.dir/main.c.o.requires
	$(MAKE) -f server/CMakeFiles/server.dir/build.make server/CMakeFiles/server.dir/main.c.o.provides.build
.PHONY : server/CMakeFiles/server.dir/main.c.o.provides

server/CMakeFiles/server.dir/main.c.o.provides.build: server/CMakeFiles/server.dir/main.c.o

# Object files for target server
server_OBJECTS = \
"CMakeFiles/server.dir/__/anet/src/connction.c.o" \
"CMakeFiles/server.dir/__/anet/src/epollmp.c.o" \
"CMakeFiles/server.dir/__/anet/src/iomp.c.o" \
"CMakeFiles/server.dir/__/anet/src/netapi.c.o" \
"CMakeFiles/server.dir/__/anet/src/tcpclient.c.o" \
"CMakeFiles/server.dir/__/anet/src/tcpserver.c.o" \
"CMakeFiles/server.dir/main.c.o"

# External object files for target server
server_EXTERNAL_OBJECTS =

bin/server: server/CMakeFiles/server.dir/__/anet/src/connction.c.o
bin/server: server/CMakeFiles/server.dir/__/anet/src/epollmp.c.o
bin/server: server/CMakeFiles/server.dir/__/anet/src/iomp.c.o
bin/server: server/CMakeFiles/server.dir/__/anet/src/netapi.c.o
bin/server: server/CMakeFiles/server.dir/__/anet/src/tcpclient.c.o
bin/server: server/CMakeFiles/server.dir/__/anet/src/tcpserver.c.o
bin/server: server/CMakeFiles/server.dir/main.c.o
bin/server: server/CMakeFiles/server.dir/build.make
bin/server: server/CMakeFiles/server.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking C executable ../bin/server"
	cd /mnt/hgfs/linux_share/redis-net2.0/server && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/server.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
server/CMakeFiles/server.dir/build: bin/server
.PHONY : server/CMakeFiles/server.dir/build

server/CMakeFiles/server.dir/requires: server/CMakeFiles/server.dir/__/anet/src/connction.c.o.requires
server/CMakeFiles/server.dir/requires: server/CMakeFiles/server.dir/__/anet/src/epollmp.c.o.requires
server/CMakeFiles/server.dir/requires: server/CMakeFiles/server.dir/__/anet/src/iomp.c.o.requires
server/CMakeFiles/server.dir/requires: server/CMakeFiles/server.dir/__/anet/src/netapi.c.o.requires
server/CMakeFiles/server.dir/requires: server/CMakeFiles/server.dir/__/anet/src/tcpclient.c.o.requires
server/CMakeFiles/server.dir/requires: server/CMakeFiles/server.dir/__/anet/src/tcpserver.c.o.requires
server/CMakeFiles/server.dir/requires: server/CMakeFiles/server.dir/main.c.o.requires
.PHONY : server/CMakeFiles/server.dir/requires

server/CMakeFiles/server.dir/clean:
	cd /mnt/hgfs/linux_share/redis-net2.0/server && $(CMAKE_COMMAND) -P CMakeFiles/server.dir/cmake_clean.cmake
.PHONY : server/CMakeFiles/server.dir/clean

server/CMakeFiles/server.dir/depend:
	cd /mnt/hgfs/linux_share/redis-net2.0 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /mnt/hgfs/linux_share/redis-net2.0 /mnt/hgfs/linux_share/redis-net2.0/server /mnt/hgfs/linux_share/redis-net2.0 /mnt/hgfs/linux_share/redis-net2.0/server /mnt/hgfs/linux_share/redis-net2.0/server/CMakeFiles/server.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : server/CMakeFiles/server.dir/depend

