//Library for the Humidity/Temperature sensor (DHT11)
#include <dht11.h>

//Instance Variables:

//Sensor Pins -
int moisturePin = 0;
int lightPin = 2;
int dhtPin = 4;

//Sensor Values -
int moisture;
int light;
float temperature;
int humidity;

//Sensor Indicators Pins -
int led = 13;
int tled = 12;
int lled = 8;
int hled = 7;

//RGB Led Pins -
int red = 9;
int green = 10;
int blue = 11;

//Button to change sensors -
int button = 4;

//Stores the index of the current sensor -
int mode = 1;

//Button input -
int mval;

//Stores the index of the current crop -
int crop = 1;

//VU Meter Pins -
int vu1 = 6;
int vu2 = 5;
int vu3 = 3;

//Warning Light Pin -
int warn = 2;

//Potentiometer (to change crops) Pin -
int pot = 3;

//Debounce variables to prevent multiple button presses -
long lastDebounceTime = 0;
long debounceDelay = 200;  

//Variables to store the minimum, ideal, maximum, and warning values for the moisture sensor, for the current crop
int mmin;
int mideal;
int mmax;
int mwarn;

//Variables to store ideal temperature and the range on either side for the current crop
double tideal;
int trange;

//Variables to store the minimum, ideal, maximum, and warning values for the light sensor, for the current crop
int lmin;
int lideal;
int lmax;
int lwarn;

//Used while switching crops
int previous = 1;

//Boolean variable to track whether the data is to be sent to the server or not
boolean sendData = false;

//dht11 object
dht11 DHT11;
#define DHT11PIN A1

void setup() 
{
  //Open the serial port:
  Serial.begin(9600); //It is used to transfer data to the python script
  
  //Setup all the LED pins for Output
  pinMode (led, OUTPUT);
  pinMode (tled, OUTPUT);
  pinMode (lled, OUTPUT);
  pinMode (hled, OUTPUT);
  pinMode (warn, OUTPUT);
  pinMode (vu1, OUTPUT);
  pinMode (vu2, OUTPUT);
  pinMode (vu3, OUTPUT);
  
  //The button pin for Input
  pinMode (button, INPUT);
  
  //...and put the moisture indicator on, since the moisture sensor is the default sensor
  digitalWrite (led, HIGH);
}


