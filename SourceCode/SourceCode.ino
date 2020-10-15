#include "NoteList.h"

// ECE 3872
// Team 2A
// BUZZ BOX

int PLAY = 10;
//int LIVE = 11;
int STOP = 12;
int RECORD = 13;
int noteButtonPin = A1;
int speaker = 4;

noteNode* head = 0;
noteNode* tail = 0;

void setup()
{
  pinMode(PLAY, INPUT_PULLUP);
  //pinMode(LIVE, INPUT_PULLUP);
  pinMode(STOP, INPUT_PULLUP);
  pinMode(RECORD, INPUT_PULLUP);
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

void playing()
{
  while (1)
  {
    int stopPlaying = digitalRead(STOP);

    if (stopPlaying == LOW)
      return;
    
    Serial.println("PLAYING");
  }
}

void recording()
{
	float notePressed, noteReleased, silenceStart, silenceEnd;
	int input;
	
	while (1) // Recording Loop
	{
		silenceStart = millis();
    
		while (1) // No note pressed
		{
			
			Serial.println("RECORDING");
			int stopRecording = digitalRead(STOP);
			input = analogRead(noteButtonPin);
			
			if (stopRecording == LOW) // Add in last silence node
			  return;
		  
			if (input != 0) // Note has been pressed
			{
				notePressed = silenceEnd = millis();
				break;
			}
		}
		
		noteNode* silentNote = new noteNode();
		silentNote->duration = silenceEnd - silenceStart;
		silentNote->silent = true;
		addNote(head, tail, silentNote);
		float note;
		
		while (input != 0) // Note is being held
		{
			if (input > 40 && input < 80) // A2
			{
				note = 110;
				Serial.println("Button 1");
				tone(speaker, 110, 100); 
			}

			else if (input > 90 && input < 120) // B2
			{
				note = 123.471;
				Serial.println("Button 2");
				tone(speaker, 123.471, 100); 
			}

			else if (input > 125 && input < 140) // C3
			{
				note = 130.813;
				Serial.println("Button 3");
				tone(speaker, 130.813, 100); 
			}

			else if (input > 150 && input < 170) // D3
			{
				note = 146.832;
				Serial.println("Button 4");
				tone(speaker, 146.832, 100); 
			}

			else if (input > 200 && input < 230) // E3
			{
				note = 164.814;
				Serial.println("Button 5");
				tone(speaker, 164.814, 100); 
			}

			else if (input > 280 && input < 340) // F3
			{
				note = 174.614;
				Serial.println("Button 6");
				tone(speaker, 174.614, 100); 
			}

			else if (input > 400 && input < 500) // G3
			{
				note = 195.998;
				Serial.println("Button 7");
				tone(speaker, 195.998, 100); 
			}

			else if (input > 1000 && input < 1100) // G#3
			{
				note = 207.652;
				Serial.println("Button 8");
				tone(speaker, 207.652, 100); 
			}
		}
		
		noteReleased = millis();
		noteNode* newNote = new noteNode();
		newNote->note;
		newNote->duration = noteReleased - notePressed;
		newNote->silent = false;
		addNote(head, tail, newNote);
	}
}
