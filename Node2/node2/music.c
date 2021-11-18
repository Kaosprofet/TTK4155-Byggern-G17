#ifndef INCLUDES_H
#include "includes.h"
#endif

/*Code and inspiration used from ILiviu at
https://www.programming-electronics-diy.xyz/2021/02/playing-music-and-tones-using-piezo.html


/* 
  Songs available at https://github.com/robsoncouto/arduino-songs                                            
                                              
                                              Robson Couto, 2019
*/

// notes of the moledy followed by the duration.
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
    if (!music.play){
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
    int amount_of_notes = sizeof(mii_theme_notes)/sizeof(mii_theme_notes[0])/2;
    play_music(mii_theme_notes, MII_THEME_TEMPO,amount_of_notes);
  }
}
