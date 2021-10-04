#ifndef INCLUDES_H
#include "includes.h"
#endif
#include "fonts.h"

#ifndef OLED_CMD
#define OLED_CMD 0x1000 //The address room for OLED command
#endif

#ifndef OLED_DATA
#define OLED_DATA 0x1200 //The address room for OLED data
#endif

#define height 64
#define width 128
#define num_pages 8
#define SRAM_address_start 0

static FILE mystdout = FDEV_SETUP_STREAM(oled_type, NULL, _FDEV_SETUP_WRITE);

volatile char * OLED_CMD_val = (char *) OLED_CMD;
volatile char * OLED_DATA_val = (char *) OLED_DATA;

uint8_t selected_font = 5;

typedef enum{PAGE, HORIZONTAL, VERTICAL} modes;
volatile oled_position position; //Defines the row/col position of the writer

void oled_test(void){
		oled_reset();
		oled_set_font(LARGE);
		oled_penis();
}


//Writes the input command to the command register of the OLED
void writeCMD(uint8_t cmd){
	OLED_CMD_val[0] = cmd;
}

//Writes the input data to the data register of the OLED
void writeDATA(uint8_t data){
	OLED_DATA_val[0] = data;
}

// Writes data to SRAM with address based on current page and column 
void writeDataSRAM(uint8_t data) {
	uint8_t address = SRAM_address_start+position.col+width*position.page;
	writeSRAM(address, data);
}

// Prints entire screen from SRAM memory
void printFromSRAM(void) {
	for (uint16_t address = SRAM_address_start; address < SRAM_address_start+(width * num_pages); address++) {
		uint8_t data = readSRAM(address);
		oled_pos(address / width, address % width);
		writeDATA(data);
	}
}

//Initialization
void initOLED(void){
	setBit(MCUCR, SRE);	
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
	for(uint8_t i=0; i <(num_commands); i++){
		writeCMD(RecInitCommands[i]);
	}
	
}
// ------------------------------------- Typing, writing and drawing things on the screen --------------------



void oled_set_font(fonts font){
	switch(font){
		case(LARGE):
			selected_font = 8;
			break;
		case(NORMAL):
			selected_font = 5;
			break;
		case(SMALL):
			selected_font = 4;
			break;
	}
	//return selected_font; // trengs vel ikke?
};


//Typing characters

void oled_type(uint8_t c){
	int printChar = c-32;
	switch(selected_font){
		case(4):
			for(int a = 0; a<4; a++){
				writeDATA(pgm_read_word(&font4[printChar][a])); 
				position.col += 4;
				}
			break;
		case(5):
			for(int b = 0; b<5; b++){
				writeDATA(pgm_read_word(&font5[printChar][b])); 
				position.col +=5;
				}
			break;
		case(8):
			for(int d = 0; d<8; d++){
				writeDATA(pgm_read_word(&(font8[printChar][d])));
				position.col +=8;
				}
			break;
			
	}
}
void oled_printf(char* data, ...){
	va_list args;
	va_start(args, data);
	vfprintf(&mystdout, data, args);
	va_end(args);
}

//Printing a string
void oled_print(char string[]){
	for(int i=0; i<strlen(string);i++){
		oled_type(string[i]);
	}
}

//Prints the string centered column wise on the current page
void oled_print_centered(char string[]){
	int half = (strlen(string)*selected_font)/2;
	uint8_t column = 64 - half;
	oled_goto_col(column);
	oled_print(string);
}	

//Prints left-adjusted and moves pointer down. 
void oled_print_left(char string[], int column){
	oled_goto_col(column);
	oled_print(string);
}

//Writing special characters. 
void oled_elektra(void){for(int i = 0; i<8; i++){writeDATA(pgm_read_word(&specialSymbols[1][i]));} position.col +=8;}
void oled_penis(void){for(int i = 0; i<8; i++){writeDATA(pgm_read_word(&specialSymbols[0][i]));} position.col +=8;}
void oled_rightarrow(void){for(int i = 0; i<8; i++){writeDATA(pgm_read_word(&specialSymbols[2][i]));} position.col +=8;}
void oled_leftarrow(void){for(int i = 0; i<8; i++){writeDATA(pgm_read_word(&specialSymbols[3][i]));} position.col +=8;}

//---------------------------------------------------------Drawing stuff ---------------------------------------

void oled_draw_hline(int length, int thickness){
	for(int i = 0; i<length-1;i++){
		writeDATA(thickness);
	}
}


