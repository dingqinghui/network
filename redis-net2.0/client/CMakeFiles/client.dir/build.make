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
include client/CMakeFiles/client.dir/depend.make

# Include the progress variables for this target.
include client/CMakeFiles/client.dir/progress.make

# Include the compile flags for this target's objects.
include client/CMakeFiles/client.dir/flags.make

client/CMakeFiles/client.dir/__/anet/src/connction.c.o: client/CMakeFiles/client.dir/flags.make
client/CMakeFiles/client.dir/__/anet/src/connction.c.o: anet/src/connction.c
	$(CMAKE_COMMAND) -E cmake_progress_report /mnt/hgfs/linux_share/redis-net2.0/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object client/CMakeFiles/client.dir/__/anet/src/connction.c.o"
	cd /mnt/hgfs/linux_share/redis-net2.0/client && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/client.dir/__/anet/src/connction.c.o   -c /mnt/hgfs/linux_share/redis-net2.0/anet/src/connction.c

client/CMakeFiles/client.dir/__/anet/src/connction.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/client.dir/__/anet/src/connction.c.i"
	cd /mnt/hgfs/linux_share/redis-net2.0/client && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /mnt/hgfs/linux_share/redis-net2.0/anet/src/connction.c > CMakeFiles/client.dir/__/anet/src/connction.c.i

client/CMakeFiles/client.dir/__/anet/src/connction.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/client.dir/__/anet/src/connction.c.s"
	cd /mnt/hgfs/linux_share/redis-net2.0/client && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /mnt/hgfs/linux_share/redis-net2.0/anet/src/connction.c -o CMakeFiles/client.dir/__/anet/src/connction.c.s

client/CMakeFiles/client.dir/__/anet/src/connction.c.o.requires:
.PHONY : client/CMakeFiles/client.dir/__/anet/src/connction.c.o.requires

client/CMakeFiles/client.dir/__/anet/src/connction.c.o.provides: client/CMakeFiles/client.dir/__/anet/src/connction.c.o.requires
	$(MAKE) -f client/CMakeFiles/client.dir/build.make client/CMakeFiles/client.dir/__/anet/src/connction.c.o.provides.build
.PHONY : client/CMakeFiles/client.dir/__/anet/src/connction.c.o.provides

client/CMakeFiles/client.dir/__/anet/src/connction.c.o.provides.build: client/CMakeFiles/client.dir/__/anet/src/connction.c.o

client/CMakeFiles/client.dir/__/anet/src/epollmp.c.o: client/CMakeFiles/client.dir/flags.make
client/CMakeFiles/client.dir/__/anet/src/epollmp.c.o: anet/src/epollmp.c
	$(CMAKE_COMMAND) -E cmake_progress_report /mnt/hgfs/linux_share/redis-net2.0/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object client/CMakeFiles/client.dir/__/anet/src/epollmp.c.o"
	cd /mnt/hgfs/linux_share/redis-net2.0/client && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/client.dir/__/anet/src/epollmp.c.o   -c /mnt/hgfs/linux_share/redis-net2.0/anet/src/epollmp.c

client/CMakeFiles/client.dir/__/anet/src/epollmp.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/client.dir/__/anet/src/epollmp.c.i"
	cd /mnt/hgfs/linux_share/redis-net2.0/client && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /mnt/hgfs/linux_share/redis-net2.0/anet/src/epollmp.c > CMakeFiles/client.dir/__/anet/src/epollmp.c.i

client/CMakeFiles/client.dir/__/anet/src/epollmp.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/client.dir/__/anet/src/epollmp.c.s"
	cd /mnt/hgfs/linux_share/redis-net2.0/client && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /mnt/hgfs/linux_share/redis-net2.0/anet/src/epollmp.c -o CMakeFiles/client.dir/__/anet/src/epollmp.c.s

client/CMakeFiles/client.dir/__/anet/src/epollmp.c.o.requires:
.PHONY : client/CMakeFiles/client.dir/__/anet/src/epollmp.c.o.requires

client/CMakeFiles/client.dir/__/anet/src/epollmp.c.o.provides: client/CMakeFiles/client.dir/__/anet/src/epollmp.c.o.requires
	$(MAKE) -f client/CMakeFiles/client.dir/build.make client/CMakeFiles/client.dir/__/anet/src/epollmp.c.o.provides.build
