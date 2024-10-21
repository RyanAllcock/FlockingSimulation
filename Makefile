CXX := g++

BIN := temp/
LIB := lib/
SRC := source/
OBJ := $(BIN)shader.o $(BIN)boids.o
LINK := -lopenGL32 -lmingw32 -lSDL2main -lSDL2 -lglew32

main: main.cpp $(OBJ)
	$(CXX) -o boids.exe $(OBJ) main.cpp $(LINK)

$(BIN)shader.o: $(LIB)shader.cpp $(LIB)shader.hpp
	$(CXX) -c -o $(BIN)shader.o $(LIB)shader.cpp

$(BIN)boids.o: $(SRC)boids.cpp $(SRC)boids.hpp
	$(CXX) -c -o $(BIN)boids.o $(SRC)boids.cpp

prepare:
	mkdir $(BIN)

clean:
	cd $(BIN) & del /q /s "*.o" & cd .. & $(MAKE) --no-print-directory main