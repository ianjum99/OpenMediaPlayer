CXX = g++
CXXFLAGS = -Wall -std=c++11
LDFLAGS = -lavformat -lavcodec -lavutil -lswscale -lavfilter -lswresample -lSDL2

SRC_DIR = src
INCLUDE_DIR = include
OBJ_DIR = obj
TARGET = media_player

SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)/*.o $(TARGET)
