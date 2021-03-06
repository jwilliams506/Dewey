//#include "compass.h"
//#include "math.h"


class Drive {
    const int Left_CW = 5;
    const int Left_CCW = 6;

    const int Right_CCW = 10;
    const int Right_CW = 11;

    const int ledPin = 13;
    byte Right_Comp = 4;
    const byte Left_Comp = 0;
    const byte RightBack_Comp = 0;
    const byte LeftBack_Comp = 15;
    

    byte holdCommand;
    byte slowSpeed;
    long prevCm;
    int driveRight = 0;
    bool autonomous = false;
    


  public:
    Drive();
    void driveAutonomous(long);
    byte isAutonomous();
    void driveUpdate();
    byte driveStatus();
    void runSpeed(byte );
    byte readSpeed();
    void driveSTOP();
    void driveFWD();
    void driveHold();
    void driveSlow();
    void driveResume();
    void checkValue (byte);
    void headingCompensate();
    void driveDisplayHeading();

  private:
    byte driveSpeed = 100 ;
    boolean hold = false;
    boolean slow = false;
    float setHeading = 0;
    float driveHeading = 0;
    float difference = 0;
    
    byte driveDirection = 'S';  // Start in STOP mode
    byte driveCommand = 'S';  // Start drive command STOP
    
    void driveBACK();
    void driveRIGHT();
    void driveLEFT();
};

Drive::Drive () {
  driveSpeed = 128;
  pinMode(Left_CW, OUTPUT);
  pinMode(Left_CCW, OUTPUT);
  pinMode(Right_CCW, OUTPUT);
  pinMode(Right_CW, OUTPUT);
};


void Drive::driveHold () {
  if (hold != true) {
    holdCommand = driveDirection;
    driveDirection = 'S';  // set to stop
    checkValue(driveDirection);
    hold = true;
  }
};

void Drive::driveSlow () {
  if (slow != true) {
    slowSpeed = driveSpeed;
    driveSpeed = 144;  // set to = to "1"
    slow = true;
  }
};

void Drive::driveResume () {
  if (hold == true) { // run this casee only once after a hold
    hold = false;
    slow = false;
    driveDirection = holdCommand;
    checkValue(driveDirection);
  };
};

void Drive::runSpeed (byte amount) {
  driveSpeed = amount;
};

byte Drive::readSpeed () {
  int driveSpeed1 = int(driveSpeed);
  byte outSpeed = (((driveSpeed1 + 1) / 16) + byte('0') - 7);
  return byte(outSpeed);
};


void Drive::driveAutonomous (long cm)
{
  if (autonomous)
  {
    if (cm <= 20)
    {
      if (cm < prevCm)
      {
        driveRight = false;
      }
      if (driveRight)
      {
        driveDirection = 'R';
      }
      else
      {
        driveDirection = 'L';
      }
    }
    else
    {
      driveRight = 0;
      driveDirection = 'F';
    }
    prevCm = cm;
  }
};


// this is the drive action that updates any changed parameeters on the drive
void Drive::driveUpdate () {
  
    if (hold != true) {
  // read compass
  
  if (driveDirection == 'F' ) {
    
    driveFWD();
    
    // driveDisplayHeading();
    // use the above instead of the below.
    
 //   driveHeading = headingAverage();

 //   difference = (( driveHeading - setHeading +180) % (float) 360) -180;
//    difference = headingDiff(setHeading,driveHeading);
//    Serial.print("Update setHeading=");
//    Serial.print(driveHeading);
//    Serial.print(", AVG driveHeading=");
//    Serial.print(driveHeading);
//    Serial.print("  Heading Diff=");
//    Serial.println(difference);
    
  }

  if (driveDirection == 'B' ) {
    driveBACK();
  }

    }// End hold - Don't go forward or back if in hold

  if (driveDirection == 'S' ) {
    driveSTOP();
  }
  
  // check if in autonomous mode

};


void Drive::driveDisplayHeading(){
      
    Serial.print("Status Start Heading=");
    Serial.print(setHeading);   
    Serial.print(", driveHeading=");
    Serial.print(driveHeading);
    Serial.print(",  Heading Diff=");
    Serial.print(difference);
    Serial.print(",  RightComp=");
    Serial.println(Right_Comp);
  };

