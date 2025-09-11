#include "./terminal.hpp"

namespace clig {

	inline void _setCursor(uint32_t x, uint16_t y){
        	cout<<CSI<<x<<';'<<y<<"col"<<'H';
	}

	inline void _resetCursor(){
        	_setCursor(0,0);
	}

	bool setRawMode(bool mode){
		termios term;
		tcgetattr(STDIN_FILENO,&term);
		if(mode){
			tcgetattr(STDIN_FILENO, &term);
   			ttystate.c_lflag &= ~(ICANON | ECHO);
 			ttystate.c_cc[VMIN] = 1;
			ttystate.c_cc[VTIME] = 0; 
   			tcsetattr(STDIN_FILENO, TCSANOW, &term);
    			return fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK) == 0; 
		}

		term.c_lflag |= (ICANON | ECHO);
		tcsetattr(STDIN_FILENO, TCSANOW, &ttystate);
    		return fcntl(STDIN_FILENO, F_SETFL, fcntl(STDIN_FILENO, F_GETFL) & ~O_NONBLOCK) == 0;
	}

	inline void _setColor(const uint8_t color = TEXT_COLOR_DEFAULT,const uint8_t bkgColor = TEXT_BACKGROUND_COLOR_DEFAULT, const uint8_t mode = TEXT_DEFAULT) {
		cout<<CSI<<mode<<';'<<color<<';'<<bkgColor<<'m';
	}

	inline void _resetColor(){
		cout<<CSI<<"0m";
	}

};
