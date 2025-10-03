#include "./file.hpp"

File::File(){}
File::File(string path){
	this->status = (uint8_t) fs::exists(path);
	this->path = path;
}

bool File::check_status(){
	if(this->file.rdstate() != ios::goodbit){
		this->file.close();
		this->status = -1;
		return true;
	}
	return false;
}

string File::read(){
	string output = "";
	string temp_l;

	this->status = 0;
	
	this->file.open(this->path,ios::in);
	if(this->check_status()) return output;
	
	while(getline(this->file,temp_l)){
		output+=temp_l;
	}
	
	if(this->check_status()) return output;
	this->file.close();
	return output;
}

uint8_t File::write(string content){
	this->status = 0;
	
	this->file.open(this->path,ios::out | ios::app);
	if(this->check_status()) return this->status;
	
	this->file<<content;
	if(this->check_status()) return this->status;
	
	this->file.close();
	return this->status;
};

uint8_t File::overwrite(string content){
	this->status = 0;

	this->file.open(this->path,ios::out);
	if(this->check_status()) return this->status;

	this->file << content;
	if(this->check_status()) return this->status;
	
	this->file.close();
	return this->status;
}

template<typename T> uint8_t File::bin_read(T* output,size_t size){
	this->status = 0;

	this->file.open(this->path, ios::in | ios::binary);
	if(this->check_status()) return this->status;

	this->file.read((char*) output, size);
	if(this->check_status()) return this->status;

	this->file.close();
	return this->status;
}

template<typename T> uint8_t File::bin_write(T* content, size_t size){
	this->status = 0;

	this->file.open(this->path,ios::out | ios::binary);
	if(this->check_status()) return this->status;
	
	this->file.write((char*) content, size);
	if(this->check_status()) return this->status;

	this->file.close();
	return this->status;
}

bool File::ok(){
	return this->file.rdstate() == ios::goodbit && (!this->status);
}
