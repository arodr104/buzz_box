#include <Stepper.h>
#include <SparkFun_TB6612.h>

int stepsPerRev = 100;
Stepper myStepper(stepsPerRev, 2, 4, 7, 8);
int stepCount = 0;
Motor DC_motor = Motor(0, 1, 3, 1, 9);

void setup()
{
	Serial.begin(9600);
}

void loop()
{
	//Stepper Motor Test
	while (stepCount < 5)
	{
		myStepper.step(1);
		stepCount++;
		Serial.print("Steps: ");
		Serial.println(stepCount);
		delay(500);
	}
	
	while (stepCount > 0)
	{
		myStepper.step(-1);
		stepCount--;
		Serial.print("Steps: ");
		Serial.println(stepCount);
		delay(500);
	}
	
	DC_motor.drive(200, 1000);
	DC_motor.drive(-200, 1000);
	DC_motor.brake();
	Serial.print("Done");
	delay(1000)
}