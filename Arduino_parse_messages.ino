#include <LiquidCrystal_I2C.h>

#define BUZZER    8           // Pin for buzzer
#define LED      13

struct header {
  char command[16]; /* for storing keywords such as Buzzer, LED, Motor, etc.*/
  char status[8];   /* for storing On and Off */
};


LiquidCrystal_I2C lcd(0x27, 16, 2);

struct header hdr;

String inputString = "";      // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete
int buzzer = 0;
int led = 0;
int on_off = 0;



void setup() {
  // initialize serial:
  Serial.begin(9600);

  lcd.init();
  lcd.backlight();
  
  pinMode(BUZZER, OUTPUT);
  pinMode(LED, OUTPUT);
  
  // reserve 200 bytes for the inputString:
  inputString.reserve(200);

  lcd.setCursor(0,0);
  lcd.print("Waiting for msg.");
}

void display_message() {
  
}
void compare_string(char *s1, char *s2, int n) {

  Serial.print("Comparing ");
  Serial.print(s1);
  Serial.print(" and ");
  Serial.println(s2);
  
  if (strncmp(s1, s2, n) == 0) {
    
    Serial.println("The two strings are the same");
    
    if (strncmp(s1, "Buzzer", 6) == 0) {
      lcd.setCursor(0,0);
      lcd.print("Buzzer          ");
      buzzer = 1;
    }
    if (strncmp(s1, "LED", 3) == 0) {
      lcd.setCursor(0,0);
      lcd.print("LED             ");
      led = 1;
    }
    if (strncmp(s1, "Motor", 5) == 0) {
      lcd.setCursor(0,0);
      lcd.print("Motor           ");
      led = 1;
    }
    if (strncmp(s1, "On", 2) == 0) {
      lcd.setCursor(0,1);
      lcd.print("On              ");
      on_off = 1;
    }
    if (strncmp(s1, "Off", 3) == 0) {
      lcd.setCursor(0,1);
      lcd.print("Off             ");
      on_off = 0;
    }
  } else {
    Serial.println("The two strings are not the same");
  }
}

void processMessage(char *message) {
  int i=0;
  int n=0;
  int len = 0;
  char buffer[12];

  len = strlen(message);
  
  for (i=0,n=0; i <= len; i++) {
    buffer[n++] = message[i];
    if ((message[i] == ' ') || (message[i] == '\n')) {
      Serial.println(buffer);
      compare_string(buffer, "On", 2);
      compare_string(buffer, "Off", 3);
      compare_string(buffer, "Buzzer", 6);
      compare_string(buffer, "LED", 3);
      n = 0;
    }
  }
}

void control_devices() {
  
    if ((buzzer == 1) && (on_off == 1)) {
      buzzer = 0;
      digitalWrite(BUZZER, HIGH);
    }
 
    if ((buzzer == 1) && (on_off == 0)) {
      buzzer = 0;
      digitalWrite(BUZZER, LOW);
    }
    
    if ((led == 1) && (on_off == 1)) {
      led = 0;
      digitalWrite(LED, HIGH);
    }
    
    if ((led == 1) && (on_off == 0)) {
      led = 0;
      digitalWrite(LED, LOW);
    }
}

void loop() {
  char buf[24];
  
  delay(500);
  
  // print the string when a newline arrives:
  if (stringComplete) {
    Serial.println(inputString);
    inputString.toCharArray(buf, 24);
    // clear the string:
    inputString = "";
    stringComplete = false;

    processMessage(buf);

    control_devices();
  }
}

void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char) Serial.read();

    Serial.println(inChar);
    
    // add it to the inputString:
    inputString += inChar;
    
    // if incoming character is a newline, 
    // set a flag so loop() function can
    // do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}
