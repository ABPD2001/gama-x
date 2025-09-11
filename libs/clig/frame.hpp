#ifndef FRAME_HPP
#define FRAME_HPP
#include <iostream>
#include <sys/ioctl.h> // getting ioctl() system call
#include <unistd.h> // getting systemcal ioctl
#include <cstdint>
#include <cmath>
#include <string>
#include <vector>
#include <array>
#include <thread>

#define ESC '\033'  // ESC controll defined.
#define CSI ESC+'[' // defining controll axis.

using std::cin;
using std::cout;
using std::uint8_t;
using std::uint16_t;
using std::uint32_t;
using std::vector;
using std::array;
using std::thread;

// Copy right by ABPD2001: Abolfazl pouretemadi 2025.

namespace clig {

struct TableItem_t {
	vector<string> values;
	string title;
	Color_t color = Color_t{};
};

class Frame {
	private:
		// char **screen_buffer; it was a buffer to write in stdout, but we can use without buffer to not set every place of frame and reseting whole buffering.
		uint32_t columns = 0; // x axis, using 32-bit int to be more protable and optimized.
		uint16_t lines = 0; // y axis, using 16-bit int because its not required to be large like columns (rectangle shape of mintors).
		void _write_char_(uint32_t x, uint16_t y, char ch);

	public:
		char line_char = '-',rect_xaxis_char = '-',rect_yaxis_char = '|',bar_fill_char = '█', fill_rect_char = '█',bar_open_char = '[', bar_close_char = ']',empty_char = ' ',
                bar_empty_char = '-', rect_corner_char = '+';

		Frame();
		Frame(uint16_t lines,uint32_t columns);
		void clear(uint32_t x = 0, uint16_t y = 0, uint32_t width = -1, uint16_t height = -1);
		void line(uint32_t x,uint16_t y,uint32_t width, uint8_t stroke = 1);
		void rect(uint32_t x,uint16_t y ,uint32_t width, uint16_t height);
		void fill_rect(uint32_t x, uint16_t y, uint32_t width, uint16_t height, const uint8_t color = TEXT_COLOR_DEFAULT);
		void bar(uint32_t x,uint16_t y, uint8_t percentage,uint32_t width = -1, const uint8_t color = TEXT_COLOR_DEFAULT);
		inline void write(uint32_t x,  uint16_t y, char ch, Color_t color);
		inline void write(uint32_t x, uint16_t y, string text, Color_t color);
		void Frame::table(uint32_t x, uint16_t y, vector<TableItem_t> columns);
		void Frame::fill_rect(uint32_t x, uint16_t y, uint32_t width, uint16_t height, const uint8_t color);		
};

};
#endif
