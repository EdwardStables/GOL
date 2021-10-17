build: build/olcPixelGameEngine.o src/game.cpp
	g++ -o GOL build/olcPixelGameEngine.o src/game.cpp \
		-lX11 -lGL -lpthread -lpng -lstdc++fs -std=c++17

build/olcPixelGameEngine.o: lib/olcPixelGameEngine.h lib/olcPixelGameEngine.cpp
	g++ -c lib/olcPixelGameEngine.cpp \
		-o build/olcPixelGameEngine.o \
		-lX11 -lGL -lpthread -lpng -lstdc++fs -std=c++17