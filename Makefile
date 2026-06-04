buildAssembler:
	g++ ./assembler/main.cpp ./assembler/utils/*.cpp ./assembler/core/*.cpp -o ./gxa
buildLinker:
	g++ ./assembler/main.cpp ./assembler/utils/*.cpp ./assembler/core/*.cpp -o ./gxl

build:
	g++ ./assembler/main.cpp ./assembler/utils/*.cpp ./assembler/core/*.cpp -o ./gxl
	g++ ./assembler/main.cpp ./assembler/utils/*.cpp ./assembler/core/*.cpp -o ./gxa