void Drive::headingCompensate(){
      // use average heading value for difference calculation
   //   driveHeading = headingAverage();
   // use immediate heading value for difference calculation
      driveHeading = compass();

 //      Calculate the heading difference   
     difference = headingDiff(setHeading,driveHeading);

// Compensate for the difference in heading
     if (difference > 3){
     Right_Comp = Right_Comp - 1;
    }
    else if (difference < -3){
    Right_Comp = Right_Comp + 1;
    }
    else
    Right_Comp = 4;
  };

byte Drive::driveStatus () {

 
    headingCompensate();
    // Turn on to display the heading and comp
    driveDisplayHeading();

    driveUpdate();
    
  
  return driveDirection;
};


// this is the command processor that handles changes in commands
void Drive::checkValue (byte value) {


    if (value >= '0' and value <= '9') {
      byte oldSpeed = driveSpeed;
      driveSpeed = ((((value - '0') + 6) * 16) + 15);
      
// Print Change for debugging
    Serial.print("  OldSpeed=");
    Serial.print((char) oldSpeed);
    Serial.print(" NewSpeed=");
    Serial.println((char) driveSpeed);
    Serial.println("Test Send");

      
    }
    else {
      byte oldMove = driveDirection;
      
    switch (value) {
      case 'x':
      case 'X':
        driveDirection = 'S';
        driveSTOP();
        break;
      case 's':
      case 'S':
      case '0':
        driveDirection = 'S';
        if (autonomous)
        {
          autonomous = false;
        }
        driveSTOP();
        break;
      case 'f':
      case 'F':
        driveDirection = 'F';
//        setHeading = headingAverage();
//    Serial.print("F - Set Heading=");
//    Serial.println(setHeading); 

// Reset Right_Comp to default
    Right_Comp = 4;

// Store heading value  before we start moving  - test both

//     // Store average heading so we can compare with while traveling
     //setHeading = headingAverage();
     // Store current heading so we can compare with while traveling
     setHeading = compass();

        driveFWD();
 // Store heading value  before we start moving - Test both
        
//     // Store average heading so we can compare with while traveling
     //setHeading = headingAverage();
     // Store current heading so we can compare with while traveling
   //  setHeading = compass();
     
    Serial.print("F - Set Heading=");
    Serial.println(setHeading); 
        
        break;
      case 'b':
      case 'B':
        driveDirection = 'B';
        driveBACK();
        break;
      case 'r':
      case 'R':
        driveDirection = 'R';
        driveRIGHT();
        break;
      case 'l':
      case 'L':
        driveDirection = 'L';
        driveLEFT();
        break;
      case 'a':
      case 'A':
        autonomous = true;
        driveAutonomous(20);
        break;
      default:
        break;
        
        }  //  End Switch       


// Print Change for debugging
    Serial.print("  OldMove=");
    Serial.print((char) oldMove);
    Serial.print(" NewMove=");
    Serial.println((char) driveDirection);

    
        
    } // end else
    driveUpdate();

    
};


void Drive::driveFWD () {
  analogWrite(Left_CW, 0);
  analogWrite(Left_CCW, driveSpeed - Left_Comp);
  analogWrite(Right_CCW, 0);
  analogWrite(Right_CW, driveSpeed - Right_Comp);
  driveDirection = 'F';
};

void Drive::driveBACK () {
  analogWrite(Left_CW, driveSpeed - LeftBack_Comp);
  analogWrite(Left_CCW, 0);
  analogWrite(Right_CCW, driveSpeed - RightBack_Comp);
  analogWrite(Right_CW, 0);
  driveDirection = 'B';
};

void Drive::driveSTOP () {
  analogWrite(Left_CW, 0);
  analogWrite(Left_CCW, 0);
  analogWrite(Right_CCW, 0);
  analogWrite(Right_CW, 0);
  driveDirection = 'S';
};

void Drive::driveRIGHT () {
  analogWrite(Left_CW, 0);
  analogWrite(Left_CCW, driveSpeed / 2);
  analogWrite(Right_CCW, driveSpeed / 2);
  analogWrite(Right_CW, 0);
  driveDirection = 'R';
};

void Drive::driveLEFT () {
  analogWrite(Left_CW, driveSpeed / 2);
  analogWrite(Left_CCW, 0);
  analogWrite(Right_CCW, 0);
  analogWrite(Right_CW, driveSpeed / 2);
  driveDirection = 'L';
};

byte Drive:: isAutonomous() {
  return autonomous;
};


