CXX = g++
CXXFLAGS = -Iinclude
LDFLAGS = -lPsapi -lkernel32 -ladvapi32 -lws2_32 -liphlpapi
TARGET = eva-01
SRCS = src\main.cpp src\CentralDogma.cpp src\TerminalDogma.cpp
all:
	$(CXX) $(CXXFLAGS) $(SRCS) $(LDFLAGS) -o $(TARGET)