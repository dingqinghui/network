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
CMAKE_SOURCE_DIR = /mnt/hgfs/linux_share/net_server

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /mnt/hgfs/linux_share/net_server

# Include any dependencies generated for this target.
include server/CMakeFiles/server.dir/depend.make

# Include the progress variables for this target.
include server/CMakeFiles/server.dir/progress.make

# Include the compile flags for this target's objects.
include server/CMakeFiles/server.dir/flags.make

server/CMakeFiles/server.dir/main.c.o: server/CMakeFiles/server.dir/flags.make
server/CMakeFiles/server.dir/main.c.o: server/main.c
	$(CMAKE_COMMAND) -E cmake_progress_report /mnt/hgfs/linux_share/net_server/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object server/CMakeFiles/server.dir/main.c.o"
	cd /mnt/hgfs/linux_share/net_server/server && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/server.dir/main.c.o   -c /mnt/hgfs/linux_share/net_server/server/main.c

server/CMakeFiles/server.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/server.dir/main.c.i"
	cd /mnt/hgfs/linux_share/net_server/server && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /mnt/hgfs/linux_share/net_server/server/main.c > CMakeFiles/server.dir/main.c.i

server/CMakeFiles/server.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/server.dir/main.c.s"
	cd /mnt/hgfs/linux_share/net_server/server && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /mnt/hgfs/linux_share/net_server/server/main.c -o CMakeFiles/server.dir/main.c.s

server/CMakeFiles/server.dir/main.c.o.requires:
.PHONY : server/CMakeFiles/server.dir/main.c.o.requires

server/CMakeFiles/server.dir/main.c.o.provides: server/CMakeFiles/server.dir/main.c.o.requires
	$(MAKE) -f server/CMakeFiles/server.dir/build.make server/CMakeFiles/server.dir/main.c.o.provides.build
.PHONY : server/CMakeFiles/server.dir/main.c.o.provides

server/CMakeFiles/server.dir/main.c.o.provides.build: server/CMakeFiles/server.dir/main.c.o

server/CMakeFiles/server.dir/__/net/src/net_server.c.o: server/CMakeFiles/server.dir/flags.make
server/CMakeFiles/server.dir/__/net/src/net_server.c.o: net/src/net_server.c
	$(CMAKE_COMMAND) -E cmake_progress_report /mnt/hgfs/linux_share/net_server/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object server/CMakeFiles/server.dir/__/net/src/net_server.c.o"
	cd /mnt/hgfs/linux_share/net_server/server && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/server.dir/__/net/src/net_server.c.o   -c /mnt/hgfs/linux_share/net_server/net/src/net_server.c

server/CMakeFiles/server.dir/__/net/src/net_server.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/server.dir/__/net/src/net_server.c.i"
	cd /mnt/hgfs/linux_share/net_server/server && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /mnt/hgfs/linux_share/net_server/net/src/net_server.c > CMakeFiles/server.dir/__/net/src/net_server.c.i

server/CMakeFiles/server.dir/__/net/src/net_server.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/server.dir/__/net/src/net_server.c.s"
	cd /mnt/hgfs/linux_share/net_server/server && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /mnt/hgfs/linux_share/net_server/net/src/net_server.c -o CMakeFiles/server.dir/__/net/src/net_server.c.s

server/CMakeFiles/server.dir/__/net/src/net_server.c.o.requires:
.PHONY : server/CMakeFiles/server.dir/__/net/src/net_server.c.o.requires

server/CMakeFiles/server.dir/__/net/src/net_server.c.o.provides: server/CMakeFiles/server.dir/__/net/src/net_server.c.o.requires
	$(MAKE) -f server/CMakeFiles/server.dir/build.make server/CMakeFiles/server.dir/__/net/src/net_server.c.o.provides.build
.PHONY : server/CMakeFiles/server.dir/__/net/src/net_server.c.o.provides

server/CMakeFiles/server.dir/__/net/src/net_server.c.o.provides.build: server/CMakeFiles/server.dir/__/net/src/net_server.c.o

server/CMakeFiles/server.dir/__/net/src/socket.c.o: server/CMakeFiles/server.dir/flags.make
server/CMakeFiles/server.dir/__/net/src/socket.c.o: net/src/socket.c
	$(CMAKE_COMMAND) -E cmake_progress_report /mnt/hgfs/linux_share/net_server/CMakeFiles $(CMAKE_PROGRESS_3)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object server/CMakeFiles/server.dir/__/net/src/socket.c.o"
	cd /mnt/hgfs/linux_share/net_server/server && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/server.dir/__/net/src/socket.c.o   -c /mnt/hgfs/linux_share/net_server/net/src/socket.c

