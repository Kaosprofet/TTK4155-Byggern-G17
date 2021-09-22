#include "functions.h"
#include <avr/io.h>


void initOLED(void); //Run this on bootup

void oled_goto_page(uint8_t page);
void oled_goto_col(uint8_t col);
void oled_pos(uint8_t page, uint8_t col);
void oled_home(void); // Sets position 0,0 (upper left corner)
void oled_clear_page(void); //Clears the current page
void oled_clear_spage(int page); //Clears a specified page
void oled_reset(void); //Clears the whole page, position is set to home afterwards
void oled_type(char c); 
void oled_print(char*);  



typedef struct {int page, col;} oled_position;