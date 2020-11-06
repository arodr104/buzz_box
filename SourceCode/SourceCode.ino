// ECE 3872
// Team 2A
// BUZZ BOX

#include <Stepper.h>
#include "SparkFun_TB6612.h"
#define STEPS 200
#define NOTELIMIT 110
#define stepperLimit 160

// Pin assignments
int PLAY = 10;
int RESET = A2;
int PLAYLIVE = A4;
int STOP = 13;
int RECORD = 12;
int noteButtonPin = A1;
int speaker = 11;

Motor DC_motor = Motor(7, 8, 1, 2);
Stepper stepper(STEPS, 1, 0, 4, 5);

// Struct to hold note information
struct noteNode {
  float duration;
  float note;
};

noteNode noteArray[NOTELIMIT];
float silentNotes[NOTELIMIT];
int noteIndex = 0;
int silentIndex = 0;
int noteCount = 0;
int stepperBasePOS = 0;
int stepperPOS = 0;

void setup()
{
  noteArray[0].note = -1;
    
  pinMode(PLAY, INPUT_PULLUP);
  pinMode(STOP, INPUT_PULLUP);
  pinMode(RECORD, INPUT_PULLUP);
  pinMode(PLAYLIVE, INPUT_PULLUP);
  pinMode(RESET, INPUT_PULLUP);
  pinMode(3, OUTPUT);
  pinMode(6, OUTPUT);
  digitalWrite(3, HIGH);
  digitalWrite(6, HIGH);
  stepper.setSpeed(20);
}

void loop()
{
	int startPlay = digitalRead(PLAY);
	int startRec = digitalRead(RECORD);
  int playLive = digitalRead(PLAYLIVE);
  int resetBox = digitalRead(RESET);

	if (startPlay == LOW)
	  playing();

	else if (startRec == LOW)
	  recording();

  else if (playLive == LOW)
    live();

  else if (resetBox == LOW)
    restart();
}

// ---------------------------------------------------------------------------
// playing()
// 
// Description:
//   Playback loop. Plays current notes and moves Buzz with a unique movement
//   per each note (movement is yet to be added). Recorded notes are played by
//   traversing the recording linked list. When all notes have been played
//   Buzz will return to his starting position and playback will start over.
//   Breaks out of loop when STOP is pressed.
// ---------------------------------------------------------------------------
void playing()
{
	// Empty List
	if (noteArray[0].note == -1)
		return;
   
  int i = 0;
  int stopPlaying;
	
	// Playing Loop
	while (1)
	{
		//noteNode* curr = head;
		while (i <= noteIndex)
		{
      // No sound when not note was pressed
      float startSilent = millis();
      
      while (millis() - startSilent < silentNotes[i])
      {
        stopPlaying = digitalRead(STOP);

        if (stopPlaying == LOW)
          return;
      }
           
			float startNote = millis();
      int stepperDirection;

      // Choose stepper motor direction based on the current note
      if (stepperPOS == 0 && noteArray[i].note < 150)
      {
          stepperDirection = 1;
      }
      else if (noteArray[i].note < 150)
      {
        stepperDirection = -1;
      }
      else
      {
        stepperDirection = 1;
      }

			// Delay for the length of note
			while (millis() - startNote < noteArray[i].duration)
			{
        tone(speaker, noteArray[i].note, 50);

        if (stepperPOS >= stepperLimit && stepperDirection == 1)
          stepperDirection -= 2;
          
        else if (stepperPOS <= 0 && stepperDirection == -1)
          stepperDirection += 2;
          
        stepper.step(stepperDirection);
        DC_motor.drive(1);
        stepperPOS += stepperDirection;

				stopPlaying = digitalRead(STOP);

				if (stopPlaying == LOW)
					return;		
			}
     
			i++;
		}

    i = 0;
	}
}

