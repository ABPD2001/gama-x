build:
	g++ ./main.cpp ./utils/*.cpp ./core/*.cpp -o ./gx

devBuild:
	g++ -fsanitize=address -g ./main.cpp ./utils/*.cpp ./core/*.cpp -o ./gx

buildPM:
	g++ ./package-manager/main.cpp ./utils/*.cpp ./core/*.cpp -o ./gxpm

devBuildPM:
	g++ -fsanitize=address -g ./package-manager/main.cpp ./utils/*.cpp ./core/*.cpp -o ./gxpm