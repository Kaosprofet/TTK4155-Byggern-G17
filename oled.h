#include "functions.h"
#include <avr/io.h>


void initOLED(void);
void printString(char inString);

void oled_goto_page(uint8_t page);
void oled_goto_col(uint8_t col);
void oled_pos(uint8_t page, uint8_t col);
void oled_reset();
void oled_home();
void oled_print(char*);
void oled_clear_page(void);

typedef struct {int page, col;} oled_position;