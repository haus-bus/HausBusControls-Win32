CXX = g++
CXXFLAGS = -Wall -std=c++11 -g3 -D_WIN32 -D__DUMMY__
LDFLAGS = -static

SHELL := cmd.exe
RM := del /S /Q

PROJ := AR8Simulator
BUILD_DIR := Debug
OBJ_EXT := .o
DEP_EXT := .d
LIB_EXT := .a
EXEC_EXT := .exe

USER_OBJS :=
LIBS := 
C_SRCS := 
OBJS := 
C_DEPS := 
EXECUTABLES := 
OUTPUT_FILE_PATH :=
OUTPUT_FILE_PATH_AS_ARGS :=
QUOTE := "
ADDITIONAL_DEPENDENCIES:=
OUTPUT_FILE_DEP:=
LIB_DEP:=
LINKER_SCRIPT_DEP:=

INCLUDE = \
-I ../../SwFramework

SRC = \
SwFramework/IoStream.cpp \
SwFramework/Reactive.cpp \
SwFramework/Scheduler.cpp \
SwFramework/SwFramework.cpp \
SwFramework/EventPkg/EventBase.cpp \
SwFramework/EventPkg/EventPkg.cpp \
SwFramework/FileSystems/FatSystem.cpp \
SwFramework/Protocols/HBCP.cpp \
SwFramework/Protocols/Ethernet/IP.cpp \
SwFramework/Protocols/Ethernet/IpHeader.cpp \
SwFramework/Protocols/Ethernet/MAC.cpp \
SwFramework/Protocols/Ethernet/TcpHeader.cpp \
SwFramework/Protocols/Ethernet/UdpHeader.cpp \
SwFramework/Protocols/IpStack/ArpManager.cpp \
SwFramework/Protocols/IpStack/Dhcp.cpp \
SwFramework/Protocols/IpStack/IpConnection.cpp \
SwFramework/Protocols/IpStack/IpStackManager.cpp \
SwFramework/Protocols/IpStack/TcpConnection.cpp \
SwFramework/Protocols/IpStack/UdpConnection.cpp \
SwFramework/Security/Checksum.cpp \
SwFramework/Security/Checksum16.cpp \
SwFramework/Security/Crc7.cpp \
SwFramework/Security/Crc8.cpp \
SwFramework/Time/Calender.cpp \
SwFramework/Time/WeekTime.cpp \
SwFramework/Tracing/Logger.cpp \
SwFramework/Tracing/Traces.cpp \
Win32/Basics.cpp \
Win32/Enc28j60.cpp \
Win32/MemoryManager.cpp \
Win32/SystemTime.cpp \
AR8Simulator/AR8Simulator.cpp


OBJS += $(SRC:.cpp=$(OBJ_EXT))
DEPS += $(SRC:.cpp=$(DEP_EXT))
EXEC = $(PROJ)$(EXEC_EXT)

SUB_DIRS := $(foreach DIR,$(sort $(dir $(OBJS))),$(DIR).)

all: $(EXEC)

$(EXEC): $(SUB_DIRS) $(OBJS)
	@echo linking $@
	@$(CXX) $(LDFLAGS) -o $@ $(OBJS) $(LBLIBS)

.PRECIOUS: %/.

# the next target creates all needed subdirs
%/.:
	mkdir "$(@:%/.=%)"

define define_obj_target
$(1) : Makefile $(2)
	@echo compile $(2)
	@$(CXX) $(CXXFLAGS) $(INCLUDE) -c -MD -MP -MF "$(1:%.o=%.d)" -MT"$(1:%.o=%.d)" -MT"$(1:%.o=%.o)" -o "$(1)" "$(2)" 
endef

# generate the targets by using the above template
$(foreach OBJ,$(OBJS),$(eval $(call define_obj_target,$(OBJ),$(OBJ:%.o=../../%.cpp))))

.PHONY: clean 
clean:
	$(RM) *$(EXEC_EXT) *$(LIB_EXT) *$(OBJ_EXT) *$(DEP_EXT)