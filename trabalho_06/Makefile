CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11 -O2
INCLUDES = -I src/core -I src/geometria -I src/iluminacao -I src/materiais
LDFLAGS = -lGL -lGLU -lglut

# Encontra todos os arquivos .cpp nas subpastas
SRCS = $(wildcard src/core/*.cpp) \
       $(wildcard src/geometria/*.cpp) \
       $(wildcard src/iluminacao/*.cpp) \
       $(wildcard src/materiais/*.cpp)

# Define os arquivos .o correspondentes
OBJS = $(SRCS:.cpp=.o)

# Nome do executável
TARGET = trabalhoFinal

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

run: $(TARGET)
	./$(TARGET)
