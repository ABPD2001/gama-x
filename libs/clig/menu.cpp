#include "./menu.hpp"

namespace clig {
	Menu::Menu(uint32_t x, uint16_t y, vector<Button_t> buttons, MenuStyle_t style){
		this->x = x;
		this->y = y;
		this->style = style;

		if(!this->dataRender) this->_default_data_render_();
		else this->dataRender(this->buttons, this->style);

		this->buttons = buttons;
	}

	void Menu::_default_render_buttons_(){
		uint8_t biggest_btn = 0;

                for(vector<Button_t>::iterator it = this->buttons.begin(); it != this->buttons.end(); ++it){
                	if(*it > biggest_btn) biggest_btn = *it;
                }

                for(uint8_t i = 0; i < this->buttons.size(); i++){
                	for(uint8_t w = x; w<biggest_btn+x; w++){
                        	if(i == this->cur_idx) _setColor(this->buttons[i].hover.color,this->buttons[i].hover.background,this->buttons[i].hover.mode);
                                else _setColor(this->buttons[i].color.color,this->buttons[i].color.background,this->buttons[i].color.mode);
				
				if(this->style.vertical) _setCursor(w,y+i+(i*this->style.gap));
				else {
					uint16_t back_offset = 0;
					for(uint8_t j = 0; j<i; j++){
						back_offset += this->buttons[j].title.length() + this->style.padding + this->style.gap;
						if(this->style.prefix) back_offset++;
						if(this->style.suffix) back_offset++;
					}

					_setCursor(w+back_offset,y);
				}

				cout << this->style.prefix << this->buttons[i].title << this->style.suffix;			
				
                        }
                }
		_resetCursor();

	}

	void Menu::_default_data_render_(){
		uint8_t biggest_btn = 0;
                
		for(vector<Button_t>::iterator it = buttons.begin(); it != buttons.end(); ++it){
                        if(biggest_btn < it->title.length()) biggest_btn = it->title.length();
                }

                for(vector<Button_t>::iterator it = buttons.begin(); it != buttons.end(); ++it){
                        const uint8_t offset = it->title.length();

                        if(offset) {
                                const uint8_t left_space = (uint8_t) trunc(offset/2);
                                const uint8_t right_space = (uint8_t) ceil(offset/2);
                                for(uint8_t s = 0; s<left_space; j++){
                                        it->title = ' '+it->title;
                                }
                                for(uint8_t s = 0;s<right_space; j++){
                                        it->title = it->title+' ';
                                }
                        }

                }
	}

	void Menu::_default_period_handler_(){
		char keyboad_key;

                while(1){
                        cin.get(&keyboard_key);
                        if(cin.fail()) {
                                cin.clear();
                                return;
                        }

                        switch(keybaord_key){
                                case '\n':
                                        onButtonClickHandler(this->buttons[this->cur_idx].id);
                                        break;
                                case '\033[5~':
                                        this->cur_idx++;
                                        if(this->cur_idx+1 > this->buttons.size()) this->cur_idx = 0;
					if(!this->cur_idx+1) this->cur_idx = this->buttons.size()-1;
                                        break;
                        }

                        if(!this->renderButtons) this->_default_render_buttons_();
			else this->renderButtons(this->buttons, this->style, this->cur_idx);
                        usleep(period*1000);
                }

	}

	void Menu::join(){
		
	}
};
