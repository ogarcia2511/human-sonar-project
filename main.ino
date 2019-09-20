#include <Adafruit_VL53L0X.h>


Adafruit_VL53L0X laser = Adafruit_VL53L0X();

VL53L0X_RangingMeasurementData_t dist;
const int button = 3;
const int headphones = 4;

int count = 0;
int freq;

float val[5];
float avg = 0;

void setup() 
{
  Serial.begin(9600);
  
  if (!laser.begin()) 
  {
    Serial.println(F("Failed to boot VL53L0X"));
    while(1);
  }

  pinMode(button, INPUT);
  pinMode(headphones, OUTPUT);
}

void loop() 
{  
  while(digitalRead(button) == HIGH)
  {
    Serial.println("pressed");
    laser.rangingTest(&dist, false); 
    // pass in 'true' to get debug data printout!

    // checs for failure
    if(dist.RangeStatus != 4) 
    {  
      // phase failures have incorrect data
      Serial.print("Distance (mm): "); 
      Serial.println(dist.RangeMilliMeter);
    }
    else 
    {
      Serial.println(" out of range ");
    }
    // takes a rolling average of values so that the tone is smoother
    val[count%5] = float(dist.RangeMilliMeter);
    for(int i = 0; i < 5; i++)
    {
      avg += val[i];
    }
    avg = avg / 5;
    freq = 3000 - (((avg - 1)/999) * 4500); //linear transform from range 1mm to 1000mm, inverts it so higher -> lower       

  tone(headphones, (int) freq, 200);
  // zeroes the average so it is unique between tones
  avg = 0;
  count++;
  delay(10);
  }
}