server/CMakeFiles/server.dir/__/net/src/socket.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/server.dir/__/net/src/socket.c.i"
	cd /mnt/hgfs/linux_share/net_server/server && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /mnt/hgfs/linux_share/net_server/net/src/socket.c > CMakeFiles/server.dir/__/net/src/socket.c.i

server/CMakeFiles/server.dir/__/net/src/socket.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/server.dir/__/net/src/socket.c.s"
	cd /mnt/hgfs/linux_share/net_server/server && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /mnt/hgfs/linux_share/net_server/net/src/socket.c -o CMakeFiles/server.dir/__/net/src/socket.c.s

server/CMakeFiles/server.dir/__/net/src/socket.c.o.requires:
.PHONY : server/CMakeFiles/server.dir/__/net/src/socket.c.o.requires

server/CMakeFiles/server.dir/__/net/src/socket.c.o.provides: server/CMakeFiles/server.dir/__/net/src/socket.c.o.requires
	$(MAKE) -f server/CMakeFiles/server.dir/build.make server/CMakeFiles/server.dir/__/net/src/socket.c.o.provides.build
.PHONY : server/CMakeFiles/server.dir/__/net/src/socket.c.o.provides

server/CMakeFiles/server.dir/__/net/src/socket.c.o.provides.build: server/CMakeFiles/server.dir/__/net/src/socket.c.o

server/CMakeFiles/server.dir/__/net/src/socket_epoll.c.o: server/CMakeFiles/server.dir/flags.make
server/CMakeFiles/server.dir/__/net/src/socket_epoll.c.o: net/src/socket_epoll.c
	$(CMAKE_COMMAND) -E cmake_progress_report /mnt/hgfs/linux_share/net_server/CMakeFiles $(CMAKE_PROGRESS_4)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object server/CMakeFiles/server.dir/__/net/src/socket_epoll.c.o"
	cd /mnt/hgfs/linux_share/net_server/server && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/server.dir/__/net/src/socket_epoll.c.o   -c /mnt/hgfs/linux_share/net_server/net/src/socket_epoll.c

server/CMakeFiles/server.dir/__/net/src/socket_epoll.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/server.dir/__/net/src/socket_epoll.c.i"
	cd /mnt/hgfs/linux_share/net_server/server && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /mnt/hgfs/linux_share/net_server/net/src/socket_epoll.c > CMakeFiles/server.dir/__/net/src/socket_epoll.c.i

server/CMakeFiles/server.dir/__/net/src/socket_epoll.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/server.dir/__/net/src/socket_epoll.c.s"
	cd /mnt/hgfs/linux_share/net_server/server && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /mnt/hgfs/linux_share/net_server/net/src/socket_epoll.c -o CMakeFiles/server.dir/__/net/src/socket_epoll.c.s

server/CMakeFiles/server.dir/__/net/src/socket_epoll.c.o.requires:
.PHONY : server/CMakeFiles/server.dir/__/net/src/socket_epoll.c.o.requires

server/CMakeFiles/server.dir/__/net/src/socket_epoll.c.o.provides: server/CMakeFiles/server.dir/__/net/src/socket_epoll.c.o.requires
	$(MAKE) -f server/CMakeFiles/server.dir/build.make server/CMakeFiles/server.dir/__/net/src/socket_epoll.c.o.provides.build
.PHONY : server/CMakeFiles/server.dir/__/net/src/socket_epoll.c.o.provides

server/CMakeFiles/server.dir/__/net/src/socket_epoll.c.o.provides.build: server/CMakeFiles/server.dir/__/net/src/socket_epoll.c.o

server/CMakeFiles/server.dir/__/net/src/socket_event.c.o: server/CMakeFiles/server.dir/flags.make
server/CMakeFiles/server.dir/__/net/src/socket_event.c.o: net/src/socket_event.c
	$(CMAKE_COMMAND) -E cmake_progress_report /mnt/hgfs/linux_share/net_server/CMakeFiles $(CMAKE_PROGRESS_5)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object server/CMakeFiles/server.dir/__/net/src/socket_event.c.o"
	cd /mnt/hgfs/linux_share/net_server/server && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/server.dir/__/net/src/socket_event.c.o   -c /mnt/hgfs/linux_share/net_server/net/src/socket_event.c