// ---------------------------------------------------------------------------
// recording()
// 
// Description:
//   Recording loop. Waits for input note to append to current note list.
//   Silence before a note is pressed is added as a note. Breaks out of loop
//   when STOP is pressed.
// ---------------------------------------------------------------------------
void recording()
{
	float notePressed;
	float noteReleased;
	float silenceStart;
	float silenceEnd;
	int input;
	
	// Recording Loop
	while (1)
	{
		silenceStart = millis();
		
		// No note pressed
		while (1) 
		{
			int stopRecording = digitalRead(STOP);
			input = analogRead(noteButtonPin);
			if (stopRecording == LOW)
			  return;
		  
			// Note has been pressed
			if (input < 1000)
			{
				notePressed = silenceEnd = millis();
				break;
			}
		}
   
    silentNotes[silentIndex++] = silenceEnd - silenceStart;

    if (silentIndex >= NOTELIMIT)
      silentIndex--;
		
		float note;
		String noteName;
		
		// Note is being held
		while (input != 1023)
		{
			if (input < 300) // A2
			{
				note = 110;
				tone(speaker, 110, 100);
			}

			else if (input > 450 && input < 600) // B2
			{
				note = 123.471;
				tone(speaker, 123.471, 100);
			}

			else if (input > 600 && input < 720) // C3
			{
				note = 130.813;
				tone(speaker, 130.813, 100); 
			}

			else if (input > 730 && input < 790) // D3
			{
				note = 146.832;
				tone(speaker, 146.832, 100); 
			}

			else if (input > 800 && input < 840) // E3
			{
				note = 164.814;
				tone(speaker, 164.814, 100); 
			}

			else if (input > 845 && input < 860) // F3
			{
				note = 174.614;
				tone(speaker, 174.614, 100); 
			}

			else if (input > 861 && input < 885) // G3
			{
				note = 195.998;
				tone(speaker, 195.998, 100); 
			}

			else if (input > 885 && input < 950) // G#3
			{
				note = 207.652;
				tone(speaker, 207.652, 100); 
			}

			input = analogRead(noteButtonPin);
		}
		
		noteReleased = millis();
		
		// Add note
   if (noteIndex <= NOTELIMIT)
   {
      noteArray[noteIndex].note = note;
      noteArray[noteIndex].duration = noteReleased - notePressed;
      noteIndex++;
  		noteCount++;
		}
	}
}

void live()
{
    // Note is being held
    while (1)
    {
      if (digitalRead(STOP) == LOW)
        return;
        
      int input = analogRead(noteButtonPin);
      
      if (input < 300) // A2
      {
        tone(speaker, 110, 100);
        int stepperDirection = -1;

        if (stepperPOS <= 0)
          stepperDirection += 2;

        
        stepper.step(stepperDirection);
        stepperPOS += stepperDirection;
        DC_motor.drive(1);
      }

      else if (input > 450 && input < 600) // B2
      {
        tone(speaker, 123.471, 100);
        int stepperDirection = -1;

        if (stepperPOS <= 0)
          stepperDirection += 2;

        stepper.step(stepperDirection);
        stepperPOS += stepperDirection;
        DC_motor.drive(1);
      }

      else if (input > 600 && input < 720) // C3
      {
        tone(speaker, 130.813, 100); 
        int stepperDirection = -1;

        if (stepperPOS <= 0)
          stepperDirection += 2;

        stepper.step(stepperDirection);
        stepperPOS += stepperDirection;
        DC_motor.drive(1);
      }

      else if (input > 730 && input < 790) // D3
      {
        tone(speaker, 146.832, 100); 
        int stepperDirection = -1;

        if (stepperPOS <= 0)
          stepperDirection += 2;

        stepper.step(stepperDirection);
        stepperPOS += stepperDirection;
        DC_motor.drive(1);
      }

      else if (input > 800 && input < 840) // E3
      {
        tone(speaker, 164.814, 100); 
        int stepperDirection = 1;

        if (stepperPOS >= stepperLimit)
          stepperDirection -= 2;

        stepper.step(stepperDirection);
        stepperPOS += stepperDirection;
        DC_motor.drive(1);
      }

      else if (input > 845 && input < 860) // F3
      {
        tone(speaker, 174.614, 100); 
        int stepperDirection = 1;

        if (stepperPOS >= stepperLimit)
          stepperDirection -= 2;

        stepper.step(stepperDirection);
        stepperPOS += stepperDirection;
        DC_motor.drive(1);
      }

      else if (input > 861 && input < 885) // G3
      {
        tone(speaker, 195.998, 100); 
        int stepperDirection = 1;

        if (stepperPOS >= stepperLimit)
          stepperDirection -= 2;

        stepper.step(stepperDirection);
        stepperPOS += stepperDirection;
        DC_motor.drive(1);
      }

      else if (input > 885 && input < 950) // G#3
      {
        tone(speaker, 207.652, 100);
        int stepperDirection = 1;

        if (stepperPOS >= stepperLimit)
          stepperDirection -= 2;

        stepper.step(stepperDirection);
        stepperPOS += stepperDirection;
        DC_motor.drive(1); 
      }
    }
}

void restart()
{
  float startTime = millis();

  while (millis() - startTime < 3000)
  {
    if (digitalRead(RESET) == HIGH)
    {
      while(stepperPOS > 0)
      {
        stepper.step(-1);
        stepperPOS--;
      }

      return;
    }
  }
  
  noteArray[0].note = -1;
  noteIndex = 0;
  silentIndex = 0;
  delay(2000);
}
