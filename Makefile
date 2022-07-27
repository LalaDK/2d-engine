build:
	g++ -std=c++17 ./src/*.cpp ./src/Physics/*.cpp -I /opt/homebrew/include -L /opt/homebrew/lib -lSDL2 -lSDL2_image -lSDL2_gfx -lm -o ./dist/app

run:
	./dist/app
