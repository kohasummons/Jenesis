
#OBJS specifies which files to compile as part of the project
OBJS = $(wildcard src/*.cpp) $(wildcard objs/*.o) 
# $(wildcard objs/bullet/*.o)

#CC specifies which compiler we're using
CC = g++

#INCLUDE_PATHS specifies the additional include paths we'll need
INCLUDE_PATHS = -I.\deps\include -I.\deps\include\bullet

#LIBRARY_PATHS specifies the additional library paths we'll need
LIBRARY_PATHS = -L.\deps\lib

#COMPILER_FLAGS specifies the additional compilation options we're using
# -w suppresses all warnings
# -Wl,-subsystem,windows gets rid of the console window
COMPILER_FLAGS = -w -Wl,-subsystem,windows

#LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = -lmingw32 -lopengl32 -lglu32 -lSDL2main -lSDL2 -lglew32

#OBJ_NAME specifies the name of our exectuable
BIN_NAME = .\bin\Game

#This is the target that compiles our executable
all : $(OBJS)

	cmd /k $(CC) $(OBJS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(BIN_NAME)