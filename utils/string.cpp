#include "./string.hpp"

vector<string> split(string text, char splitter){
        vector<string> output;
        string temp;

        for(char ch:text){
                if(ch == splitter) {
                        output.push_back(temp);
                        temp.clear();
                        continue;
                }
                temp+=ch;
        }
        output.push_back(temp);
        temp.clear();

        return output;
}

string replace_all(string text, char from, char to){
	string output = "";
	for (unsigned int i = 0; i<text.length(); i++){
		if (text[i] == from){
			output+=to;
			continue;
		}
		output+=text[i];
	}
	return output;
}