.PHONY : client/CMakeFiles/client.dir/__/anet/src/epollmp.c.o.provides

client/CMakeFiles/client.dir/__/anet/src/epollmp.c.o.provides.build: client/CMakeFiles/client.dir/__/anet/src/epollmp.c.o

client/CMakeFiles/client.dir/__/anet/src/error.c.o: client/CMakeFiles/client.dir/flags.make
client/CMakeFiles/client.dir/__/anet/src/error.c.o: anet/src/error.c
	$(CMAKE_COMMAND) -E cmake_progress_report /mnt/hgfs/linux_share/redis-net2.0/CMakeFiles $(CMAKE_PROGRESS_3)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object client/CMakeFiles/client.dir/__/anet/src/error.c.o"
	cd /mnt/hgfs/linux_share/redis-net2.0/client && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/client.dir/__/anet/src/error.c.o   -c /mnt/hgfs/linux_share/redis-net2.0/anet/src/error.c

client/CMakeFiles/client.dir/__/anet/src/error.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/client.dir/__/anet/src/error.c.i"
	cd /mnt/hgfs/linux_share/redis-net2.0/client && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /mnt/hgfs/linux_share/redis-net2.0/anet/src/error.c > CMakeFiles/client.dir/__/anet/src/error.c.i

client/CMakeFiles/client.dir/__/anet/src/error.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/client.dir/__/anet/src/error.c.s"
	cd /mnt/hgfs/linux_share/redis-net2.0/client && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /mnt/hgfs/linux_share/redis-net2.0/anet/src/error.c -o CMakeFiles/client.dir/__/anet/src/error.c.s

client/CMakeFiles/client.dir/__/anet/src/error.c.o.requires:
.PHONY : client/CMakeFiles/client.dir/__/anet/src/error.c.o.requires

client/CMakeFiles/client.dir/__/anet/src/error.c.o.provides: client/CMakeFiles/client.dir/__/anet/src/error.c.o.requires
	$(MAKE) -f client/CMakeFiles/client.dir/build.make client/CMakeFiles/client.dir/__/anet/src/error.c.o.provides.build
.PHONY : client/CMakeFiles/client.dir/__/anet/src/error.c.o.provides

client/CMakeFiles/client.dir/__/anet/src/error.c.o.provides.build: client/CMakeFiles/client.dir/__/anet/src/error.c.o

client/CMakeFiles/client.dir/__/anet/src/iomp.c.o: client/CMakeFiles/client.dir/flags.make
client/CMakeFiles/client.dir/__/anet/src/iomp.c.o: anet/src/iomp.c
	$(CMAKE_COMMAND) -E cmake_progress_report /mnt/hgfs/linux_share/redis-net2.0/CMakeFiles $(CMAKE_PROGRESS_4)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object client/CMakeFiles/client.dir/__/anet/src/iomp.c.o"
	cd /mnt/hgfs/linux_share/redis-net2.0/client && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/client.dir/__/anet/src/iomp.c.o   -c /mnt/hgfs/linux_share/redis-net2.0/anet/src/iomp.c

client/CMakeFiles/client.dir/__/anet/src/iomp.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/client.dir/__/anet/src/iomp.c.i"
	cd /mnt/hgfs/linux_share/redis-net2.0/client && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /mnt/hgfs/linux_share/redis-net2.0/anet/src/iomp.c > CMakeFiles/client.dir/__/anet/src/iomp.c.i

client/CMakeFiles/client.dir/__/anet/src/iomp.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/client.dir/__/anet/src/iomp.c.s"
	cd /mnt/hgfs/linux_share/redis-net2.0/client && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /mnt/hgfs/linux_share/redis-net2.0/anet/src/iomp.c -o CMakeFiles/client.dir/__/anet/src/iomp.c.s

client/CMakeFiles/client.dir/__/anet/src/iomp.c.o.requires:
.PHONY : client/CMakeFiles/client.dir/__/anet/src/iomp.c.o.requires

