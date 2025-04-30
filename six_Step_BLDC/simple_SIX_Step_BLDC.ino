// Pin definitions for Hall sensors
const int hallA = A0; // Hall sensor A input
const int hallB = A1; // Hall sensor B input
const int hallC = A2; // Hall sensor C input

// Adjustable timing (in milliseconds)
unsigned int on_time = 1;  // Increase this for lower speed
unsigned int off_time = 1; // Increase this for lower speed

void setup() {
  // Initialize Hall sensor pins
  pinMode(hallA, INPUT);
  pinMode(hallB, INPUT);
  pinMode(hallC, INPUT);

  // Initialize MOSFET control pins
  pinMode(3, OUTPUT); // AH
  pinMode(4, OUTPUT); // AL
  // pinMode(5, OUTPUT); // SDA
  pinMode(6, OUTPUT); // BH
  pinMode(7, OUTPUT); // BL
  // pinMode(8, OUTPUT); // SDB
  pinMode(9, OUTPUT); // CH
  pinMode(10, OUTPUT); // CL
  // pinMode(11, OUTPUT); // SDC 

  // // Disable shutdown pins
  // digitalWrite(5, LOW); // Disable shutdown A 
  // digitalWrite(8, LOW); // Disable shutdown B 
  // digitalWrite(11, LOW); // Disable shutdown C
}

void loop() {
  // Read Hall sensor states
  int hallState = (digitalRead(hallA) << 2) | (digitalRead(hallB) << 1) | digitalRead(hallC);

  // Perform commutation based on Hall sensor state
  bldc_move(hallState);

  // Delay for on-time and off-time
  delay(on_time);
  all_off();
  delay(off_time);
}

void all_off() {
  // Turn off all phases
  digitalWrite(3, LOW); // AH
  digitalWrite(4, LOW); // AL
  digitalWrite(6, LOW); // BH
  digitalWrite(7, LOW); // BL
  digitalWrite(9, LOW); // CH
  digitalWrite(10, LOW); // CL
}

void bldc_move(int hallState) {
  // Commutation based on Hall sensor state
  switch (hallState) {
    case 0b101: AH_BL(); break; // Hall state: 101
    case 0b100: AH_CL(); break; // Hall state: 100
    case 0b110: BH_CL(); break; // Hall state: 110
    case 0b010: BH_AL(); break; // Hall state: 010
    case 0b011: CH_AL(); break; // Hall state: 011
    case 0b001: CH_BL(); break; // Hall state: 001
    default: all_off(); break;  // Invalid or undefined state
  }
}

// Commutation phases
void AH_BL() {
  digitalWrite(3, HIGH); // AH
  digitalWrite(4, LOW);  // AL
  digitalWrite(6, LOW);  // BH
  digitalWrite(7, HIGH); // BL
  digitalWrite(9, LOW);  // CH
  digitalWrite(10, LOW); // CL
}

void AH_CL() {
  digitalWrite(3, HIGH); // AH
  digitalWrite(4, LOW);  // AL
  digitalWrite(6, LOW);  // BH
  digitalWrite(7, LOW);  // BL
  digitalWrite(9, LOW);  // CH
  digitalWrite(10, HIGH); // CL
}

void BH_CL() {
  digitalWrite(3, LOW);  // AH
  digitalWrite(4, LOW);  // AL
  digitalWrite(6, HIGH); // BH
  digitalWrite(7, LOW);  // BL
  digitalWrite(9, LOW);  // CH
  digitalWrite(10, HIGH); // CL
}

void BH_AL() {
  digitalWrite(3, LOW);  // AH
  digitalWrite(4, HIGH); // AL
  digitalWrite(6, HIGH); // BH
  digitalWrite(7, LOW);  // BL
  digitalWrite(9, LOW);  // CH
  digitalWrite(10, LOW); // CL
}

void CH_AL() {
  digitalWrite(3, LOW);  // AH
  digitalWrite(4, HIGH); // AL
  digitalWrite(6, LOW);  // BH
  digitalWrite(7, LOW);  // BL
  digitalWrite(9, HIGH); // CH
  digitalWrite(10, LOW); // CL
}

void CH_BL() {
  digitalWrite(3, LOW);  // AH
  digitalWrite(4, LOW);  // AL
  digitalWrite(6, LOW);  // BH
  digitalWrite(7, HIGH); // BL
  digitalWrite(9, HIGH); // CH
  digitalWrite(10, LOW); // CL
}
