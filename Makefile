build:
	g++ ./main.cpp ./utils/*.cpp ./core/*.cpp ./user/core.hpp -o ./gx
buildPM:
	g++ ./package-manager/main.cpp -o ./gxpm
