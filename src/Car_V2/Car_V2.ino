#include <nRF24L01.h>
#include <RF24.h>
#include <RF24_config.h>
#include <SPI.h>

#define trigPin 3
#define echoPin 2
#define ledPin A0

int M1 = 5;   //direction of motor 1
int M2 = 7;   //direction of motor 2
int S1 = 4;   //speed of motor 1
int S2 = 6;   //speed of motor 2
bool isMoving = false;  //set movement to false on start
bool obstacle = false;  //no obstacle when powering up
bool inertiaObsDetect = false;  //no inertia at start
unsigned long previousMillis = 0;   // will store last time car stopped

class RF24Test: public RF24
{
  public: RF24Test(int a, int b): RF24(a, b) {}
};

RF24Test radio(9, 10);

// Radio pipe addresses for the 2 nodes to communicate.
const uint64_t pipes[2] = { 2468, 1357 };

void setup()
{
  Serial.begin(57600);  //init serial bitrate

  radio.begin();  //init radio

  radio.openWritingPipe(pipes[1]);
  radio.openReadingPipe(1, pipes[0]); //open pipes

  radio.startListening(); //start listening for data
  Serial.println("Listening...");

  pinMode(M1, OUTPUT);
  pinMode(M2, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ledPin, OUTPUT);    //set pins to input and output depending on function of sensor/component

  //radio.powerUp();
}

void loop()
{
  // if there is data ready
  if ( radio.available() )
  {

    byte transmission[7];   //set up data array for incoming data
    bool done = false;    //by default data reception is not complete
    while (!done)
    {
      // Fetch the payload, and see if this was the last one.
      done = radio.read( &transmission, 7 );

      // Spew it
      /* Serial.print(transmission[0]);
        Serial.print(" ");
        Serial.print(transmission[1]);
        Serial.print(" ");
        Serial.print(transmission[2]);
        Serial.print(" ");
        Serial.print(transmission[3]);
        Serial.print(" ");
        Serial.print(transmission[4]);
        Serial.print(" ");
        Serial.print(transmission[5]);
        Serial.print(" ");
        Serial.print(transmission[6]);
        Serial.print("   ");*/

      // Delay just a little bit to let the other unit
      // make the transition to receiver
      delay(20);
    }

    int joyX = map(transmission[0], 0, 255, -255, 255);
    int joyY = map(transmission[1], 0, 255, -255, 255);
    int accelX = transmission[2];
    int accelY = transmission[3];
    int accelZ = transmission[4];
    int zBut = transmission[5];
    int cBut = transmission[6];       //assign variables to joy data


    // if (joyY > 10 || joyY < -10) {
    //  isMoving = true;
    // }

    unsigned long currentMillis = millis();   //how much time since code started

    //Serial.println(isMoving);

    digitalWrite(ledPin, HIGH);   //when data is incoming, led will light up

    long duration, distanceCm, distanceInch;    //set up values for distance sensing
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);       //one ping

    duration = pulseIn(echoPin, HIGH);    //get the amount of time needed for ping to return
    distanceCm = (duration / 2) / 29.1;
    distanceInch = distanceCm / 2.54;     //calc distance in 2 units

    if (distanceInch <= 12) {
      obstacle = true;
    } else {
      obstacle = false;
    }
    //make obstacle true or false if there is one or not



    if ((obstacle && isMoving) || (obstacle && inertiaObsDetect)) {
      Serial.println("DANGER");

      reverse();
    } else {
      //Serial.println("CLEAR");

      //left-right turning
      if (joyX >= 10) {

        right();

      } else if (joyX <= -10) {

        left();

      } else {

        digitalWrite(M1, LOW);
        analogWrite(S1, 0);


      }

      //_________________________________________

      //forward-backward movement
      if (joyY < -10) {
        digitalWrite(M2, HIGH);
        analogWrite(S2, 255 - (abs(joyY)));
        isMoving = true;
        Serial.println("REVERSE");
      } else if (joyY > 10) {
        digitalWrite(M2, LOW);
        analogWrite(S2, abs(joyY));
        isMoving = true;
        Serial.println("FORWARD");
      } else {
        digitalWrite(M2, LOW);
        analogWrite(S2, 0);

        if (isMoving == true && !obstacle) {
          isMoving = false;
          inertiaObsDetect = true;
          previousMillis = currentMillis;
          Serial.println(previousMillis);
          Serial.println(currentMillis);
        }

        if (inertiaObsDetect) {
          if (currentMillis - previousMillis >= 5000) {
            inertiaObsDetect = false;
            Serial.println("------------------------");
          }
        }
      }

//code to set up movement using joystick and obstacle detection




      // if (isMoving == true && !obstacle && joyY < 10 && joyY > -10) {
      //   delay(5000);
      //   isMoving = false;
      // }
    }



    // First, stop listening so we can talk
    radio.stopListening();

    // Send the final one back.
    byte response = B0;
    radio.write( &response, sizeof(response) );
    //Serial.println("Sent response.\n\r");

    // Now, resume listening so we catch the next packets.
    radio.startListening();

  } else {
    digitalWrite(ledPin, LOW);
  }


}


void right() {
  digitalWrite(M1, HIGH);
  analogWrite(S1, 0);
  Serial.println("RIGHT");
}

void left() {
  digitalWrite(M1, LOW);
  analogWrite(S1, 255);
  Serial.println("LEFT");
}

void forward() {
  digitalWrite(M2, LOW);
  analogWrite(S2, 255);
  Serial.println("FORWARD");
}

void reverse() {
  digitalWrite(M2, HIGH);
  analogWrite(S2, 0);
  Serial.println("REVERSE");
}

//four funcions for the four directions
