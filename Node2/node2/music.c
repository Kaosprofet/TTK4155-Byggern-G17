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

void music_set_melody_values(int frequency){
  pwm_select_frequency(frequency); //Must run frequency first as duty uses frequency in calculation
  pwm_selct_duty_cycle(0.5);
}

void music_buzzer(int frequency, int duration){
  music_set_melody_values(frequency);
  delay_ms(duration);
}


void play_music(const int* melody, int tempo){
// sizeof gives the number of bytes, each int value is composed of two bytes (16 bits)
// there are two values per note (pitch and duration), so for each note there are four bytes
int notes=sizeof(melody)/sizeof(melody[0])/2;

// this calculates the duration of a whole note in ms (60s/tempo)*4 beats
int wholenote = 240000 / tempo;

int divider = 0, noteDuration = 0;
  // iterate over the notes of the melody. 
  // Remember, the array is twice the number of notes (notes + durations)
  for (int thisNote = 0; thisNote < notes*2; thisNote = thisNote + 2) {

    // calculates the duration of each note
    divider = melody[thisNote + 1];
    if (divider > 0) {
      // regular note, just proceed
      noteDuration = (wholenote) / divider;
    } else if (divider < 0) {
      // dotted notes are represented with negative durations!!
      noteDuration = (wholenote) /(divider);
      noteDuration *= 1.5; // increases the duration in half for dotted notes
    }
      // we only play the note for 90% of the duration, leaving 10% as a pause
    music_buzzer(melody[thisNote], noteDuration*0.9);
    // Wait for the specief duration before playing the next note.
    int note_pause = noteDuration*0.10;
    // stop the waveform generation before the next note.
    music_buzzer(0, note_pause);
  }
}

