#define LED_EASY 0
#define LED_MEDIUM 1
#define LED_HARD 2
#define BTN_DIFFICULTY 3
#define BTN_GAME 4
#define BUZZER 5
#define LED_1 6
#define BTN_1 7
#define LED_2 8
#define BTN_2 9
#define LED_3 10
#define BTN_3 11
#define LED_4 12
#define BTN_4 13
#define LENGTH_EASY 5
#define LENGTH_MEDIUM 10
#define LENGTH_HARD 15

enum GameState {
	configure,
	calculate,
	play
} game = configure;

enum DifficultyState {
	easy,
  	medium,
  	hard
} difficulty = easy;

enum DifficultyButtonState {
	offPressed,
  	onPressed,
  	changingDifficulty
} difficultyButton = changingDifficulty;

int length;

void setup()
{
	pinMode(LED_EASY, OUTPUT);
	pinMode(LED_MEDIUM, OUTPUT);
	pinMode(LED_HARD, OUTPUT);
	pinMode(BTN_DIFFICULTY, INPUT_PULLUP);
	pinMode(BTN_GAME, INPUT_PULLUP);
}

void loop()
{
	switch (game) {
		case configure:
			switch (difficultyButton) {
				case offPressed:
					if (digitalRead(BTN_DIFFICULTY) == LOW) { difficultyButton = onPressed; }
					break;
					
				case onPressed:
					if (digitalRead(BTN_DIFFICULTY) == HIGH) { difficultyButton = changingDifficulty; }
					break;
					
				case changingDifficulty:
					switch (difficulty) {
						case easy:
							digitalWrite(LED_EASY, HIGH);
							digitalWrite(LED_MEDIUM, HIGH);
							digitalWrite(LED_HARD, LOW);
							difficulty = medium;
							break;
						case medium:
							digitalWrite(LED_EASY, HIGH);
							digitalWrite(LED_MEDIUM, HIGH);
							digitalWrite(LED_HARD, HIGH);
							difficulty = hard;
							break;
						case hard:
							digitalWrite(LED_EASY, HIGH);
							digitalWrite(LED_MEDIUM, LOW);
							digitalWrite(LED_HARD, LOW);
							difficulty = easy;
							break;
					}
					difficultyButton = offPressed;
					break;
			}
			if (digitalRead(BTN_GAME) == LOW) { game = calculate; }
			break;
			
		case calculate:		
			switch (difficulty) {
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
			
			int static sequence[length];
			
			for (int i = 0; i < length; i++) {
				sequence[i] = rand()%length;
			}		
			
			if (digitalRead(BTN_GAME) == HIGH) { game = play; }
			break;
			
		case play:
			if (digitalRead(BTN_GAME) == LOW) {
				while (true) {
					if (digitalRead(BTN_GAME) == HIGH) {
						game = configure;
						break;
					}
				}
			}
			break;
	}
}
