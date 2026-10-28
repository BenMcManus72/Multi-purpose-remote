#include <Arduino.h> // maybe removed
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <RF24.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1


// setting up the radio pins and the channel it sends to
RF24 radio(9, 10);
const byte address[6] = "24116";

// loads the screen size
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int selection = 0;
int left_click;
int right_click;
int enter_button;
const char* colours[] = {"Off", "Red", "Green", "Blue", "Cyan", "Yellow", "White", "Purple", "rainbow"};
const int total_colours = sizeof(colours) / sizeof(colours[0]);
int input_cooldown = 100;
char send_packet;


// this controls what you are selecting in the menu with ">"
void menu_update(int func_selection, const char** menu_list){
	display.clearDisplay();
	display.setCursor(0, 0);
	for ( int count = 0; count < total_colours; count ++){
	if (func_selection == count){
		display.print("> ");
	}
	display.println(menu_list[count]);
	}
	display.display();
}

void setup(void) {
	pinMode(3, OUTPUT);
	digitalWrite(3, LOW);
	pinMode(4, INPUT_PULLUP);
	pinMode(5, INPUT_PULLUP);
	pinMode(7, INPUT_PULLUP);

	//sets variables for the oled
	display.begin(SSD1306_SWITCHCAPVCC, 0X3c);
	display.clearDisplay();

	display.setTextSize(1);
	display.setTextColor(SSD1306_WHITE);
	display.setCursor(0, 0);
	menu_update(selection, colours);
	//radio setup
	radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
	radio.powerDown();
}

void loop(void) {
	left_click = digitalRead(4);
	right_click = digitalRead(5);
	enter_button = digitalRead(7);

//controls what happens when the buttons are pressed
	if (enter_button == LOW){
			radio.powerUp();
			delay(20);
			send_packet = '1' + selection;
			radio.write(&send_packet, sizeof(send_packet));
			while (digitalRead(7) == LOW) {}
			delay(50);
	}
	if (left_click == LOW){
		if (selection == 0){
			selection = total_colours -1;
		}
		else{
			selection = selection -1;
		}
		menu_update(selection, colours);
		delay(input_cooldown);
	}
	if (right_click == LOW){
		if (selection == total_colours -1){
			selection = 0;
		}
		else{
			selection = selection + 1;
		}
		menu_update(selection, colours);
		delay(input_cooldown);
	}
}