client/CMakeFiles/client.dir/__/anet/src/iomp.c.o.provides: client/CMakeFiles/client.dir/__/anet/src/iomp.c.o.requires
	$(MAKE) -f client/CMakeFiles/client.dir/build.make client/CMakeFiles/client.dir/__/anet/src/iomp.c.o.provides.build
.PHONY : client/CMakeFiles/client.dir/__/anet/src/iomp.c.o.provides

client/CMakeFiles/client.dir/__/anet/src/iomp.c.o.provides.build: client/CMakeFiles/client.dir/__/anet/src/iomp.c.o

client/CMakeFiles/client.dir/__/anet/src/netapi.c.o: client/CMakeFiles/client.dir/flags.make
client/CMakeFiles/client.dir/__/anet/src/netapi.c.o: anet/src/netapi.c
	$(CMAKE_COMMAND) -E cmake_progress_report /mnt/hgfs/linux_share/redis-net2.0/CMakeFiles $(CMAKE_PROGRESS_5)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object client/CMakeFiles/client.dir/__/anet/src/netapi.c.o"
	cd /mnt/hgfs/linux_share/redis-net2.0/client && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/client.dir/__/anet/src/netapi.c.o   -c /mnt/hgfs/linux_share/redis-net2.0/anet/src/netapi.c

client/CMakeFiles/client.dir/__/anet/src/netapi.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/client.dir/__/anet/src/netapi.c.i"
	cd /mnt/hgfs/linux_share/redis-net2.0/client && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /mnt/hgfs/linux_share/redis-net2.0/anet/src/netapi.c > CMakeFiles/client.dir/__/anet/src/netapi.c.i

client/CMakeFiles/client.dir/__/anet/src/netapi.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/client.dir/__/anet/src/netapi.c.s"
	cd /mnt/hgfs/linux_share/redis-net2.0/client && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /mnt/hgfs/linux_share/redis-net2.0/anet/src/netapi.c -o CMakeFiles/client.dir/__/anet/src/netapi.c.s

client/CMakeFiles/client.dir/__/anet/src/netapi.c.o.requires:
.PHONY : client/CMakeFiles/client.dir/__/anet/src/netapi.c.o.requires

client/CMakeFiles/client.dir/__/anet/src/netapi.c.o.provides: client/CMakeFiles/client.dir/__/anet/src/netapi.c.o.requires
	$(MAKE) -f client/CMakeFiles/client.dir/build.make client/CMakeFiles/client.dir/__/anet/src/netapi.c.o.provides.build
.PHONY : client/CMakeFiles/client.dir/__/anet/src/netapi.c.o.provides

client/CMakeFiles/client.dir/__/anet/src/netapi.c.o.provides.build: client/CMakeFiles/client.dir/__/anet/src/netapi.c.o

client/CMakeFiles/client.dir/__/anet/src/tcpclient.c.o: client/CMakeFiles/client.dir/flags.make
client/CMakeFiles/client.dir/__/anet/src/tcpclient.c.o: anet/src/tcpclient.c
	$(CMAKE_COMMAND) -E cmake_progress_report /mnt/hgfs/linux_share/redis-net2.0/CMakeFiles $(CMAKE_PROGRESS_6)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object client/CMakeFiles/client.dir/__/anet/src/tcpclient.c.o"
	cd /mnt/hgfs/linux_share/redis-net2.0/client && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/client.dir/__/anet/src/tcpclient.c.o   -c /mnt/hgfs/linux_share/redis-net2.0/anet/src/tcpclient.c

client/CMakeFiles/client.dir/__/anet/src/tcpclient.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/client.dir/__/anet/src/tcpclient.c.i"
	cd /mnt/hgfs/linux_share/redis-net2.0/client && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /mnt/hgfs/linux_share/redis-net2.0/anet/src/tcpclient.c > CMakeFiles/client.dir/__/anet/src/tcpclient.c.i

client/CMakeFiles/client.dir/__/anet/src/tcpclient.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/client.dir/__/anet/src/tcpclient.c.s"
	cd /mnt/hgfs/linux_share/redis-net2.0/client && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /mnt/hgfs/linux_share/redis-net2.0/anet/src/tcpclient.c -o CMakeFiles/client.dir/__/anet/src/tcpclient.c.s

