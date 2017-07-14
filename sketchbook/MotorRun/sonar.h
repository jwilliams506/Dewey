const int trigPin = 2;    //Trig - green Jumper
const int echoPin = 4;    //Echo - yellow Jumper
long duration, cm, inches;

  long convertCM(long);
  long convertIN(long);
  void printDistance(long);
  void setupSonar();
//  void getSensor();

void setupSonar()
{
  //Define sensor inputs and outputs
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  }
  
  long convertCM(long duration)
  {
      cm = (duration / 2) / 29.1;
  return cm;
}

 long convertIN(long duration)
  {
  inches = (duration / 2) / 74;
  return inches;
}

long getSensor() {
  // The sensor is triggered by a HIGH pulse of 10 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Read the signal from the sensor: a HIGH pulse whose
  // duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);
  
    if (duration == 0) {
    Serial.print("Duration=");
    Serial.println(duration);
    printDistance(duration);
    Serial.print(" Dewey Stopped because of Sensor 0 ");
  }
  
  return duration;
}

void printDistance(long duration) {
  cm = convertCM(duration);
  inches = convertIN(duration);
  Serial.print(inches);
  Serial.print("in, ");
  Serial.print(cm);
  Serial.print("cm  ");
}