void oled_draw_box(uint8_t xpos, uint8_t ypos, uint8_t w, uint8_t h, uint8_t thickness){
	uint8_t start_page = ypos/8;
	uint8_t extra_top = ypos - start_page*8;
	uint8_t needed_pages = (extra_top + h +(8-1))/8;
	uint8_t extra_bottomn = needed_pages*8 - (extra_top + h);
	uint8_t total_height = extra_top + h + extra_bottomn;
	
	
	bool raw_square[total_height][w];
	for(int h1=0; h1<total_height; h1++){
		for(int w1=0; w1<w; w1++){
			raw_square[h1][w1]=0;
		}
	}
	
	//filling the square
	for(int h2 = extra_top; h2<(total_height-extra_bottomn); h2++){
		for(int w2 = 0; w2 < w; w2++){
			raw_square[h2][w2]=1;
		}
	}
	
	//Removing the insides
	for(int h3 = extra_top + thickness; h3 < (total_height-extra_bottomn-thickness); h3++){
		for(int w3 = thickness; w3 <(w-thickness); w3++){
			raw_square[h3][w3]=0;
		}
	}
	
	//Converting raw_sqare writable bytes.
	uint8_t out_square[needed_pages][w];
	for(int p0 = 0; p0<needed_pages; p0++){
		for(int c0 = 0; c0<w; c0++){
			uint8_t byte = 0;
			uint8_t start = (p0*8);
			uint8_t end = (p0*8)+8;
			uint8_t bits[8] = {1,2,4,8,16,32,64,128};
			for(int h4=start;h4 < end;h4++){
				if(raw_square[h4][c0]==1){
	              byte = byte + bits[h4-start];
				}
			}
			out_square[p0][c0] = (uint8_t)byte;
		}
	}
	

	
	
	//Writing it to the oled;
	uint8_t writebyte;
	for(int p1=0;p1<needed_pages; p1++){
		oled_pos(start_page+p1,xpos);
		for(int c1=0; c1<w; c1++){
			writebyte = out_square[p1][c1];
			writeDATA(writebyte);
		}
	}
	//printing the square
	/*
	for(int h0=0; h0<total_height; h0++){
		printf("[");
		for(int w0=0; w0<w; w0++){
			printf("%d ",raw_square[h0][w0]);
		}
		printf("]\n\r");
	}
	
	printf("\n\r\n\r");
	for(int p1=0; p1<needed_pages; p1++){
		printf("[");
		for(int c1=0; c1<w; c1++){
			printf("%d ",out_square[p1][c1]);
		}
		printf("]\n\r");
	}
	*/
}
// ----------------------------------------- Cleaning the screen ------------------------------------------------

//Clear the current page
void oled_clear_page(void){
	oled_goto_col(0); //Moves to the 0 collumn
	for(int i=0; i<128; i++){writeDATA(0x00);} //Writes a 0 in every column
}

//Clear a specific page
void oled_clear_spage(int page){ oled_goto_page(page); oled_clear_page();}

//Clear specific pixel
void oled_clear_specific(int row, int col){
	oled_pos(row, col);
	writeDATA(0x00);
}

//Reset the whole screen.
void oled_reset(void){
	for(int i=0; i<8; i++){ oled_clear_spage(i); } //Loops through all pages and clears them
	oled_home(); //Sets the position to 0,0
}


//----------------------------------------------------- Position related -----------------------------------------------------


void set_addressing_mode(modes mode){
	writeCMD(0x20); //Tells the OLED that we are going to change the address mode
	//next bit is the addressing mode (reference: manual page 27)
	switch(mode){
		case(PAGE):
			writeCMD(0b00000010);
			break;
		case(HORIZONTAL):
			writeCMD(0b00000000);
			break;
		case(VERTICAL):
			writeCMD(0b00000001);
			break;
	}
}

uint8_t oled_goto_page(uint8_t page){
	if (page >  7 || page < 0){ //returns 0 if input is illegal
		return 0;
	}
	else {
		position.page = page; //stores the page position in the position struct
		set_addressing_mode(PAGE);
		writeCMD(0xB0 + page); //Commands the OLED to change to the specified page
		set_addressing_mode(HORIZONTAL);
	}
	return 0; // never reached
}

uint8_t oled_goto_col(uint8_t column){
	if (column > 127 || column < 0){
		return 0;
	}
	else {
		position.col = column;
		uint8_t min = column % 16;
		uint8_t max = column / 16;
		set_addressing_mode(PAGE);
		writeCMD(min);
		writeCMD(0x10 + max);
		set_addressing_mode(HORIZONTAL);
	}
	return 0; // never reached
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
