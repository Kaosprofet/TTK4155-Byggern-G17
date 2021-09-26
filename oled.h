#include <avr/io.h>

typedef enum {LARGE, NORMAL, SMALL} fonts;
typedef struct {int page, col;} oled_position;
	
void initOLED(void); //Run this on boot-up

//Position
void oled_goto_page(uint8_t page);
void oled_goto_col(uint8_t col);
void oled_pos(uint8_t page, uint8_t col);
void oled_home(void); // Sets position 0,0 (upper left corner)
void oled_nl(void); //New line function (next page all the way to the left) 
void oled_indent(int length); //Indents a defined length from left to right 

//Clearing
void oled_clear_page(void); //Clears the current page
void oled_clear_spage(int page); //Clears a specified page
void oled_clear_specific(int row, int col); //Clears a specific column in a specific row.
void oled_reset(void); //Clears the whole page, position is set to home afterwards

//Writing
void oled_set_font(fonts font); //Sets the font. LARGE, NORMAL, SMALL
void oled_type(char c); //Types the inputted character based on the font-size and the contents of fonts
void oled_print(char string[]); //Prints the inputted string 

//Drawing
void oled_draw_hline(int length, int thickness); //Prints a line with a defined length and thickness
void oled_draw_box(int x, int y, int w, int h, int thick); //Draws a box with the specified starting position **
//Printing special characters
void oled_elektra(void); 
void oled_penis(void);
