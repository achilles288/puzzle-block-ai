JFLAGS = -g
JC = javac
JVM= java

JAVA_SRCS = \
	src/puzzleblock/Block.java \
	src/puzzleblock/Board.java \
	src/puzzleblock/Effect.java \
	src/puzzleblock/PuzzleBlock.java \
	src/puzzleblock/PuzzleBlock_AI.java


CFLAGS = -fPIC
CC = g++


ifeq ($(OS),Windows_NT)
	INCLUDES = \
		-I$(JAVA_HOME)/include \
		-I$(JAVA_HOME)/include/win32
	
	LIBS = 	
	
else
	INCLUDES = \
		-I$(JAVA_HOME)/include \
		-I$(JAVA_HOME)/include/linux
	
	LIBS = 
	
endif


CXX_SRCS = \
	src/puzzleblock/c++/puzzleblock.cpp \
	src/puzzleblock/c++/routefinding.cpp


DEST = build


ifeq ($(OS),Windows_NT)
	libpuzzleblock := puzzleblock.dll
else
	libpuzzleblock := libpuzzleblock.so
endif

puzzleblock: mkdir cp
	@ echo Compiling C++
	@ $(CC) $(CFLAGS) $(INCLUDES) $(CXX_SRCS) $(LIBS) \
	  -shared -o $(DEST)/puzzleblock/$(libpuzzleblock)
	@ echo Compiling Java
	@ $(JC) $(JFLAGS) -d $(DEST) $(JAVA_SRCS)
	@ echo Compilation success

mkdir:
	@ mkdir -p $(DEST)
	@ mkdir -p $(DEST)/puzzleblock

cp:
	@ cp -r src/puzzleblock/images $(DEST)/puzzleblock/images


MAIN = puzzleblock/PuzzleBlock

run: $(DEST)/$(MAIN).class
	$(JVM) -Djava.library.path=$(DEST)/puzzleblock -cp $(DEST) $(MAIN)

clean:
	$(RM) -r $(DEST)/puzzleblock

