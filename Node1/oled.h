#ifndef OLED_H
#define OLED_H

#include <avr/io.h>

typedef enum {LARGE, NORMAL, SMALL} fonts;
typedef struct {int page, col;} oled_position;
	
void initOLED(void); // Run this on boot-up

// Position
uint8_t oled_goto_page(uint8_t page);
uint8_t oled_goto_col(uint8_t col);
void oled_pos(uint8_t page, uint8_t col);
void oled_home(void); // Sets position 0,0 (upper left corner)
void oled_nl(void); // New line function (next page all the way to the left) 
void oled_indent(int length); // Indents a defined length from left to right 

// Clearing
void oled_clear_page(void); // Clears the current page
void oled_clear_spage(int page); // Clears a specified page
void oled_clear_specific(int row, int col); // Clears a specific column in a specific row.
void oled_reset(void); // Clears the whole screen, position is set to home afterwards

// Writing
void oled_set_font(fonts font); // Sets the font. LARGE, NORMAL, SMALL
void oled_type(unsigned char c); // Types the inputted character based on the font-size and the contents of fonts
void oled_print(char string[]); // Prints the inputted string 
void oled_print_centered(char string[]); // Prints the string centered on the current page
void oled_print_left(char string[], int column);// Prints left-adjusted to a specified column
void oled_print_right(char string[], int column);// Prints right-adjusted to a specified column

// Drawing
void oled_draw_hline(int length, int thickness); // Prints a line with a defined length and thickness
void oled_draw_box(uint8_t xpos, uint8_t ypos, uint8_t w, uint8_t h, uint8_t thickness); // Draws a box with the specified starting position, width, height and thickness 
void oled_walking(uint8_t row, uint8_t col); // Animates a walking figure 
void oled_walkingDeletesAPage(uint8_t page);
void oled_moonwalkingDeletesAPage(uint8_t page);
void oled_draw_line(uint8_t startx,uint8_t starty,uint8_t endx,uint8_t endy); // Draws a line between two points
void oled_draw_circle(uint8_t center_x,uint8_t center_y, uint8_t radius);

// Printing special characters
void oled_elektra(void); 
void oled_penis(void);
void oled_rightarrow(void); // Prints a right arrow
void oled_leftarrow(void); // Prints a left arrow

// Test
void oled_test(void);

#endif