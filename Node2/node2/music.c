/*Code and inspiration used from ILiviu at
https://www.programming-electronics-diy.xyz/2021/02/playing-music-and-tones-using-piezo.html


/* 
  Songs available at https://github.com/robsoncouto/arduino-songs                                            
                                              
                                              Robson Couto, 2019
*/
#ifndef INCLUDES_H
#include "includes.h"
#endif

int game_over_value = 0;


// notes of the melody followed by the duration.
// a 4 means a quarter note, 8 an eighteenth , 16 sixteenth, so on
// !!negative numbers are used to represent dotted notes,
// so -4 means a dotted quarter note, that is, a quarter plus an eighteenth!

static void music_set_melody_values(int frequency){
  pwm_select_frequency(frequency); 
}

static void tone(int frequency, int duration){
  music_set_melody_values(frequency);
  delay_ms(duration);
}


void play_music(const int* melody, int tempo, int amount_of_notes){

// sizeof gives the number of bytes, each int value is composed of two bytes (16 bits)
// there are two values per note (pitch and duration), so for each note there are four byte
int notes = amount_of_notes;
// this calculates the duration of a whole note in ms (60s/tempo)*4 beats
int wholenote = 240000 / tempo;

int divider = 0, noteDuration = 0, note_pause = 0, thisNote = 0;
  // iterate over the notes of the melody. 
  // Remember, the array is twice the number of notes (notes + durations)
      	
  for (thisNote = 0; thisNote < notes*2; thisNote = thisNote + 2) {
    can_decode_message();
    if (!music.play && game_over_value == 0){
      break;
    }
    // calculates the duration of each note
    divider = melody[thisNote + 1];
    if (divider > 0) {
      // regular note, just proceed
      noteDuration = (wholenote) / divider;
    } else if (divider < 0) {
      // dotted notes are represented with negative durations!!
      noteDuration = -1.5*(wholenote) /(divider);
    }
      // we only play the note for 90% of the duration, leaving 10% as a pause
    tone(melody[thisNote], noteDuration*0.9);
    // Wait for the specief duration before playing the next note.
    note_pause = noteDuration*0.10;
    // stop the waveform generation before the next note.
    tone(0, note_pause);
  }
}

void song_select(void){
  while (music.play){
    if (music.melody == 1){
      int amount_of_notes = sizeof(key_board_cat_melody)/sizeof(key_board_cat_melody[0])/2; //Calculates size of notesheet
	  game_over_value = 0; //Used to seperate between high score and game over 
      play_music(key_board_cat_melody, key_board_cat_tempo,amount_of_notes);//Plays the song
	}
    else if (music.melody == 2){ //Plays rick roll
      int amount_of_notes = sizeof(Never_gonna_give_u_up_melody)/sizeof(Never_gonna_give_u_up_melody[0])/2; //Calculates size of notesheet
	    game_over_value = 0; //Used to seperate between high score and game over 
      play_music(Never_gonna_give_u_up_melody, rick_astely_tempo,amount_of_notes);//Plays the song
    }
  }
}

void game_over_music(void){ //Plays game over music
  int amount_of_notes = sizeof(game_over_melody)/sizeof(game_over_melody[0])/2; //Calculates size of notesheet
  game_over_value = 1; //Used to seperate between high score and game over 
  play_music(game_over_melody, game_over_music_tempo,amount_of_notes); //Plays the song
}
