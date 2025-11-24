
const int micPin = 34;      

const int rPin = 27;        
const int gPin = 25;         
const int bPin = 26;       


int lastMic = 0;
unsigned long lastClap = 0;


int colorIndex = 0;
int colors[][3] = {
  {0,   255, 255},  
  {255, 0,   255},  
  {255, 255, 0  },  
  {0,   0,   255},  
  {0,   255, 0  },  
  {255, 0,   0  }, 
  {0,   0,   0  }  
};

void setup() {
  Serial.begin(115200);

  // PWM
  ledcAttachPin(rPin, 0); ledcSetup(0, 5000, 8);
  ledcAttachPin(gPin, 1); ledcSetup(1, 5000, 8);
  ledcAttachPin(bPin, 2); ledcSetup(2, 5000, 8);

  setColor(255,255,255); 

  delay(300);
  lastMic = analogRead(micPin); 
}

void setColor(int r, int g, int b) {
  ledcWrite(0, r);
  ledcWrite(1, g);
  ledcWrite(2, b);
}

void loop() {
  int micValue = analogRead(micPin);


  if ((micValue - lastMic) > 400 && millis() - lastClap > 300) {
    lastClap = millis();
    colorIndex = (colorIndex + 1) % 7;

    setColor(
      colors[colorIndex][0],
      colors[colorIndex][1],
      colors[colorIndex][2]
    );

    Serial.println("CLAP !");
  }

  lastMic = micValue;
}
