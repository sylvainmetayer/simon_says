#define LED_EASY 13
#define LED_MEDIUM 12
#define LED_HARD 11
#define BTN_DIFFICULTY 3
#define BTN_GAME 4
#define BUZZER A1
#define ACTION_NB 4
#define LED_1 9
#define BTN_1 5
#define LED_2 8
#define BTN_2 6
#define LED_3 10
#define BTN_3 2
#define LED_4 7
#define BTN_4 A0
#define LENGTH_EASY 5
#define LENGTH_MEDIUM 10
#define LENGTH_HARD 15
#define LIGHT_SPEED 1000
#define WAIT_TIME 5000

enum GameState {
	configure,
	calculate,
	play
} gameState = configure;

enum DifficultyState {
	easy,
  	medium,
  	hard
} difficultyState = easy;

enum DifficultyButtonState {
	offPressed,
  	onPressed,
  	changingDifficulty
} difficultyButtonState = offPressed;

enum PlayingState {
	playSequence,
	waitInput,
	playerSequence
} playingState;

enum PlayerSequenceState {
	getInput,
	checkInput,
	lostGame,
	wonGame
} playerSequenceState;

int length;
int sequence[LENGTH_HARD]; // On suppose que LENGTH_HARD est la taille maximum de la séquence
int waiting = 0;
int playerInput;
int playerSequenceNumber;

void setup()
{
	Serial.begin(9600);
	Serial.println("");
	pinMode(LED_EASY, OUTPUT);
	pinMode(LED_MEDIUM, OUTPUT);
	pinMode(LED_HARD, OUTPUT);
	pinMode(BTN_DIFFICULTY, INPUT_PULLUP);
	pinMode(BTN_GAME, INPUT_PULLUP);
	pinMode(LED_1, OUTPUT);
	pinMode(LED_2, OUTPUT);
	pinMode(LED_3, OUTPUT);
	pinMode(LED_4, OUTPUT);
	pinMode(BTN_1, INPUT_PULLUP);
	pinMode(BTN_2, INPUT_PULLUP);
	pinMode(BTN_3, INPUT_PULLUP);
	pinMode(BTN_4, INPUT_PULLUP);
}

