#include <nRF24L01.h>
#include <RF24.h>
#include <RF24_config.h>
#include <Nunchuk.h>
#include <Wire.h>
#include <SPI.h>


#define BAUDRATE 57600



class RF24Test: public RF24
{
  public: RF24Test(int a, int b): RF24(a, b) {}
};

RF24Test radio(9, 10);

// Radio pipe addresses for the 2 nodes to communicate.
const uint64_t pipes[2] = { 2468, 1357 };

void setup()
{
  Serial.begin(BAUDRATE);

  Wire.begin();

  // Change TWI speed for nuchuk, which uses Fast-TWI (400kHz)
  Wire.setClock(400000);

  // nunchuk_init_power(); // A1 and A2 is power supply
  nunchuk_init();

   // Redirect stdout
  
  
  radio.begin();
  //radio.powerUp();
  radio.openWritingPipe(pipes[0]);
  radio.openReadingPipe(1, pipes[1]);
  radio.printDetails();
  

  
  
  
  
}

void loop()
{
  
  if (nunchuk_read()) {
    // Work with nunchuk_data

    //nunchuk_print();

    byte analogX = nunchuk_joystickX_raw();
    byte analogY = nunchuk_joystickY_raw();
    byte accelX = nunchuk_accelX_raw();
    byte accelY = nunchuk_accelY_raw();
    byte accelZ = nunchuk_accelZ_raw();
    byte zButton = nunchuk_buttonZ();
    byte cButton = nunchuk_buttonC();
    byte car_data[7];

    car_data[0] = analogX;
    car_data[1] = analogY;
    car_data[2] = accelX;
    car_data[3] = accelY;
    car_data[4] = accelZ;
    car_data[5] = zButton;
    car_data[6] = cButton;

    Serial.print(car_data[0]);
    Serial.print(' ');
    Serial.print(car_data[1]);
    Serial.print(' ');
    Serial.print(car_data[2]);
    Serial.print(' ');
    Serial.print(car_data[3]);
    Serial.print(' ');
    Serial.print(car_data[4]);
    Serial.print(' ');
    Serial.print(car_data[5]);
    Serial.print(' ');
    Serial.println(car_data[6]);


    radio.stopListening();
    radio.write( &car_data, 7 );
    
    // Now, continue listening
    radio.startListening();

    // Wait here until we get a response, or timeout (250ms)
    unsigned long started_waiting_at = millis();
    bool timeout = false;
    while ( ! radio.available() && ! timeout )
      if (millis() - started_waiting_at > 200 )
        timeout = true;

    // Describe the results
    if ( timeout )
    {
      Serial.println("Failed, response timed out.");

    }
    else
    {
      // Grab the response, compare, and send to debugging spew
      byte response;
      radio.read( &response, sizeof(response) );
      //Serial.println(response,BIN);

      if (response == B0)
      {

        Serial.println("Ok");
      }
      else
      {

        Serial.println("No connection");
      }
    }




  }
  // Try again  later
  delay(50);

}