client/CMakeFiles/client.dir/__/anet/src/tcpclient.c.o.requires:
.PHONY : client/CMakeFiles/client.dir/__/anet/src/tcpclient.c.o.requires

client/CMakeFiles/client.dir/__/anet/src/tcpclient.c.o.provides: client/CMakeFiles/client.dir/__/anet/src/tcpclient.c.o.requires
	$(MAKE) -f client/CMakeFiles/client.dir/build.make client/CMakeFiles/client.dir/__/anet/src/tcpclient.c.o.provides.build
.PHONY : client/CMakeFiles/client.dir/__/anet/src/tcpclient.c.o.provides

client/CMakeFiles/client.dir/__/anet/src/tcpclient.c.o.provides.build: client/CMakeFiles/client.dir/__/anet/src/tcpclient.c.o

client/CMakeFiles/client.dir/__/anet/src/tcpserver.c.o: client/CMakeFiles/client.dir/flags.make
client/CMakeFiles/client.dir/__/anet/src/tcpserver.c.o: anet/src/tcpserver.c
	$(CMAKE_COMMAND) -E cmake_progress_report /mnt/hgfs/linux_share/redis-net2.0/CMakeFiles $(CMAKE_PROGRESS_7)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object client/CMakeFiles/client.dir/__/anet/src/tcpserver.c.o"
	cd /mnt/hgfs/linux_share/redis-net2.0/client && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/client.dir/__/anet/src/tcpserver.c.o   -c /mnt/hgfs/linux_share/redis-net2.0/anet/src/tcpserver.c

client/CMakeFiles/client.dir/__/anet/src/tcpserver.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/client.dir/__/anet/src/tcpserver.c.i"
	cd /mnt/hgfs/linux_share/redis-net2.0/client && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /mnt/hgfs/linux_share/redis-net2.0/anet/src/tcpserver.c > CMakeFiles/client.dir/__/anet/src/tcpserver.c.i

client/CMakeFiles/client.dir/__/anet/src/tcpserver.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/client.dir/__/anet/src/tcpserver.c.s"
	cd /mnt/hgfs/linux_share/redis-net2.0/client && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /mnt/hgfs/linux_share/redis-net2.0/anet/src/tcpserver.c -o CMakeFiles/client.dir/__/anet/src/tcpserver.c.s

client/CMakeFiles/client.dir/__/anet/src/tcpserver.c.o.requires:
.PHONY : client/CMakeFiles/client.dir/__/anet/src/tcpserver.c.o.requires

client/CMakeFiles/client.dir/__/anet/src/tcpserver.c.o.provides: client/CMakeFiles/client.dir/__/anet/src/tcpserver.c.o.requires
	$(MAKE) -f client/CMakeFiles/client.dir/build.make client/CMakeFiles/client.dir/__/anet/src/tcpserver.c.o.provides.build
.PHONY : client/CMakeFiles/client.dir/__/anet/src/tcpserver.c.o.provides

client/CMakeFiles/client.dir/__/anet/src/tcpserver.c.o.provides.build: client/CMakeFiles/client.dir/__/anet/src/tcpserver.c.o

client/CMakeFiles/client.dir/main.c.o: client/CMakeFiles/client.dir/flags.make
client/CMakeFiles/client.dir/main.c.o: client/main.c
	$(CMAKE_COMMAND) -E cmake_progress_report /mnt/hgfs/linux_share/redis-net2.0/CMakeFiles $(CMAKE_PROGRESS_8)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object client/CMakeFiles/client.dir/main.c.o"
	cd /mnt/hgfs/linux_share/redis-net2.0/client && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/client.dir/main.c.o   -c /mnt/hgfs/linux_share/redis-net2.0/client/main.c

client/CMakeFiles/client.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/client.dir/main.c.i"
	cd /mnt/hgfs/linux_share/redis-net2.0/client && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /mnt/hgfs/linux_share/redis-net2.0/client/main.c > CMakeFiles/client.dir/main.c.i

client/CMakeFiles/client.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/client.dir/main.c.s"
	cd /mnt/hgfs/linux_share/redis-net2.0/client && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /mnt/hgfs/linux_share/redis-net2.0/client/main.c -o CMakeFiles/client.dir/main.c.s

