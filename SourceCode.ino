// ECE 3872
// Team 2A
// BUZZ BOX
// Generated by Proteus Visual Designer for Arduino

// Peripheral Configuration Code (Do Not Edit)
//---CONFIG_BEGIN---
#pragma GCC push_options
#pragma GCC optimize ("Os")

#include <core.h> // Required by cpu
#include <cpu.h>
#include <spi.h> // Required by SD1
#include <WaveStore.h>
#include <Generic.h>
#include <Wire.h> // Required by KEYPAD1
#include <MCP23008.h> // Required by KEYPAD1
#include <Keypad.h>
#include <WavePlayer.h>

#pragma GCC pop_options

// Peripheral Constructors
CPU &cpu = Cpu;
GenericButton PLAYBACK = GenericButton (A1);
GenericButton RECORD = GenericButton (7);
GenericButton CLEAR = GenericButton (8);
Keypad_MCP23008 KEYPAD1 = Keypad_MCP23008 (1, 1, 1, 4, 3, "123456789*0#");
GenericButton STOP = GenericButton (A2);
GenericBuzzer BUZ1 = GenericBuzzer (6);
WavePlayer &AUDIO1 = WP;
WaveStore &SD1 = WS;

void peripheral_setup () {
 KEYPAD1.begin ();
 AUDIO1.begin (2, 3, 4, 5);
 SD1.begin (10);
}

void peripheral_loop() {
}
//---CONFIG_END---
// Global Variables
bool var_main;
bool var_recording;
String var_key;
String var_Notes;
bool var_Playing;
long var_Mode;
bool var_tmp;

// --------------------------------------------------------
// chart_SETUP()
// 
// Description:
//   Main loop, idles when var_Mode is 0. Waits for
//   var_Mode to be updated by buttons PLAYBACK or RESET.
// --------------------------------------------------------
void chart_SETUP() {
 var_main=true;
 while (var_main) {
  var_Mode=0;
  while (var_Mode==0) {
  }
  if(var_Mode==1) {
   chart_Record();
  }
  if(var_Mode==2) {
   chart_Playback();
  }
 }
}

// --------------------------------------------------------
// chart_Record()
// 
// Description:
//   Recording loop. Waits for input note to append to
//   current note list. Breaks out of loop when STOP or
//   CLEAR buttons are pressed.
// --------------------------------------------------------
void chart_Record() {
 var_recording=true;
 while (var_recording) {
  var_key=KEYPAD1.getKey(true);
  if(var_recording==false) {
   var_key="";
  }
  var_Notes=var_Notes+var_key;
 }
}

// --------------------------------------------------------
// chart_Playback()
// 
// Description:
//   Playback loop. Plays current notes and moves Buzz with
//   a unique movement per each note. When all notes have
//   been played Buzz will return to his starting position
//   and playback will start over. Breaks out of loop when
//   STOP or CLEAR buttons are pressed.
// --------------------------------------------------------
void chart_Playback() {
 var_Playing=true;
 while (var_Playing) {
  AUDIO1.play("audio.wav",true);
 }
}

// Entry Points and Interrupt Handlers
void setup () { peripheral_setup();  chart_SETUP(); }
void loop () { peripheral_loop(); }