server/CMakeFiles/server.dir/__/net/src/socket_event.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/server.dir/__/net/src/socket_event.c.i"
	cd /mnt/hgfs/linux_share/net_server/server && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /mnt/hgfs/linux_share/net_server/net/src/socket_event.c > CMakeFiles/server.dir/__/net/src/socket_event.c.i

server/CMakeFiles/server.dir/__/net/src/socket_event.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/server.dir/__/net/src/socket_event.c.s"
	cd /mnt/hgfs/linux_share/net_server/server && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /mnt/hgfs/linux_share/net_server/net/src/socket_event.c -o CMakeFiles/server.dir/__/net/src/socket_event.c.s

server/CMakeFiles/server.dir/__/net/src/socket_event.c.o.requires:
.PHONY : server/CMakeFiles/server.dir/__/net/src/socket_event.c.o.requires

server/CMakeFiles/server.dir/__/net/src/socket_event.c.o.provides: server/CMakeFiles/server.dir/__/net/src/socket_event.c.o.requires
	$(MAKE) -f server/CMakeFiles/server.dir/build.make server/CMakeFiles/server.dir/__/net/src/socket_event.c.o.provides.build
.PHONY : server/CMakeFiles/server.dir/__/net/src/socket_event.c.o.provides

server/CMakeFiles/server.dir/__/net/src/socket_event.c.o.provides.build: server/CMakeFiles/server.dir/__/net/src/socket_event.c.o

# Object files for target server
server_OBJECTS = \
"CMakeFiles/server.dir/main.c.o" \
"CMakeFiles/server.dir/__/net/src/net_server.c.o" \
"CMakeFiles/server.dir/__/net/src/socket.c.o" \
"CMakeFiles/server.dir/__/net/src/socket_epoll.c.o" \
"CMakeFiles/server.dir/__/net/src/socket_event.c.o"

# External object files for target server
server_EXTERNAL_OBJECTS =

/mnt/hgfs/linux_share/bin/server: server/CMakeFiles/server.dir/main.c.o
/mnt/hgfs/linux_share/bin/server: server/CMakeFiles/server.dir/__/net/src/net_server.c.o
/mnt/hgfs/linux_share/bin/server: server/CMakeFiles/server.dir/__/net/src/socket.c.o
/mnt/hgfs/linux_share/bin/server: server/CMakeFiles/server.dir/__/net/src/socket_epoll.c.o
/mnt/hgfs/linux_share/bin/server: server/CMakeFiles/server.dir/__/net/src/socket_event.c.o
/mnt/hgfs/linux_share/bin/server: server/CMakeFiles/server.dir/build.make
/mnt/hgfs/linux_share/bin/server: server/CMakeFiles/server.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking C executable /mnt/hgfs/linux_share/bin/server"
	cd /mnt/hgfs/linux_share/net_server/server && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/server.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
server/CMakeFiles/server.dir/build: /mnt/hgfs/linux_share/bin/server
.PHONY : server/CMakeFiles/server.dir/build

server/CMakeFiles/server.dir/requires: server/CMakeFiles/server.dir/main.c.o.requires
server/CMakeFiles/server.dir/requires: server/CMakeFiles/server.dir/__/net/src/net_server.c.o.requires
server/CMakeFiles/server.dir/requires: server/CMakeFiles/server.dir/__/net/src/socket.c.o.requires
server/CMakeFiles/server.dir/requires: server/CMakeFiles/server.dir/__/net/src/socket_epoll.c.o.requires
server/CMakeFiles/server.dir/requires: server/CMakeFiles/server.dir/__/net/src/socket_event.c.o.requires
.PHONY : server/CMakeFiles/server.dir/requires

server/CMakeFiles/server.dir/clean:
	cd /mnt/hgfs/linux_share/net_server/server && $(CMAKE_COMMAND) -P CMakeFiles/server.dir/cmake_clean.cmake
.PHONY : server/CMakeFiles/server.dir/clean

server/CMakeFiles/server.dir/depend:
	cd /mnt/hgfs/linux_share/net_server && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /mnt/hgfs/linux_share/net_server /mnt/hgfs/linux_share/net_server/server /mnt/hgfs/linux_share/net_server /mnt/hgfs/linux_share/net_server/server /mnt/hgfs/linux_share/net_server/server/CMakeFiles/server.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : server/CMakeFiles/server.dir/depend