client/CMakeFiles/client.dir/main.c.o.requires:
.PHONY : client/CMakeFiles/client.dir/main.c.o.requires

client/CMakeFiles/client.dir/main.c.o.provides: client/CMakeFiles/client.dir/main.c.o.requires
	$(MAKE) -f client/CMakeFiles/client.dir/build.make client/CMakeFiles/client.dir/main.c.o.provides.build
.PHONY : client/CMakeFiles/client.dir/main.c.o.provides

client/CMakeFiles/client.dir/main.c.o.provides.build: client/CMakeFiles/client.dir/main.c.o

# Object files for target client
client_OBJECTS = \
"CMakeFiles/client.dir/__/anet/src/connction.c.o" \
"CMakeFiles/client.dir/__/anet/src/epollmp.c.o" \
"CMakeFiles/client.dir/__/anet/src/error.c.o" \
"CMakeFiles/client.dir/__/anet/src/iomp.c.o" \
"CMakeFiles/client.dir/__/anet/src/netapi.c.o" \
"CMakeFiles/client.dir/__/anet/src/tcpclient.c.o" \
"CMakeFiles/client.dir/__/anet/src/tcpserver.c.o" \
"CMakeFiles/client.dir/main.c.o"

# External object files for target client
client_EXTERNAL_OBJECTS =

bin/client: client/CMakeFiles/client.dir/__/anet/src/connction.c.o
bin/client: client/CMakeFiles/client.dir/__/anet/src/epollmp.c.o
bin/client: client/CMakeFiles/client.dir/__/anet/src/error.c.o
bin/client: client/CMakeFiles/client.dir/__/anet/src/iomp.c.o
bin/client: client/CMakeFiles/client.dir/__/anet/src/netapi.c.o
bin/client: client/CMakeFiles/client.dir/__/anet/src/tcpclient.c.o
bin/client: client/CMakeFiles/client.dir/__/anet/src/tcpserver.c.o
bin/client: client/CMakeFiles/client.dir/main.c.o
bin/client: client/CMakeFiles/client.dir/build.make
bin/client: client/CMakeFiles/client.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking C executable ../bin/client"
	cd /mnt/hgfs/linux_share/redis-net2.0/client && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/client.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
client/CMakeFiles/client.dir/build: bin/client
.PHONY : client/CMakeFiles/client.dir/build

client/CMakeFiles/client.dir/requires: client/CMakeFiles/client.dir/__/anet/src/connction.c.o.requires
client/CMakeFiles/client.dir/requires: client/CMakeFiles/client.dir/__/anet/src/epollmp.c.o.requires
client/CMakeFiles/client.dir/requires: client/CMakeFiles/client.dir/__/anet/src/error.c.o.requires
client/CMakeFiles/client.dir/requires: client/CMakeFiles/client.dir/__/anet/src/iomp.c.o.requires
client/CMakeFiles/client.dir/requires: client/CMakeFiles/client.dir/__/anet/src/netapi.c.o.requires
client/CMakeFiles/client.dir/requires: client/CMakeFiles/client.dir/__/anet/src/tcpclient.c.o.requires
client/CMakeFiles/client.dir/requires: client/CMakeFiles/client.dir/__/anet/src/tcpserver.c.o.requires
client/CMakeFiles/client.dir/requires: client/CMakeFiles/client.dir/main.c.o.requires
.PHONY : client/CMakeFiles/client.dir/requires

client/CMakeFiles/client.dir/clean:
	cd /mnt/hgfs/linux_share/redis-net2.0/client && $(CMAKE_COMMAND) -P CMakeFiles/client.dir/cmake_clean.cmake
.PHONY : client/CMakeFiles/client.dir/clean

client/CMakeFiles/client.dir/depend:
	cd /mnt/hgfs/linux_share/redis-net2.0 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /mnt/hgfs/linux_share/redis-net2.0 /mnt/hgfs/linux_share/redis-net2.0/client /mnt/hgfs/linux_share/redis-net2.0 /mnt/hgfs/linux_share/redis-net2.0/client /mnt/hgfs/linux_share/redis-net2.0/client/CMakeFiles/client.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : client/CMakeFiles/client.dir/depend

