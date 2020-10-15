// ECE 3872
// Team 2A
// BUZZ BOX

int PLAY = 10;
//int LIVE = 11;
int STOP = 12;
int RECORD = 13;
int noteButtonPin = A1;
int speaker = 4;

struct noteNode {
  float duration;
  float note;
  bool silent;
  String name;
  
  noteNode* next;
};

noteNode* tail = new noteNode();
noteNode* head = tail;

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
	if (head == NULL) // Empty List
		return;
		
	float startNote;
	float endNote;
	Serial.println("PLAYING");
	while (1) // Playing Loop
	{
		noteNode* curr = head;
		
		while (curr != NULL)
		{
			Serial.println(curr->name);

      if (!curr->silent)
			  tone(speaker, curr->note, curr->duration);
        
			startNote = endNote = millis();
			
			while (endNote - startNote < curr->duration) // Delay for the length of note
			{
				int stopPlaying = digitalRead(STOP);

				if (stopPlaying == LOW)
					return;
				
				endNote = millis();
			}
			
			curr = curr->next;
		}
	}
}

void recording()
{
	float notePressed;
	float noteReleased;
	float silenceStart;
	float silenceEnd;
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
		
    tail->name = "Silent";
		tail->duration = silenceEnd - silenceStart;
		tail->silent = true;
    tail->next = new noteNode();
    tail = tail->next;
		float note;
		String noteName;
		
		while (input != 0) // Note is being held
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
    tail->name = noteName;
		tail->note = note;
		tail->duration = noteReleased - notePressed;
		tail->silent = false;
    tail->next = new noteNode();
    tail = tail->next;
	}
}
