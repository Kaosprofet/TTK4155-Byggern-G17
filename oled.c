#ifndef INCLUDES_H
#include "includes.h"
#include "fonts.h"
#endif

#ifndef OLED_CMD
#define OLED_CMD 0x1000 //The address room for OLED command
#endif

#ifndef OLED_DATA
#define OLED_DATA 0x1200 //The address room for OLED data
#endif

#define height 64
#define width 128


volatile char * OLED_CMD_val = (char *) OLED_CMD;
volatile char * OLED_DATA_val = (char *) OLED_DATA;


volatile oled_position position; //Defines the row/col position of the writer


//Writes the input command to the command register of the OLED
void writeCMD(uint8_t cmd){
	OLED_CMD_val[0] = cmd;
}

//Writes the input data to the data register of the OLED
void writeDATA(uint8_t data){
	OLED_DATA_val[0] = data;
}

//Initialization
void initOLED(void){
	uint8_t RecInitCommands[] = { //From the OLED manual page 15 (Recommended initialization commands)
		0xae, //Display off
		0xa1, //Segment remap
		0xda, //Common pads hardware: alternative
		0x12, 0xc8, //common output scan direction:com63~com0
		0xa8, //multiplex ration mode:63
		0x3f, 0xd5, //display divide ratio/osc. freq. mode
		0x80, 0x81, //contrast control
		0x50, 0xd9, //set pre-charge period
		0x21, 0x20, //Set Memory Addressing Mode
		0x02, 0xdb, //VCOM deselect level mode
		0x30, 0xad, //master configuration
		0x00, 0xa4, //out follows RAM content
		0xa6, //set normal display
		0xaf, // display on
	};
	uint8_t num_commands = 22;
	
	//Writes all the commands
	for(uint8_t i=0; i <(num_commands -1); i++){
		writeCMD(RecInitCommands[i]);
	}
	
}
// ------------------------------------- Typing, writing and drawing things on the screen --------------------

uint8_t selected_font = 5;

void oled_set_font(fonts font){
	switch(font){
		case(LARGE):
		selected_font = 8;
		case(NORMAL):
		selected_font = 5;
		case(SMALL):
		selected_font = 4;
	}
};


//Typing characters

void oled_type(char c){
	uint8_t printChar = 32 - c;
	switch(selected_font){
		case(4):
			for(int i = 0; i<3; i++){
				int byte = font4[printChar][i];
				writeDATA(byte);
				position.col += 4;
				}
		case(5):
			for(int i = 0; i<4; i++){writeDATA(pgm_read_word(&font5[printChar][i])); position.col +=5;}
		case(8):
			for(int i = 0; i<7; i++){writeDATA(pgm_read_word(&font8[printChar][i])); position.col +=8;}
	}
}


void oled_print(char string[]){
	for(int i=0; i<strlen(string)-1;i++){
		oled_type(string(i));
	}
}
//Writing the elektra logo
void oled_elektra(void){
	for(int i = 0; i<8; i++){writeDATA(pgm_read_word(&specialSymbols[1][i])); position.col +=8;}
}
void oled_penis(void){
	for(int i = 0; i<8; i++){writeDATA(pgm_read_word(&specialSymbols[0][i])); position.col +=8;}
}

void oled_hline(int length, int thickness){
	for(int i = 0; i<length-1;i++){
		writeDATA(thickness);
	}
}

// ----------------------------------------- Cleaning the screen ------------------------------------------------

//Clear the current page
void oled_clear_page(void){
	oled_goto_col(0); //Moves to the 0 collumn
	for(int i=0; i<128; i++){writeDATA(0);} //Writes a 0 in every column
}

//Clear a specific page
void oled_clear_spage(int page){ oled_goto_page(page); oled_clear_page();}

//Clear specific pixel
void oled_clear_specific(int row, int col){
	oled_pos(row, col);
	writeDATA(0);
}

//Reset the whole screen.
void oled_reset(void){
	for(int i=0; i<7; i++){ oled_clear_spage(i); } //Loops through all pages and clears them
	oled_home(); //Sets the position to 0,0
}


//----------------------------------------------------- Position related -----------------------------------------------------
typedef enum{PAGE, HORIZONTAL, VERTICAL} modes;

void set_addressing_mode(modes mode){
	writeCMD(0x20); //Tells the OLED that we are going to change the address mode
	//next bit is the addressing mode (reference: manual page 27)
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
		writeCMD(0xB0 + page); //Commands the OLED to change to the specified page
		set_addressing_mode(HORIZONTAL);
	}
}

void oled_goto_col(uint8_t col){
	if (col > 127 || col < 0){
		return 0;
	}
	else {
		position.col = col;
		uint8_t min = col % 16;
		uint8_t max = col / 16;
		set_addressing_mode(PAGE);
		writeCMD(min);
		writeCMD(0x10 + max);
		set_addressing_mode(HORIZONTAL);
	}
}

void oled_pos(uint8_t page, uint8_t col){
	oled_goto_page(page);
	oled_goto_col(col);
} //combines the two functions above

void oled_home(void){
	oled_pos(0,0); //Move to page 0 column 0
}

void oled_nl(void){ //Moves the cursor to a new page
	oled_pos(position.page + 1, 0);
}

void oled_indent(int length){
	oled_pos(position.page, position.col + length);
}