CXX = g++
CXXFLAGS = -std=c++11 -Wall -O2

# Windows er jonno output file er nam
TARGET = oop4.exe

SRCS = main.cpp Engine.cpp Rule.cpp User.cpp
OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	del /Q *.o $(TARGET)
