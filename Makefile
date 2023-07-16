# Configurate dirs
APPS = ./apps
BIN = ./bin
INCLUDE = ./include
OBJ = ./obj
SRC = ./src

FLAGS = -Wall -O3

# Build and generate object files and program files
all: \
	$(patsubst $(SRC)/%.cpp, $(OBJ)/%.o, $(wildcard $(SRC)/*.cpp)) \
	$(patsubst $(APPS)/%.cpp, $(BIN)/%, $(wildcard $(APPS)/*.cpp))

$(OBJ)/%.o: $(SRC)/%.cpp $(INCLUDE)/%.h
	g++ -c $< -I $(INCLUDE) -o $@ $(FLAGS)

$(BIN)/%: $(APPS)/%.cpp
	g++ $< $(OBJ)/*.o -I $(INCLUDE) -o $@ $(FLAGS)


# Clean up project build files
clean:
	rm $(BIN)/*
	rm $(OBJ)/*
