#ifndef INCLUDES_H
#include "includes.h"
#endif

/*Code and inspiration used from ILiviu at
https://www.programming-electronics-diy.xyz/2021/02/playing-music-and-tones-using-piezo.html


/* 
  Songs available at https://github.com/robsoncouto/arduino-songs                                            
                                              
                                              Robson Couto, 2019
*/
#define zelda_tempo 88
#define rick_astely_tempo 114

// notes of the moledy followed by the duration.
// a 4 means a quarter note, 8 an eighteenth , 16 sixteenth, so on
// !!negative numbers are used to represent dotted notes,
// so -4 means a dotted quarter note, that is, a quarter plus an eighteenth!

void music_set_melody_values(int frequency, float duty_cycle){
  pwm_select_frequency(frequency); //Must run frequency first as duty uses frequency in calculation
  pwm_selct_duty_cycle(duty_cycle);
  delay_us(100);
}


void play_music(const int* melody, int tempo){
// sizeof gives the number of bytes, each int value is composed of two bytes (16 bits)
// there are two values per note (pitch and duration), so for each note there are four bytes
int notes=220/sizeof(melody[0])/2; 

// this calculates the duration of a whole note in ms (60s/tempo)*4 beats
int wholenote = (60000 * 4) / tempo;

int divider = 0, noteDuration = 0;
  // iterate over the notes of the melody. 
  // Remember, the array is twice the number of notes (notes + durations)
  for (int thisNote = 0; thisNote < notes * 2; thisNote = thisNote + 2) {

    // calculates the duration of each note
    divider = melody[thisNote + 1];
    if (divider > 0) {
      // regular note, just proceed
      noteDuration = (wholenote) / divider;
    } else if (divider < 0) {
      // dotted notes are represented with negative durations!!
      noteDuration = (wholenote) / abs(divider);
      noteDuration *= 1.5; // increases the duration in half for dotted notes
    }
      // we only play the note for 90% of the duration, leaving 10% as a pause
    music_set_melody_values(melody[thisNote], noteDuration*0.9);

    // Wait for the specief duration before playing the next note.
    float note_pause = noteDuration*0.10;
    
    // stop the waveform generation before the next note.
    music_set_melody_values(0, note_pause);
  }
}

