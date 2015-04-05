.PHONY=all clean

CXX=clang++
CXXFLAGS=-std=c++1y -g -Iinclude `pkg-config --cflags opencv`
LD=clang++
LDFLAGS=
LDLIBS=`pkg-config --libs opencv`

SRCS=main.cpp
OBJS=$(patsubst %.cpp,build/%.o,$(SRCS))
TARGET=opencv-test

all: $(TARGET)

$(TARGET): $(OBJS)
	$(LD) $(LDFLAGS) $(LDLIBS) $(OBJS) -o $@

build/%.o: src/%.cpp | build
	$(CXX) $(CXXFLAGS) -o $@ -c $<

clean:
	rm -rf build
	rm -f $(TARGET)

build:
	mkdir build
