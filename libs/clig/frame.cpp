#include "./core.hpp"

namespace clig {

	Frame::Frame(){
		winsize window;

		ioctl(STDOUT_FILENO, TIOCGWINSZ, &window); // using input-output systemcall to get window size in automatic constructor mode.
		this->columns = window.ws_col;
		this->lines = window.ws_row;
	}

	Frame::Frame(uint16_t lines, uint32_t columns){ // constructor in manual mode.
		this->columns = columns;
		this->lines = lines;
	}

	void Frame::_write_char_(uint32_t x, uint16_t y, char ch){
		_setCursor(x,y);
		cout<<ch;
	}

	inline void Frame::write(uint32_t x, uint16_t y, char ch, Color_t color = Color_t{}){
		_setColor(color.color, color.background, color.mode);
		this->_write_char_(x,y,ch);
		_resetCursor();
		_resetColor();
	}

	void Frame::clear(uint32_t x, uint16_t y, uint32_t width, uint16_t height){
		if(width == -1) width = this->columns;
		if(height == -1) height = this->lines;

		_resetCursor();
		for(uint16_t i = 0;i<y;i++){
			for(uint32_t j = 0;j<x;j++){
				this->_write_char_(j,i,this->empty_char);
			}
		}
		_resetCursor();
	}

	void Frame::line(uint32_t x, uint16_t y, uint32_t width, uint8_t stroke){
		for(int strk = 0; strk<stroke; strk++){
			for(uint32_t i = x+1;i < width+1+x;i++){
				this->_write_char_(i,strk*y,this->line_char);
			}
		}
		_resetCursor();
	}

	void Frame::rect(uint32_t x, uint16_t y, uint32_t width, uint16_t height){
		this->_write_char_(x,y,this->rect_corner_char);
		this->_write_char_(x+width,y,this->rect_corner_char);
		this->_write_char_(x,y+height,this->rect_corner_char);
		this->_write_char_(x+width,y+height, this->rect_corner_char);

		for(int xaxis = x+1; xaxis<width+x-1; xaxis++){
			this->_write_char_(xaxis, y, this->rect_xaxis_char);
			this->_write_char_(xaxis,y+height, this->rect_xaxis_char);
		}
		for(int yaxis = y+1; yaxis<height+y-1; yaxis++){
			this->_write_char_(x,yaxis, this->rect_yaxis_char);
			this->_write_char_(x+width, yaxis, this->rect_yaxis_char);
		}

		_resetCursor();
	}

	void Frame::bar(uint32_t x, uint16_t y, uint8_t percentage,uint32_t width, const uint8_t color){
		if(width == -1) width = this->columns;
		this->_write_char_(x,y,this->bar_open_char);
		this->_write_char_(x+width-1,y,this->bar_close_char);
		_setColor(color);
		for(uint8_t count = 0; count<floor(width*percentage);count++){
			this->_write_char_(count+x,y,this->bar_fill_char);
		}
		_resetColor();
		_resetCursor();
	}

	inline void Frame::write(uint32_t x, uint16_t y, string text, Color_t color = Color_t{}){
		_setCursor(x,y);
		_setColor(color.color, color.background, color.mode);
		cout<<text;
		_resetCursor();
		_resetColor();
	}

	void Frame::table(uint32_t x, uint16_t y, vector<TableItem_t> columns){
		vector<uint8_t> biggest_words;
		uint16_t biggest_col_height = 0;

		for(vector<TableItem_t>::iterator itr = columns.begin(); itr != columns.end(); ++itr){
			uint8_t biggest_word = 0;
			for(vector<string>::iterator vitr = itr->values.begin(); vitr != its->values.end(); ++vitr){
				if(biggest_word < vitr->length()) biggest_word = vitr->length();
			}
			if(biggest_word < itr->title.length()) biggest_word = itr->title.length();
			if(biggest_col_height < itr->values.size(); ++itr) biggest_col_height = itr->values.size();
			biggest_words.push_back(biggest_word);
		}
	
	
		uint32_t cur_x = x+1;
		uint16_t cur_y = y+1;
	
		for(uint8_t i = 0; i<columns.size();  i++){
			_setCursor(cur_x+1, cur_y);
			cout<<columns[i].title;
		
			for(uint8_t j = 0; j<columns[i].values.length(); j++){
				_setCursor(cur_x-1, 4+j);
				cout<<columns[i].values[j];
			}

			for(uint32_t j = cur_x; j<cur_x+biggest_words[i]+1; j++){
				this->_write_char_(j, y, this->line_char);
				this->_write_char_(j,y+2,this->line_char);
				this->_write_char_(j,y+4+biggest_col_height);
			}
			this->_write_char_(cur_x-1, y,this->rect_corner_char);
			this->_write_char_(cur_x-1, y+2, this->rect_corner_char);
			this->_write_char_(cur_x-1, y+4+biggest_col_height, this->rect_corner_char);
		
			this->_write_char_(cur_x-1, y+1, this->rect_yaxis_char);
			for(uint8_t j = 0;j<bigger_col_height; j++){
				this->_write_char_(cur_x-1, y+3+j, this->rect_yaxis_char);
			}
		
			cur_x += biggest_words[i]+2;
		}
		this->_write_char_(cur_x-1, y,this->rect_corner_char);
        	this->_write_char_(cur_x-1, y+2, this->rect_corner_char);
        	this->_write_char_(cur_x-1, y+4+biggest_col_height, this->rect_corner_char);	
	
		this->_write_char_(cur_x-1, y+1, this->rect_yaxis_char);
		for(uint8_t i = 0; i<biggest_col_height; i++){
			this->_write_char_(cur_x-1, y+3+i, this->rect_yaxis_char);
		}
	
		_resetCursor();
	}
	void Frame::fill_rect(uint32_t x, uint16_t y, uint32_t width, uint16_t height, const uint8_t color){
		_setColor(color);
		for(uint16_t h = y; h<y+h; h++){
		for(uint32_t w = x; w<x+width; w++){
			this->_write_char_(w,h,this->fill_rect_char);
		}
		}
		_resetCursor();
		_resetColor();
	}
	
};