void loop()
{ 
  //At the beginning of every loop iteration, check the potentiometer (used to change crops)
  //Read the current position of the knob potentiometer
  int potval = analogRead (pot);
  
  //According to the position (between 0 and 1023), compute the crop which it corresponds to
  if (potval <= 70) //Corresponds to Crop 1, i.e. Rice
  {
    if (crop != 1) //If the variables are already set for rice, don't bother changing anything
    {
      //Update the crop index
      crop = 1;
      //RGB Led blinks yellow thrice, as an indication that the crop was changed successfully
      for (int i = 0; i < 3; i++)
      {
        analogWrite (red,80);
        analogWrite (green, 20);
        delay (150);
        analogWrite (red,0);
        analogWrite (green, 0);
        delay (150);
      }
      //Set the sensor ranges/limits for rice
      mmin = 675;
      mmax = 844;
      mideal = 750;
      mwarn = 1000;
      tideal = 28;
      trange = 4;
      lmin = 10;
      lmax = 150;
      lideal = 75;
      lwarn = 5;
    } 
  }
  //Repeat the same process for Crop 2, i.e. Wheat
  else if (potval <= 300)
  {
    if (crop != 2)
    {
      crop = 2;
      for (int i = 0; i < 3; i++)
      {
        analogWrite (red,80);
        analogWrite (green, 20);
        delay (150);
        analogWrite (red,0);
        analogWrite (green, 0);
        delay (150);
      }
      //Ideal values for Wheat:
      mmin = 203;
      mmax = 405;
      mideal = 250;
      mwarn = 506;
      tideal = 12.5;
      trange = 2;
      lmin = 20;
      lmax = 250;
      lideal = 150;
      lwarn = 15;
    }
  }
  //Corresponds to Millets
  else if (potval <= 700)
  {
    if (crop != 3)
    {
      crop = 3;
      for (int i = 0; i < 3; i++)
      {
        analogWrite (red,80);
        analogWrite (green, 20);
        delay (150);
        analogWrite (red,0);
        analogWrite (green, 0);
        delay (150);
      }
      //Ideal values for Millets
      mmin = 162;
      mmax = 480;
      mideal = 325;
      mwarn = 608;
      tideal = 30;
      trange = 3;
      lmin = 10;
      lmax = 200;
      lideal = 120;
      lwarn = 5;
    }
  }
  //Corresponds to Pulses
  else if (potval <= 953)
  {
    if (crop != 4)
    {
      crop = 4;
      for (int i = 0; i < 3; i++)
      {
        analogWrite (red,80);
        analogWrite (green, 20);
        delay (150);
        analogWrite (red,0);
        analogWrite (green, 0);
        delay (150);
      }
      //Ideal values for Pulses
      mmin = 203;
      mmax = 304;
      mideal = 243;
      mwarn = 345;
      tideal = 25;
      trange = 4;
      lmin = 10;
      lmax = 175;
      lideal = 100;
      lwarn = 5;
    }
  }
  //Lastly, the extreme position corresponds to Sugarcane
  else
  {
    if (crop != 5)
    {
      crop = 5;
      for (int i = 0; i < 3; i++)
      {
        analogWrite (red,80);
        analogWrite (green, 20);
        delay (150);
        analogWrite (red,0);
        analogWrite (green, 0);
        delay (150);
      }
      //Ideal values for Sugarcane
      mmin = 138;
      mmax = 276;
      mideal = 185;
      mwarn = 322;
      tideal = 25;
      trange = 3;
      lmin = 10;
      lmax = 225;
      lideal = 140;
      lwarn = 5;
    }
  }
  
  //Now check for changes in the mode, i.e., the sensor that is currently active
  
  //Set previous to mode, and then compute the new mode. 
  previous = mode;
  //Read the state of the button (used to change sensors)
  mval = digitalRead (button);
  
  int longtime = 0;
  //Loop tracks the number of seconds for which the button was long-pressed
  while (mval == HIGH)
  {
    delay(10);
    longtime += 10;
    mval = digitalRead (button);
  }
  
  //If the time exceeds 1000 (milliseconds, = 1 second), then set sendData to true, in order to upload the data to the server via Python
  if (longtime > 1000)
  {
    sendData = true;
  }
  //Otherwise, if the button was pressed, then go to the next mode/sensor
  else if (longtime > 0)
  {
    //If it has been more than 'lastDebounceTime' milliseconds (currently set to 200) since the last button press, then change modes
    //This prevents a button press from being interpreted as many small instantaneous button presses
    if((millis() - lastDebounceTime) > debounceDelay) 
    {
      //Reset all the indicator + RGB LED's, and the VU Meter
      digitalWrite (tled, LOW);
      digitalWrite (lled, LOW);
      digitalWrite (led, LOW);
      digitalWrite (hled, LOW);
      analogWrite (red,0);
      analogWrite (blue, 0);
      analogWrite (green,0);
      analogWrite (vu3,0);
      analogWrite (vu2,0);
      analogWrite (vu1,0);
      
      //Increment mode by 1
      mode++;
      
      //Wrap around mode 5 to mode 1
      if (mode == 5)
      {
        mode = 1;
      }
      
      //According to the current mode, switch on the corresponding LED indicator
      if (mode == 1)
      {
        digitalWrite (led, HIGH);
      }
      else if (mode == 2)
      {
        digitalWrite (tled, HIGH);
      }
      else if (mode == 3)
      {
        digitalWrite (lled, HIGH);
      }
      else if (mode == 4)
      {
        digitalWrite (hled, HIGH);
      }
      
      //Set the current time, for comparison at the next button press
      lastDebounceTime = millis();
    }  
  }
  
  //Read the new value from the currenly active sensor:
  
  //If moisture sensor is active
  if (mode == 1)
  {
    //Read the value from the sensor
    moisture = analogRead(moisturePin);
    
    //Map the value onto a range of 0 to 765, where 765 corresponds to the ideal value
    //This value is used to light up the three LED's in the VU meter, each in the range of 0 to 255
    //255*3 = 765
    int vum = map (moisture, 0, mideal, 0, 765);
    vum = constrain (vum, 0, 765); //Make sure it is within the limits
    
    //If the button had been long-pressed, send the new, sensed data
    if (sendData == true)
    {
      //Express the reading as an integer percentage of the ideal value
      float perc = ((float)moisture*100/mideal);
      int perco = (int)perc;
      //Convert it to a String variable
      String per = String(perco);
      //Append 0's to the front of the number to make sure it is exactly 3 digits
      while (per.length() < 3)
      {
        per = "0"+per;
      }
      //Send the data to the Serial Monitor, which is being monitored by the Python script continuously
      //Format - "<Sensor Index>.<Crop Index>.<Percentage of Ideal Value (3 digits strictly)>:<Actual Value>"
      Serial.print("1.");
      Serial.print(crop);
      Serial.print(".");
      Serial.print(per);
      Serial.print(":");
      Serial.println (moisture);
      
      //Reset the Boolean to false
      sendData = false;
    }
    
    //Call the method, which takes the VU Meter value (0 to 765) as a parameter and displays it on the 3-LED meter 
    displayOnVUMeter(vum);
  
    //Check if current value exceeds the warning value - if so, put on the warning indicator (Red LED at the end of the VU Meter)
    if (moisture > mwarn)
    {
      digitalWrite (warn, HIGH);
    }
    else
    {
      digitalWrite (warn, LOW);
    }
    
    //If the value lies within the ideal range, light the RGB LED in a varying shade of green
    if (moisture > mmin && moisture < mmax)
    {
      //How far is it from the ideal value?
      int abmv = abs (moisture - mideal);
      //Find the difference between the ideal and minimum or maximum value, depending on which side the current value is
      int off;
      if (moisture < mideal)
      {
        off = mideal - mmin;
      }
      else
      {
        off = mmax - mideal;
      }
      //If abmv = off, let abmv = 0 (intensity of green light), and if abmv = 0, let abmv = off
      abmv = off - abmv;
      //Map the obtained abmv onto a range of 0 to 255, where 255 corresponds to off
      abmv = map (abmv, 0, off, 0, 255);
      //Write abmv as the intensity of the green light in the RGB LED
      //Therefore, the closer the value gets to the ideal value, the more green the light becomes
      analogWrite (green, abmv);
    }
    else
    {
      analogWrite (green, 0);
    }
  }
  //If temperature sensor is active
  else if (mode == 2)
  {
    //The DHT11 sensor that is being used can only be refreshed after intervals of 2 seconds
    //Therefore, if the previous mode was also temperature (that is, the temperature was sensed in the last iteration as well), then
    //wait for 2 seconds, and then get the new temperature.
    if (previous == 2)
    {
      unsigned long currentMillis = millis ();
      while (millis() - currentMillis < 2000)
      {
        if (digitalRead(button) == HIGH) //If the button is pressed in between, break the loop and interrupt the pause
        {
          break;
        }
      }
    }
    //Refresh the DHT11 sensor
    int chk = DHT11.read(DHT11PIN);
    //Get the temperature from the object
    temperature = (int)DHT11.temperature;
    
    //If the button had been long pressed to send data
    if (sendData == true)
    {
      //Follow a similar process as before
      float perc = ((temperature*100)/(float)tideal);
      int perco = (int)perc;
      String per = String(perco);
      while (per.length() < 3)
      {
        per = "0"+per;
      }
      //Print the data on the Serial Monitor, and set sendData to false
      Serial.print("2.");
      Serial.print(crop);
      Serial.print(".");
      Serial.print(per);
      Serial.print(":");
      Serial.println(temperature);
      sendData = false;
    }
    
    Serial.println (temperature);
    Serial.println (tideal);
    //If the temperature lies within 'range' of the ideal temperature, light the green LED of the RGB LED
    if (abs (temperature - tideal) < trange)
    {
      analogWrite (green,40);
      analogWrite (red,0);
      analogWrite (blue, 0);
    }
    //Otherwise, light the Red OR Blue LEDs with varying intensity, depending on whether it is warmer than the range, or colder
    //The intensity of the colours increases as the distance from the ideal range increases
    else
    {
      //Set red, green and blue to 0
      analogWrite (red,0);
      analogWrite (green,0);
      analogWrite (blue,0);
      //If it is warmer than the ideal temperature, then:
      if (temperature > tideal)
      {
        //Find the distance from the ideal temperature
        int off = temperature - tideal;
        //Map the distance onto a range of 0 to 255 (= intensity of red LED), where a 20 degrees Celsius difference corresponds to 255 (maximum intensity)
        int rval = map (off, 0, 20, 0, 255);
        rval = constrain (rval, 0, 255);
        //Write the rval to the analog pin for the Red LED
        analogWrite (red, rval);
      }
      else
      {
        //If temperature is lesser than the ideal range, then a mixture of the blue and green LEDs is used
        int off = tideal - temperature;
        int bval = map (off, 0, 20, 0, 255); //A 20 degrees Celsius difference corresponds to the maximum intensity of the blue LED
        bval = constrain (bval, 0, 255);
        analogWrite (blue, bval);
        int gval = map (off, 0, 20, 0, 50); //While the same difference corresponds to an intensity of 50/255 on the green LED
        gval = constrain (gval, 0, 255);
        analogWrite (green, gval);
        //Thus, the blue and green LEDs are mixed in a ratio of 255:50 = 51:10, to produce a bright-ish blue shade on the RGB LED. 
      }
    }
  }
  //If the Light Sensor is currently active
  else if (mode == 3)
  {
    //Read the new value of the sensor
    light = analogRead(lightPin);
    //Map it onto a range of 0 to 765, where 750/1023 corresponds to the maximum value
    int vum = map (light, 0, 750, 0, 765);
    vum = constrain (vum, 0, 765);
    
    //If data is to be sent, follow the same process as is done for moisture/temperature
    if (sendData == true)
    {
      float perc = ((float)(1023-light)*100/(1023-lideal));
      int perco = (int)perc;
      String per = String(perco);
      while (per.length() < 3)
      {
        per = "0"+per;
      }
      Serial.print("3.");
      Serial.print(crop);
      Serial.print(".");
      Serial.print(per);
      Serial.print(":");
      Serial.println (light);
      sendData = false;
    }
    
    //Pass 765-vum, and not vum to the function, since the light sensor's value decreases with increased brightness.
    //If vum was passed, 0 = bright light would've not lit up the VU Meter at all. 765-vum will reverse the scale, and thus make the meter rise with increased brightness
    displayOnVUMeter(765-vum);
    
    //If the light value is lesser (i.e., brighter) than the warning value, put on the warning light
    if (light < lwarn)
    {
      digitalWrite (warn, HIGH);
    }
    else
    {
      digitalWrite (warn, LOW);
    }
    
    //If the value lies within the ideal range, then light up the green LED with varying intensity
    if (light > lmin && light < lmax)
    {
      //Repeat the same algorithm used for moisture
      int abmv = abs (light - lideal);
      int off;
      if (light < lideal)
      {
        off = lideal - lmin;
      }
      else
      {
        off = lmax - lideal;
      }
      abmv = off - abmv;
      abmv = map (abmv, 0, off, 0, 255);
      analogWrite (green, abmv);
    }
    else
    {
      analogWrite (green, 0);
    }
  }
  //If the Humidity Sensor is currently active
  else
  {
    //If the humidity was sensed in the previous iteration as well, wait for 2 seconds before refreshing the sensor (same as temperature)
    if (previous == 4)
    {
      unsigned long currentMillis = millis ();
      while (millis() - currentMillis < 2000)
      {
        if (digitalRead(button) == HIGH)
        {
          break;
        }
      }
    }
    //Refresh DHT11 sensor
    int chk = DHT11.read(DHT11PIN);
    //Obtain value of humidity from the dht11 object
    humidity = (int)DHT11.humidity;
    
    //Send data if the button was long-pressed
    if (sendData == true)
    {
      //Since humidity is anyway a percentage, and there is no 'ideal' value, write it directly to the screen
      String per = String(humidity);
      while (per.length() < 3)
      {
        per = "0"+per;
      }
      Serial.print("4.");
      Serial.print(crop);
      Serial.print(".");
      Serial.print(per);
      Serial.print(":");
      Serial.println(humidity);
      sendData = false;
    }
    //Map the humidity % onto a range of 0 to 765 for the VU Meter, where a 100% humidity corresponds to 765
    int vum = map (humidity, 0, 100, 0, 765);
    vum = constrain (vum, 0, 765);
    
    //Call the VUMeter method to light up the meter
    displayOnVUMeter (vum);
  }
}//End of loop ()

void displayOnVUMeter (int vum)
{
  //'vum' can vary between 0 and 765 - accordingly, the three LED's need to be lit up to appropriate intensities
  //Each LED's maximum intensity is 255
  //Therefore, if vum is less than or equal to 255, only the first LED will be lit up to an intensity of vum
  if (vum <= 255)
  {
    analogWrite (vu1, vum);
    analogWrite (vu2, 0);
    analogWrite (vu3,0);
  }
  else
  {
    //Otherwise, the first LED is lit up to 255, and the rest of vum carries over to LED 2
    analogWrite (vu1, 255);
    vum -= 255; 
    //If, after subtracting 255, vum is less than or equal to 255, LED 2 will be lit up to an intensity of vum, and LED 3 will remain dark
    if (vum <= 255)
    {
      analogWrite (vu2, vum);
      analogWrite (vu3,0);
    }
    //Otherwise, LED 2 is lit up to 255, and the remaining vum is given to LED 3
    else
    {
      vum -= 255;
      analogWrite (vu2, 255);
      analogWrite (vu3, vum);
    }
  }
} //End of displayOnVUMeter(int)
