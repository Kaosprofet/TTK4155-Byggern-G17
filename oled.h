#include <functions.h>
#include <avr/io.h>


void initOLED(void);
void writeCMD(uint8_t cmd);
void writeDATA(uint8_t data);
void printString(char inString);

void oled_goto_page(uint8_t page);
void oled_goto_col(uint8_t col);
void oled_pos(uint8_t page, uint8_t col);
void oled_reset();
void oled_home();
void oled_print(char*);

typedef struct {int page, int col;} oled_position;