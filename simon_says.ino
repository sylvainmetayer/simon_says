#define LED_NUMBER 3
#define FIRST_LED 1
#define FIRST_BUTTON 11
#define BUZZER 8
#define DEBUG false

enum state {
  SHOW_NOTES, 
  WRONG_ORDER,
  BUTTON_1_PRESSED,
  BUTTON_2_PRESSED,
  BUTTON_3_PRESSED,
  WIN
} state_var = SHOW_NOTES;

void setupElement(int number, int offset, int type) {
  for (int i = 0; i < number; i++) {
    pinMode(i+offset, type);
  }
}

void setup()
{
  if (DEBUG) {
    Serial.begin(9600); 
  }
  // DEBUG dans loop : Serial.println(variable)

  setupElement(LED_NUMBER, FIRST_LED, OUTPUT);
  setupElement(LED_NUMBER, FIRST_BUTTON, INPUT_PULLUP);
}

void loop()
{
  int notes[3] = {1,2,3}; // TODO First dummy order, then need to randomize it.  

  switch (state_var) {
    case SHOW_NOTES:
      // TODO Show notes in order & buzzer
      break;
     case WRONG_ORDER:
        // Afficher toutes les leds à 1 pendant 5s
      break;
     case WIN:
        // Afficher toutes les leds pendant 10s
       break;
     case BUTTON_1_PRESSED:
      // TODO Gérer le bouton 1 dans une fonction générique
      break;
     case BUTTON_2_PRESSED: 
       // TODO Gérer le bouton 2 dans une fonction générique
      break;
     case BUTTON_3_PRESSED:
      // TODO Gérer le bouton 3 dans une fonction générique
      break;
  }

  // TODO Si bouton 1, 2  ou 3 pressé et que state_var != SHOW_NOTE, WIN, WRONG_ORDER, set state_var à BUTTON_N_PRESSED 
}