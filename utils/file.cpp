#include "./file.hpp"

File::File(){}
File::File(string path){
	this->status_code = (uint8_t) fs::exists(path);
	this->path = path;
}

bool File::check_status(){
	if(this->file.rdstate() != ios::goodbit){
		this->status = this->file.rdstate();
		this->file.close();
		this->status_code = -1;
		return true;
	}
	return false;
}

string File::read(){
	string output = "";
	string temp_l;

	this->init_status();
	
	this->file.open(this->path,ios::in);
	if(this->check_status()) return output;
	
	while(getline(this->file,temp_l)){
		this->output+=temp_l;
	}
	
	if(this->check_status()) return output;
	this->file.close();
	return output;
}

iostate File::write(string content){
	this->init_status();
	
	this->file.open(this->path,ios::out | ios::app);
	if(this->check_status()) return this->status;
	
	this->file<<content;
	if(this->file.check_status()) return this->status;
	
	this->file.close();
	return this->status;
};

iostate File::overwrite(string content){
	this->init_status();

	this->file.open(this->path,ios::out);
	if(this->file.check_status()) return this->status;

	this->file << content;
	if(this->file.check_status()) return this->status;
	
	this->file.close();
	return this->status;
}

template<typename T> iostate File::bin_read(T* output,size_t size){
	T output;
	this->init_status();

	this->file.open(this->path, ios::in | ios::binary);
	if(this->file.check_status()) return this->status;

	this->file.read((char*) output, size);
	if(this->file.check_status()) return this->status;

	this->file.close();
	return this->state;
}

template<typename T> iostate File::bin_write(T* content, size_t size){
	this->init_status();

	this->file.open(this->path,ios::out | ios::binary);
	if(this->file.check_status()) return this->status;
	
	this->file.write((char*) value, size);
	if(this->file.check_status()) return this->status;

	this->file.close();
	return this->status;
}

bool File::ok(){
	return this->state == ios::goodbit && (!this->status_code);
}

void File::init_status(){
	this->status_code = 0;
	this->status = ios::goodbit;
}
