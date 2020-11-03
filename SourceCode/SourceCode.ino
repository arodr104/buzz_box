// ECE 3872
// Team 2A
// BUZZ BOX

#include <Stepper.h>
#include "SparkFun_TB6612.h"
#define STEPS 200
#define NOTELIMIT 110

// Pin assignments
int PLAY = 10;
int STOP = 13;
int RECORD = 12;
int noteButtonPin = A1;
int speaker = 11;

Motor DC_motor = Motor(7, 8, 9, 1, 2);
Stepper stepper(STEPS, A4, A2, 4, 5);

// Struct to hold note information
struct noteNode {
  float duration;
  float note;
  //noteNode* next;
};

noteNode noteArray[NOTELIMIT];
float silentNotes[NOTELIMIT];
int noteIndex = 0;
int silentIndex = 0;
int noteCount = 0;
int stepperBasePOS = 0;
int stepperPOS = 0;

// Head and tail pointers for note list
noteNode* tail = new noteNode();
noteNode* head = tail;

void setup()
{
  for (float num : silentNotes)
    num = 0;
    
  pinMode(PLAY, INPUT_PULLUP);
  pinMode(STOP, INPUT_PULLUP);
  pinMode(RECORD, INPUT_PULLUP);
  pinMode(3, OUTPUT);
  pinMode(6, OUTPUT);
  digitalWrite(3, HIGH);
  digitalWrite(6, HIGH);
  stepper.setSpeed(20);
  Serial.begin(9600);
}

void loop()
{
	int startPlay = digitalRead(PLAY);
	int startRec = digitalRead(RECORD);

	if (startPlay == LOW)
	  playing();

	else if (startRec == LOW)
	  recording();

	Serial.println("IDLE");
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
	if (head == NULL)
		return;
   
	Serial.println("PLAYING");
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
      Serial.print("Silent Duration: ");
      Serial.println(silentNotes[i]);
      
      while (millis() - startSilent < silentNotes[i])
      {
        stopPlaying = digitalRead(STOP);

        if (stopPlaying == LOW)
          return;
      }
      
      //Serial.println(curr->duration);      
			float startNote = millis();
			Serial.print("Note Duration: ");
      Serial.println(noteArray[i].duration);
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

      int stepperLimit = 200;
			// Delay for the length of note
			while (millis() - startNote < noteArray[i].duration)
			{
        tone(speaker, noteArray[i].note, 50);
        DC_motor.drive(1);

        if (stepperPOS >= stepperLimit && stepperDirection == 1)
          stepperDirection -= 2;
          
        else if (stepperPOS <= 0 && stepperDirection == -1)
          stepperDirection += 2;
          
        Serial.println(stepperPOS);
        stepper.step(stepperDirection);
        stepperPOS += stepperDirection;

				stopPlaying = digitalRead(STOP);

				if (stopPlaying == LOW)
					return;		
			}

      DC_motor.brake();
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
			Serial.println(input);
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
				noteName = "A2";
				note = 110;
				Serial.println("A2");
				//tone(speaker, 110, 100);
			}

			else if (input > 450 && input < 600) // B2
			{
				noteName = "B2";
				note = 123.471;
				Serial.println("B2");
				//tone(speaker, 123.471, 100);
			}

			else if (input > 600 && input < 720) // C3
			{
				noteName = "C3";
				note = 130.813;
				Serial.println("C3");
				//tone(speaker, 130.813, 100); 
			}

			else if (input > 730 && input < 790) // D3
			{
				noteName = "D3";
				note = 146.832;
				Serial.println("D3");
				//tone(speaker, 146.832, 100); 
			}

			else if (input > 800 && input < 840) // E3
			{
				noteName = "E3";
				note = 164.814;
				Serial.println("E3");
				//tone(speaker, 164.814, 100); 
			}

			else if (input > 845 && input < 860) // F3
			{
				noteName = "F3";
				note = 174.614;
				Serial.println("F3");
				//tone(speaker, 174.614, 100); 
			}

			else if (input > 861 && input < 885) // G3
			{
				noteName = "G3";
				note = 195.998;
				Serial.println("G3");
				//tone(speaker, 195.998, 100); 
			}

			else if (input > 885 && input < 950) // G#3
			{
				noteName = "G#3";
				note = 207.652;
				Serial.println("G#3");
				//tone(speaker, 207.652, 100); 
			}

			input = analogRead(noteButtonPin);
		}
		
		noteReleased = millis();
		
		// Add note
   if (noteIndex <= NOTELIMIT)
   {
  		/*tail->note = note;
  		tail->duration = noteReleased - notePressed;
  		tail->next = new noteNode();
  		tail = tail->next;*/
      noteArray[noteIndex].note = note;
      noteArray[noteIndex].duration = noteReleased - notePressed;
      noteIndex++;
  		noteCount++;
		}
	}
}
