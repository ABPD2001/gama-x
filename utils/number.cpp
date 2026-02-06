#include "number.hpp"

uint32_t to_uint32(string text){
        uint32_t output = 0;

        for(uint8_t i = 0; i<text.length(); i++){
                cout<<text[i]-48<<"\n";
		output+=(text[i]-48)*pow(10,i);
        }

        return output;
};
