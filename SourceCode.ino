// ECE 3872
// Team 2A
// BUZZ BOX

int PLAY = 10;
//int LIVE = 11;
int STOP = 12;
int RECORD = 13;
int MODE = 0; // Current Mode of Operation

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
  while(MODE == 0)
  {
    int startPlay = digitalRead(PLAY);
    int startRec = digitalRead(RECORD);

    if (startPlay == LOW)
      playing();

    else if (startRec == LOW)
      recording();
    
    Serial.println("IDLE");
  }
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
  while (1)
  {
    int stopRecording = digitalRead(STOP);
  
    if (stopRecording == LOW)
      return;
      
    Serial.println("RECORDING");
  }
}