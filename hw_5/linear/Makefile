CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2

TARGET = regression
SRCS = regression.cpp
HEADERS = linear.h

$(TARGET): $(SRCS) $(HEADERS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS)

.PHONY: run clean
run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET) regression.ppm
