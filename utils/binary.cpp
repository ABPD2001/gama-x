#include "./binary.hpp"

string to_binary(uint32_t value){
	string output = "";
	
	for(uint8_t i = 1; i<33; i++){
		const int current_dig = pow(2,32-i);
		output += (value >= current_dig ? '1' : '0');
		if (value > current_dig) value -= current_dig;
	}

	return output;
}

string to_binary64(uint64_t value){
	string output = "";
	for(uint8_t i = 1; i<65; i++){
		const int current_dig = pow(2,64-i);
		output += (value > current_dig ? '1' : '0');
		if (value > current_dig) value -= current_dig;
	}

	return output;
}

uint32_t from_binary32(string value){
	uint32_t output = 0;

	for(uint8_t i = 0; i<32; i++){
		if (value[i] == '1') output+=pow(2,32-(i+1));
	}
	
	return output;
}

uint8_t from_binary8(string value){
	uint8_t output = 0;
	
	for(uint8_t i = 0; i<8; i++){
		if(value[i] == '1') output+=pow(2,8-i-1);
	}
	return output;
}
