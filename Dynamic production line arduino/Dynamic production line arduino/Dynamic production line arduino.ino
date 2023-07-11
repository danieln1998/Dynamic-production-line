#define relay D5 
#define ledPin D1
#define lightSensorPin  A0 
const unsigned long oneSecond = 1000; 
unsigned long startTime;
unsigned long elapsedTime;
int sensorValue;
int pwmValue=0;
bool calculate=false;
int levels_time_arr[4];
int averageLightValue;
int lightSum = 0; 
int measurementCount = 0; 

void setup() {

  pinMode(relay, OUTPUT);
  Serial.begin(9600);
  wifi_Setup(); 
  pinMode(ledPin, OUTPUT);
   
}

void loop() {

    GetData(levels_time_arr);
    const unsigned long heating_time = levels_time_arr[0] * oneSecond; 
    const unsigned long light_test_time = levels_time_arr[1] * oneSecond; 
    const unsigned long activation_component_time = levels_time_arr[2] * oneSecond;
    const unsigned long delay_time = levels_time_arr[3] * oneSecond; 
                  
    startTime = millis(); 
    elapsedTime = 0; 
    
    while (elapsedTime < heating_time + light_test_time + activation_component_time + delay_time) {
      elapsedTime = millis() - startTime;
      
      if (elapsedTime < heating_time) {
        Serial.println("heating");
        digitalWrite(relay, LOW);

      } else if (elapsedTime < heating_time + light_test_time) {
        digitalWrite(relay, HIGH);
       performLightTestAction(averageLightValue, lightSum, measurementCount);
       
      } else if (elapsedTime < heating_time + light_test_time + activation_component_time) {
         pwmValue = map(averageLightValue, 0, 1023, 0, 255);
         analogWrite(ledPin, pwmValue);
         Serial.println("activation_component_time");
          
      } else {
        Serial.println("delay");
         analogWrite(ledPin, 0);
         averageLightValue=0;
         lightSum = 0; 
         measurementCount = 0;
      }
    }
  
  }


void performLightTestAction( int& averageLightValue, int& lightSum, int& measurementCount) {
  static unsigned long previousTime = 0;
  unsigned long currentTime = millis();
  
  if (currentTime - previousTime >= oneSecond) {  
      previousTime = currentTime;
      int lightValue = analogRead(lightSensorPin);
      lightSum += lightValue; 
      measurementCount++; 
      calculate=true;
  }
  if (calculate){
averageLightValue = lightSum / measurementCount; 
  Serial.println(averageLightValue);
  calculate=false;
}
}



