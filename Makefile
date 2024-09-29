all:
	g++ -o dist/main -std=c++17 src/main.cpp src/memory.cpp src/commands.cpp src/parser.cpp src/errors.cpp