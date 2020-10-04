int noteButtonPin = A1;
int speaker = 4;

void setup()
{
	Serial.begin(9600);
}

void loop()
{
	int input = analogRead(noteButtonPin);
	Serial.println(input);
	
	/*
	Input must be tested in person to determine limiting values
	
	if (input < 100)
	{
		Serial.println("Button 1")
		tone(speaker, 110, 1000); //A2
	}
	
	else if (input < 200)
	{
		Serial.println("Button 2")
		tone(speaker, 123.471, 1000); //B2
	}
	
	else if (input < 300)
	{
		Serial.println("Button 3")
		tone(speaker, 130.813, 1000); //C3
	}
	
	else if (input < 400)
	{
		Serial.println("Button 4")
		tone(speaker, 146.832, 1000); //D3
	}
	
	else if (input < 500)
	{
		Serial.println("Button 5")
		tone(speaker, 164.814, 1000); //E3
	}
	
	else if (input < 600)
	{
		Serial.println("Button 6")
		tone(speaker, 174.614, 1000); //F3
	}
	
	else if (input < 700)
	{
		Serial.println("Button 7")
		tone(speaker, 195.998, 1000); //G3
	}
	
	else if (input < 800)
	{
		Serial.println("Button 8")
		tone(speaker, 207.652, 1000); //G#3
	}
	*/
}