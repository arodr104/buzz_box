// ECE 3872
// Team 2A
// BUZZ BOX

#include <Stepper.h>
#include <math.h>
#include "SparkFun_TB6612.h"
#define STEPS 200
#define NOTELIMIT 80

// Pin assignments
int PLAY = 10;
int STOP = 13;
int RECORD = 12;
int noteButtonPin = A1;
int speaker = 11;

//Motor DC_motor = Motor(7, 8, 9, 1, 2);
Stepper stepper(STEPS, 1 ,0, 4, 5);

// Struct to hold note information
struct noteNode {
  float duration;
  float note;
  bool silent;
  String name;
  
  noteNode* next;
};

int noteCount = 0;

// Head and tail pointers for note list
noteNode* tail = new noteNode();
noteNode* head = tail;

void setup()
{
  pinMode(PLAY, INPUT_PULLUP);
  pinMode(STOP, INPUT_PULLUP);
  pinMode(RECORD, INPUT_PULLUP);
  pinMode(3, OUTPUT);
  pinMode(6, OUTPUT);
  digitalWrite(3, HIGH);
  digitalWrite(6, HIGH);
  stepper.setSpeed(115);
  //Serial.begin(9600);
}

void loop()
{
	int startPlay = digitalRead(PLAY);
	int startRec = digitalRead(RECORD);

	if (startPlay == LOW)
	  playing();

	else if (startRec == LOW)
	  recording();

	//Serial.println("IDLE");
  stepper.step(1);
  //DC_motor.drive(200);
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
   
	////Serial.println("PLAYING");
	
	// Playing Loop
	while (1)
	{
		noteNode* curr = head;
		
		while (curr != NULL)
		{
			//Serial.println(curr->name);
      //Serial.println(curr->duration);
			
			// Don't play when note is silent
			if (!curr->silent)
      {
			  tone(speaker, curr->note, curr->duration);
        //DC_motor.drive(200);
        //stepper.step(15);
      }
        
			float startNote = millis();
			
			// Delay for the length of note
			while (millis() - startNote < curr->duration)
			{
				int stopPlaying = digitalRead(STOP);

				if (stopPlaying == LOW)
					return;		
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
  ////Serial.println("RESET");
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
			
			//Serial.println("RECORDING");
			int stopRecording = digitalRead(STOP);
			input = analogRead(noteButtonPin);
			//Serial.println(input);
			if (stopRecording == LOW)
			  return;
		  
			// Note has been pressed
			if (input != 1023)
			{
				notePressed = silenceEnd = millis();
				break;
			}
		}
		
		// Add silent note
    if (noteCount <= NOTELIMIT)
    {
  		tail->name = "Silent";
  		tail->duration = silenceEnd - silenceStart;
  		tail->silent = true;
  		tail->next = new noteNode();
  		tail = tail->next;
  		noteCount++;
		}
		
		float note;
		String noteName;
		
		// Note is being held
		while (input != 1023)
		{
			if (input == 0) // A2
			{
				noteName = "A2";
				note = 110;
				//Serial.println("A2");
				//tone(speaker, 110, 100);
			}

			else if (input > 500 && input < 520) // B2
			{
				noteName = "B2";
				note = 123.471;
				//Serial.println("B2");
				//tone(speaker, 123.471, 100);
			}

			else if (input > 670 && input < 700) // C3
			{
				noteName = "C3";
				note = 130.813;
				//Serial.println("C3");
				//tone(speaker, 130.813, 100); 
			}

			else if (input > 750 && input < 800) // D3
			{
				noteName = "D3";
				note = 146.832;
				//Serial.println("D3");
				//tone(speaker, 146.832, 100); 
			}

			else if (input > 810 && input < 830) // E3
			{
				noteName = "E3";
				note = 164.814;
				//Serial.println("E3");
				//tone(speaker, 164.814, 100); 
			}

			else if (input > 850 && input < 860) // F3
			{
				noteName = "F3";
				note = 174.614;
				//Serial.println("F3");
				//tone(speaker, 174.614, 100); 
			}

			else if (input > 875 && input < 885) // G3
			{
				noteName = "G3";
				note = 195.998;
				//Serial.println("G3");
				//tone(speaker, 195.998, 100); 
			}

			else if (input > 880 && input < 900) // G#3
			{
				noteName = "G#3";
				note = 207.652;
				//Serial.println("G#3");
				//tone(speaker, 207.652, 100); 
			}

			input = analogRead(noteButtonPin);
		}
		
		noteReleased = millis();
		
		// Add note
   if (noteCount <= NOTELIMIT)
   {
  		tail->name = noteName;
  		tail->note = note;
  		tail->duration = noteReleased - notePressed;
  		tail->silent = false;
  		tail->next = new noteNode();
  		tail = tail->next;
  		noteCount++;
		}
	}
}
