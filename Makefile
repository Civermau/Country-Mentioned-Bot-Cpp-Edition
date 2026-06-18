CXX     = g++
CXXFLAGS = -std=c++23
SRCS    = src/main.cpp src/database.cpp src/helpers.cpp src/embeds.cpp
TARGET  = build/bot-uwu

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS) -ldpp -lsqlite3

run: all
	./$(TARGET)

clean:
	rm -f $(TARGET)