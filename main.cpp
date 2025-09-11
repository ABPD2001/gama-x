#include <iostream>
#include <string>
#include <iostream>
#include "./libs/assembler.hpp"

using std::string;
using std::cout;
using std::cin;

int main(int argc, char *argv[]){
UVA_ASSEMBLER assembler ("./test.s","./test.uvb");
assembler.compile();
cout<<assembler.status<<"\n";
for (_uva_error_t err:assembler.errors){
	cout<<err.message<<"\n";
}
return 0;
}
