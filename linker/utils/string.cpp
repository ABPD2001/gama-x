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

string join(vector<string> arr, string glue){
	string output = "";
	for(uint32_t i = 0; i<arr.size(); i++){
		output+=arr[i];
		if(i != arr.size()-1) output+=glue;
	}
	return output;
}

bool includes(char *arr, uint8_t count, char ch){
	for(uint8_t i = 0;i<count;i++){
		if(arr[i] == ch) return true;
	}
	return false;
}

string trim(string text){
	string output = "";
	signed long int start = -1;
	signed long int end = -1;
	char empties[7] = {'\t','\r',' ','\b','\f','\a','\v'};

	for(uint16_t i = 0; i<text.length(); i++){
		if(start == -1 && !includes(empties,7,text[i])) {
			start = i;
			break;
		}
	}

	for(uint16_t i = text.length()-1; i > -1; i--){
		if(end == -1 && !includes(empties,7,text[i])) {
			end = i;
			break;
		}
	}
	if(end == -1 || start == -1) return "";

	output = text.substr(start,end-start+1);
	string cpy = "";
	bool spaced = false;

	for(uint16_t i = 0; i<output.length(); i++){
		if(includes(empties,7,output[i])){
			if(!spaced) {
				spaced = true;
				cpy+=output[i];
			}
			else continue;
		} else {
			spaced = false;
			cpy+=output[i];
		}
	}

	return cpy;

}

string replaceAll(string text, string from, string to){
	string cpy = text;

	while(cpy.find(from) != string::npos){
		cpy.replace(cpy.find(from),to.length(), to);
	}

	return cpy;
}

string filter(string text, char from){
	string output = "";
	for(char ch:text){
		if(ch != from) output+=ch;
	}
	return output;
}

string filter(string text, char *chars,uint8_t count){
	string output = "";
	for(char ch:text){
		if(includes(chars,count,ch)) continue;
		output+=ch;
	}
	return output;
}
