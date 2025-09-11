#ifndef MENU_HPP
#define MENU_HPP
#include <string>
#include <vector>
#include <thread>
#include <cmath>
#include  "unistd.h"
#include "./terminal.hpp"

using std::vector;
using std::string;
using std::thread;
using std:mutex;
using std::lock_gaurd;
using std::uint16_t;
using std::uint32_t;
using std::uint8_t;
using std::trunc;
using std::ceil;

namespace clig {
	struct Button_t {
        	string id;
        	string title;
        	Color_t color = Color_t{};
        	Color_t hover = Color_t{TEXT_COLOR_DEFAULT, TEXT_BACKGROUND_COLOR_CYAN};
        	Color_t active = Color_t{TEXT_COLOR_DEFAULT, TEXT_BACKGROUND_COLOR_BLUE};
	};

	struct MenuStyle_t {
		uint8_t gap = 2;
		uint8_t btn_padding = 0;
		const char btn_prefix = '[';
		const char btn_suffix = ']';
		bool vertical = true;
	};

	class Menu {
		private:
			vector<Button_t> buttons;
			void (*onButtonClick)(string id);
			void (*periodHandler)(Menu* self) = nullptr;
			void (*renderButtons)(vector<Button_t> buttons, Style_t styling, int cur_idx) = nullptr;
			vector<Button_t> (*dataRender)(vector<Button_t>, Style_t styling) = nullptr;
			MenuStyle_t style;
			uint32_t x = 0;
			uint16_t y = 0;
			uint8_t period = 75;
			uint8_t idx = 0;
			bool stop = true;

			void _default_render_buttons_();
			void _default_data_render_();
			void _default_period_handler_();
		public:
			Menu(uint32_t x, uint16_t y, vector<Button_t> buttons, MenuStyle_t style = MenuStyle_t{});
			void join();
			void detach();
			void stop();
	};
};
#endif
