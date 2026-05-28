build:
	g++ ./main.cpp ./utils/*.cpp ./core/*.cpp -o ./gx

devBuild:
	g++ -fsanitize=address -g ./main.cpp ./utils/*.cpp ./core/*.cpp -o ./gx

buildPM:
	g++ ./package-manager/main.cpp ./utils/*.cpp ./package-manger/utils/*.cpp -o ./gxpm
