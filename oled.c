#include "functions.h"
#include "oled.h"
#include "fonts.h"
#include <avr/io.h>

#ifndef OLED_CMD
#define OLED_CMD 0x1000 //The address room for OLED command
#endif

#ifndef OLED_DATA
#define OLED_DATA 0x1200 //The address room for OLED data
#endif

#define height 64
#define width 128


volatile char * OLED_CMD_val = (char *)OLED_CMD; 
volatile char * OLED_DATA_val = (char *)OLED_DATA;


volatile oled_position position; //Defines the row/col position of the writer

//Local functions
void writeCMD(uint8_t cmd){OLED_CMD_val[0] = cmd;} //Writes the input command to the command register of the OLED
void writeDATA(uint8_t data){OLED_DATA_val[0] = data;} //Writes the input data to the data register of the OLED





void initOLED(void){
	uint8_t RecInitCommands[] = { //From the OLED manual page 15 (Recommended initialization commands)
		0xae, //Display off
		0xa1, //Segment remap
		0xda, //Common pads hardware: alternative
		0x12,
		0xc8, //common output scan direction:com63~com0
		0xa8, //multiplex ration mode:63
		0x3f,
		0xd5, //display divide ratio/osc. freq. mode
		0x80,
		0x81, //contrast control
		0x50,
		0xd9, //set pre-charge period
		0x21, 
		0x20, //Set Memory Addressing Mode
		0x02,
		0xdb, //VCOM deselect level mode
		0x30,
		0xad, //master configuration
		0x00, 
		0xa4, //out follows RAM content 
		0xa6, //set normal display
		0xaf, // display on
	};
	uint8_t num_commands = 21;
	for(uint8_t i=0; i <(num_commands -1); i++){ //Writes all the commands
		writeCMD(RecInitCommands[i]);
	}	
	
}

void printString(char inString){
	
}


//----------------------------------------------------- Position related -----------------------------------------------------
typedef enum{PAGE, HORIZONTAL, VERTICAL} modes;

void set_addressing_mode(modes mode){
	writeCMD(0x20); //Tells the OLED that we are going to change the address mode
	//next bit is the addressing mode (reference manual page 27)
	switch(mode){
		case(PAGE):
			writeCMD(2);
		case(HORIZONTAL):
			writeCMD(0);
		case(VERTICAL):
			writeCMD(1);
	}
}

void oled_goto_page(uint8_t page){
	if (page >  7 || page < 0){ //returns 0 if input is illegal  
		return 0;
	}
	else {
		position.page = page; //stores the page position in the position struct
		set_addressing_mode(PAGE);
		writeCMD(0xB0 + page); //Sends the memory address page to the OLED.
		set_addressing_mode(HORIZONTAL);
	}
}

void oled_goto_col(uint8_t col){
	if (col > 127 || col < 0){
		return 0;
	}
	else {
		position.col = col;
		set_addressing_mode(PAGE);
		//writeCMD();
		set_addressing_mode(HORIZONTAL);
	}
}

void oled_pos(uint8_t page, uint8_t col){oled_goto_page(page); oled_goto_col(col);}