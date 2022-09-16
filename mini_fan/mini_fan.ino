// State definition
#define S0  0
#define S1  1
#define S2  2

// Address number of output in array
#define PWM 0
#define LED 1

const int ledPin = 13;
const int pwmPin = 11;
const int btnPin = 3;

const int trigPin = 10;   // Trigger pin : PWM out
const int echoPin = 7;    // Echo pin : Interrupt in


unsigned long duration;
float distance;

unsigned char state = S0;
unsigned char nextstate = S0;
unsigned char button = 0;
unsigned char face = 0;
unsigned char newinput = 0;

unsigned char ledOut = LOW;
unsigned char pwmOut = 0;

// State table definition
typedef struct {
  uint32_t out[2][2][2];     // output = FSM[state].out[button][PWM or LED]
  uint32_t next[2][2];       // nextstate = FSM[state].next[button]
} State_t;

State_t FSM[3] = 
{
  { {{{0  , LOW  },  {0  ,   LOW  }}, {{0, HIGH}, {50  , HIGH }}},     {{S0, S0}, {S1, S1}} },
  { {{{0  , HIGH },  {50  ,  HIGH }}, {{0, HIGH}, {100 , HIGH }}},     {{S1, S1}, {S2, S2}} },
  { {{{0  , HIGH },  {100  , HIGH }}, {{0, LOW }, {0  ,  LOW  }}},     {{S2, S2}, {S0, S0}} },
};

void setup() {
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);

  // Initialize pwm pin as an output:
  pinMode(pwmPin, OUTPUT);

 pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
  Serial.begin(9600);
  
  // initialize the pushbutton pin as an interrupt input:
  pinMode(btnPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(btnPin), pressed, RISING);
}

void loop() {
  // Calculate next state. then update State
  nextState();



  //깜빡깜빡
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  delayMicroseconds(10);

  duration = pulseIn(echoPin, HIGH);
  distance = (float)duration / 58.0;
  


 if (distance > 5)
  face = 0;
  else
  face = 1;


  
  Serial.printf("Distance : ");
  Serial.print(distance);
  Serial.print(" [cm] ");
      Serial.print("face: ");
    Serial.print(face);
          Serial.print(" button: ");
    Serial.println(button);

  // Output

  if ((millis()/1000)%2)
  digitalWrite(ledPin, LOW);
  else
    digitalWrite(ledPin, ledOut);

    analogWrite(pwmPin, pwmOut);
  
  delay(500);
}

void pressed(){
  button = 1;
}


void nextState(){

  pwmOut = FSM[state].out[button][face][PWM];
  ledOut = FSM[state].out[button][face][LED];
  
  nextstate = FSM[state].next[button][face];
  
  state = nextstate;
  newinput = 0;
    button = 0;

}