void loop()
{
	// Serial.print("Jeux : "), Serial.print(gameState), Serial.println();
	
	// On regarde l'état du jeux
	switch (gameState) {
		
		// Si le jeux est en train d'être configuré
		case configure:
			switch (difficultyButtonState) {
				case offPressed:
					// Serial.print("Difficulte : "), Serial.print(difficultyState), Serial.println();
					switch (difficultyState) {
						case easy:
							digitalWrite(LED_EASY, HIGH);
							digitalWrite(LED_MEDIUM, LOW);
							digitalWrite(LED_HARD, LOW);
							break;
						case medium:
							digitalWrite(LED_EASY, HIGH);
							digitalWrite(LED_MEDIUM, HIGH);
							digitalWrite(LED_HARD, LOW);
							break;
						case hard:
							digitalWrite(LED_EASY, HIGH);
							digitalWrite(LED_MEDIUM, HIGH);
							digitalWrite(LED_HARD, HIGH);
							break;
					}
					if (digitalRead(BTN_DIFFICULTY) == LOW) { difficultyButtonState = onPressed; }
					break;
					
				case onPressed:
					if (digitalRead(BTN_DIFFICULTY) == HIGH) { difficultyButtonState = changingDifficulty; }
					break;
					
				case changingDifficulty:
					switch (difficultyState) {
						case easy:
							difficultyState = medium;
							break;
						case medium:
							difficultyState = hard;
							break;
						case hard:
							difficultyState = easy;
							break;
					}
					difficultyButtonState = offPressed;
					break;
			}
			if (digitalRead(BTN_GAME) == LOW) { gameState = calculate; }
			break;
			
		// Si le jeux doit calculer la séquence de jeux
		case calculate:		
			switch (difficultyState) {
				case easy:
					length = LENGTH_EASY;
					break;				
				case medium:
					length = LENGTH_MEDIUM;
					break;					
				case hard:
					length = LENGTH_HARD;
					break;
			}
			
			for (int i = 0; i < length; i++) {
				sequence[i] = (rand()%(ACTION_NB));
			}
			
			if (digitalRead(BTN_GAME) == HIGH) { 
				gameState = play;
				playingState = playSequence;
			}
			
			break;
			
		// Si une partie est en cours
		case play:		
			// Si on appuye sur le bouton de jeux, on arrête la partie
			if (digitalRead(BTN_GAME) == LOW) {
				while (true) {
					if (digitalRead(BTN_GAME) == HIGH) {
						gameState = configure;
						break;
					}
				}
			}
			
			// On regarde l'état de la partie
			// On répète la séquence tant que le joueur n'appuye pas avec un temps d'attente de 5 secondes
			// Lorsque le joueur commence à appuyer sur un bouton il a un temps limité pour terminer la séquence en fonction de la difficulté choisie
						
			Serial.print("Partie : "), Serial.print(playingState), Serial.println();
			switch(playingState) {
				case playSequence:	
				
					int ledNumber;
					
					for (int i = 0; i < length; i++) {
						
						Serial.print("Ligne "), Serial.print(i+1), Serial.print(" : "), Serial.print((sequence[i]+1)), Serial.println();
						
						switch(sequence[i]) {
							case 0:
								ledNumber = LED_1;
								break;
							case 1:
								ledNumber = LED_2;
								break;
							case 2:
								ledNumber = LED_3;
								break;
							case 3:
								ledNumber = LED_4;
								break;
							default:
								Serial.println("Sequence incorrecte !");
								break;
						}

						delay(LIGHT_SPEED);
						digitalWrite(ledNumber, HIGH);
						delay(LIGHT_SPEED);
						digitalWrite(ledNumber, LOW);
					}	
					
					playingState = waitInput;	
					playerSequenceNumber = -1;
					break;
					
				case waitInput:
				
					if((digitalRead(BTN_1) == LOW) || (digitalRead(BTN_2) == LOW) || (digitalRead(BTN_3) == LOW) || (digitalRead(BTN_4) == LOW)) {
						playingState = playerSequence;
						playerSequenceState = getInput;
					}
					else {
						if(waiting == WAIT_TIME/100) {
							waiting = 0;
							playingState = playSequence;
						}
						else {
							delay(100);
							waiting++;
						}
					}
					
					break;
					
				case playerSequence:	
					Serial.print("Partie joueur : "), Serial.print(playerSequenceState), Serial.println();			
					switch(playerSequenceState) {
						case getInput:
						
							if(digitalRead(BTN_1) == LOW) {
								digitalWrite(LED_1, HIGH);
								while(true) {
									if(digitalRead(BTN_1) == HIGH) {
										playerInput = 0;
										playerSequenceState = checkInput;
										playerSequenceNumber++;
										digitalWrite(LED_1, LOW);
										break;
									}
								}
							}
							
							if(digitalRead(BTN_2) == LOW) {
								digitalWrite(LED_2, HIGH);
								while(true) {
									if(digitalRead(BTN_2) == HIGH) {
										playerInput = 1;
										playerSequenceState = checkInput;
										playerSequenceNumber++;
										digitalWrite(LED_2, LOW);
										break;
									}
								}
							}
							
							if(digitalRead(BTN_3) == LOW) {
								digitalWrite(LED_3, HIGH);
								while(true) {
									if(digitalRead(BTN_3) == HIGH) {
										playerInput = 2;
										playerSequenceState = checkInput;
										playerSequenceNumber++;
										digitalWrite(LED_3, LOW);
										break;
									}
								}
							}
							
							if(digitalRead(BTN_4) == LOW) {
								digitalWrite(LED_4, HIGH);
								while(true) {
									if(digitalRead(BTN_4) == HIGH) {
										playerInput = 3;
										playerSequenceState = checkInput;
										playerSequenceNumber++;
										digitalWrite(LED_4, LOW);
										break;
									}
								}
							}
							
							break;
							
						case checkInput:						
						
							if(playerInput != sequence[playerSequenceNumber]) {
								playerSequenceState = lostGame;
							}			
							else {
								if(playerSequenceNumber == (length - 1)) {
									playerSequenceState = wonGame;
								}
								else {
									playerSequenceState = getInput;
								}
							}
							
							break;
							
						case lostGame:						
							digitalWrite(LED_1, HIGH);
							digitalWrite(LED_2, HIGH);
							digitalWrite(LED_3, HIGH);
							digitalWrite(LED_4, HIGH);
							delay(2000);
							digitalWrite(LED_1, LOW);
							digitalWrite(LED_2, LOW);
							digitalWrite(LED_3, LOW);
							digitalWrite(LED_4, LOW);
							gameState = configure;							
							break;
							
						case wonGame:						
							for(int i = 0; i < 5; i++) {								
								digitalWrite(LED_1, HIGH);
								digitalWrite(LED_2, HIGH);
								digitalWrite(LED_3, HIGH);
								digitalWrite(LED_4, HIGH);
								delay(200);
								digitalWrite(LED_1, LOW);
								digitalWrite(LED_2, LOW);
								digitalWrite(LED_3, LOW);
								digitalWrite(LED_4, LOW);		
								delay(200);
							}
							gameState = configure;							
							break;
					}				
					break;
			}
			
			break;
	}
}
