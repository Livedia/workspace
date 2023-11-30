CXX=g++
CXXFLAGS=-std=c++17 -Wall -I/usr/include/curl
LDFLAGS=-lcurl -pthread

# Object files
OBJS=main.o http_downloader.o downloader.o

# Executable name
TARGET=http_download_tool

# Default target
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET) $(LDFLAGS)

# To obtain object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

# To remove generated files
clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
