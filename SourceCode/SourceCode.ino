// ECE 3872
// Team 2A
// BUZZ BOX

#include <Stepper.h>
#include "SparkFun_TB6612.h"
#define STEPS 200

// Pin assignments
int PLAY = 10;
int STOP = 12;
int RECORD = 13;
int noteButtonPin = A1;
int speaker = 4;

Motor DC_motor = Motor(7, 8, 9, 1, 11);
Stepper stepper(STEPS, 2, 3, 5, 6);

// Struct to hold note information
struct noteNode {
  float duration;
  float note;
  bool silent;
  String name;
  
  noteNode* next;
};

// Head and tail pointers for note list
noteNode* tail = new noteNode();
noteNode* head = tail;

void setup()
{
  pinMode(PLAY, INPUT_PULLUP);
  pinMode(STOP, INPUT_PULLUP);
  pinMode(RECORD, INPUT_PULLUP);
  stepper.setSpeed(115);
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
		
	float startNote;
	float endNote;
	Serial.println("PLAYING");
	
	// Playing Loop
	while (1)
	{
		noteNode* curr = head;
		
		while (curr != NULL)
		{
			Serial.println(curr->name);
			
			// Don't play when note is silent
			if (!curr->silent)
      {
			  tone(speaker, curr->note, curr->duration);
        //DC_motor.drive(200);
        stepper.step(15);
      }
        
			startNote = endNote = millis();
			
			// Delay for the length of note
			while (endNote - startNote < curr->duration)
			{
				int stopPlaying = digitalRead(STOP);

				if (stopPlaying == LOW)
					return;
				
				endNote = millis();
			}

      //DC_motor.brake();
			curr = curr->next;
		}
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
			
			Serial.println("RECORDING");
			int stopRecording = digitalRead(STOP);
			input = analogRead(noteButtonPin);
			
			if (stopRecording == LOW)
			  return;
		  
			// Note has been pressed
			if (input != 0)
			{
				notePressed = silenceEnd = millis();
				break;
			}
		}
		
		// Add silent note
		tail->name = "Silent";
		tail->duration = silenceEnd - silenceStart;
		tail->silent = true;
		tail->next = new noteNode();
		tail = tail->next;
		
		float note;
		String noteName;
		
		// Note is being held
		while (input != 0)
		{
			if (input > 40 && input < 80) // A2
			{
				noteName = "A2";
				note = 110;
				Serial.println("Button 1");
				tone(speaker, 110, 100); 
			}

			else if (input > 90 && input < 120) // B2
			{
				noteName = "B2";
				note = 123.471;
				Serial.println("Button 2");
				tone(speaker, 123.471, 100); 
			}

			else if (input > 125 && input < 140) // C3
			{
				noteName = "C3";
				note = 130.813;
				Serial.println("Button 3");
				tone(speaker, 130.813, 100); 
			}

			else if (input > 150 && input < 170) // D3
			{
				noteName = "D3";
				note = 146.832;
				Serial.println("Button 4");
				tone(speaker, 146.832, 100); 
			}

			else if (input > 200 && input < 230) // E3
			{
				noteName = "E3";
				note = 164.814;
				Serial.println("Button 5");
				tone(speaker, 164.814, 100); 
			}

			else if (input > 280 && input < 340) // F3
			{
				noteName = "F3";
				note = 174.614;
				Serial.println("Button 6");
				tone(speaker, 174.614, 100); 
			}

			else if (input > 400 && input < 500) // G3
			{
				noteName = "G3";
				note = 195.998;
				Serial.println("Button 7");
				tone(speaker, 195.998, 100); 
			}

			else if (input > 1000 && input < 1100) // G#3
			{
				noteName = "G#3";
				note = 207.652;
				Serial.println("Button 8");
				tone(speaker, 207.652, 100); 
			}

			input = analogRead(noteButtonPin);
		}
		
		noteReleased = millis();
		
		// Add note
		tail->name = noteName;
		tail->note = note;
		tail->duration = noteReleased - notePressed;
		tail->silent = false;
		tail->next = new noteNode();
		tail = tail->next;
	}
}
