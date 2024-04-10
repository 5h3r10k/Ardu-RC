# Ardu-RC

An Arduino-based Remote Control car project.

Car: [Schematic](schematics/Reciever.jpg) and [Source Code](src/Car_V2/Car_V2.ino)

Transmitter: [Schematic](schematics/Transmitter.jpg) and [Source Code](src/RF24_TX/RF24_TX.ino)

[Gallery](pix/)

Schematics prepared with [Fritzing](https://fritzing.org/)


## Objectives

- Become more familiar with the Arduino environment and Syntax.
- Learn how to use:
  - Ultrasonic Distance Sensor
  - nRF24L01+ Radio Communication module
  - L298N Motor Driver
  - Wii Nunchuk + Adapter
  - Arduino UNO® board
  - Arduino MEGA® board
- Use all of the above collectively to build and program a remote-controlled “smart” Arduino-powered car, that should be able to stop when an obstacle is detected.


## Components

- Transmitter (Remote)
  - Arduino MEGA® Board
  - nRF24L01+ Radio Module
  - Wiichuk adapter
  - Wii Nunchuk

- Receiver (Car)
  - Arduino UNO® Board
  - nRF24L01+ Radio Module
  - L298N Motor Driver
  - Ultrasonic Distance Sensor (HC-SR04)
  - Old RC Car Chassis
    - Included one DC motor for steering and one DC motor for rear-wheel drive
    - Also included a 4xAA battery holder for power supply

- Jumper wires (m->m, m->f)


## Procedure

1. Preparation
   - Disconnected existing PCB from the old RC car
   - Cleaned Chassis
   - Kept any necessary old power cables and motor wires
2. Initial Testing
   - Distance sensor with Arduino
   - Radio Modules
   - Motor Driver with car motors
   - Nunchuk with wiichuk adapter
   - Sending nunchuk readings through radio
   - Receiving nunchuk readings and moving motors
3. Prototyping
   - Affixed Arduino board and motor driver to chassis
   - Attached 9V power supply to chassis Arduino
   - Programming
     - Forward/Reverse functions
     - Steering functions
     - Distance sensor active obstacle detection function
     - Speed mapping functions
     - Obstacle detection during inertial movement
4. Final Assembly
   - Secured all components to chassis
   - Connected all components with jumper wires
   - Enforced consistent grounding
   - Tested final product


## Challenges
- Accidentally fried one of the nRF24L01+ modules due to the ground lead touching the 5V lead on the Arduino barrel socket.
- Lack of intial debugging which lead to a full code rewrite.