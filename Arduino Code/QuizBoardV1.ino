#include <SoftwareSerial.h>
#include "CapacitiveSensor.h"
#include "pitches.h"
#include "WTV20SDBreakout.h"
#include "LedControlMS.h"


#define DEBUG 0
#define NBR_MTX 1
/*Globals*/

/*
Please Set this pins according to your Hardware Setup .	
*/


/*
 *INIT mux Port
 *INIT Sound Module Port
 *INIT Buzzer Port
 *INIT LED Matrix Port
 */
int s0 = 8;
int s1 = 7;
int s2 = 6;
int s3 = 5;
/*Define Signal pin*/
int SIG_pin = 2;  //uses 1MOhm between 4 and 2 being 2 as sensor
CapacitiveSensor   cs_4_2 = CapacitiveSensor(4,SIG_pin);  
int pins[16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

/*Analog Pins are set as digital for LED Matrix**/
LedControl lc=LedControl(A2,A0,A1, NBR_MTX);

int pinSpeaker = 3; //speaker pin
int resetPin = 12;  // The pin number of the reset pin.
int clockPin = 9;  // The pin number of the clock pin.
int dataPin = 10;  // The pin number of the data pin.
int busyPin = 11;  // The pin number of the busy pin.


/*decleare basic init function for sound module*/
WTV20SDBreakout wtv020sd16p(resetPin,clockPin,dataPin,busyPin);

static int GAMESTATE=0;

/*tones notes*/

int melody[] = {
  NOTE_C4, NOTE_G3,NOTE_G3, NOTE_A3, NOTE_G3,0, NOTE_B3, NOTE_C4};
int melodyF[] = {NOTE_C4, NOTE_C4,NOTE_G3, NOTE_E3, NOTE_A3,NOTE_C4, NOTE_C4};
int noteDurations[] = {  4, 8, 8, 4,4,4,4,4 };
int noteDurationsF[] = {2, 5, 4, 3,2,1,1 };

/*question answer tuple*/
int qa[8][2]={{8,7},{9,6},
             {10,5},{11,4},
             {12,3},{13,2},
             {14,1},{15,0}};

/**Serial event data*/
String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete

void setup() {
	cs_4_2.set_CS_AutocaL_Millis(0xFFFFFFFF);     // turn off autocalibrate on channel 1 - just as an example
	pinMode(0, INPUT);
  	pinMode(1, OUTPUT);

	Serial1.begin(9600);
	inputString.reserve(100);
	
        if(DEBUG == 0){
            Serial.begin(9600);
        }




	/**MUX pins*/
	pinMode(s0, OUTPUT); 
	pinMode(s1, OUTPUT); 
	pinMode(s2, OUTPUT); 
	pinMode(s3, OUTPUT); 
	digitalWrite(s0, LOW);
	digitalWrite(s1, LOW);
	digitalWrite(s2, LOW);
	digitalWrite(s3, LOW);
	pinMode(SIG_pin,INPUT);

	for (int i=0; i< NBR_MTX; i++){
    	lc.shutdown(i,false);
	  /* Set the brightness to a medium values */
	    lc.setIntensity(i,10);
	  /* and clear the display */
	    lc.clearDisplay(i);
	  }
	displayFull();
	wtv020sd16p.volume(7);
  	wtv020sd16p.reset();
    Serial1.println("Game On");

}

void loop() {

	if (Serial1.available()) 
          serialEvent1();

	if (stringComplete) {
	    Serial1.println(inputString);
	    parseJson(inputString);
	    inputString = "";
	    stringComplete = false;
  	}
	
  	if(DEBUG == 1){
    	Serial1.println("Pin Check: ");  
  	}    
  	int i =0;
	for(i=8;i<16;i++){
		pins[i]=buttonTouched(i);
		if(DEBUG == 1){
			Serial.print(pins[i]);
		}
		if(pins[i]==1){
			countDownBegins(i);
		}	
	    delay(10);
    
      }
    if(DEBUG == 1){
		Serial.println();  
	}  
    delay(500);  
}

/**Serial event read**/
void serialEvent1() {
  while (Serial1.available()) {
    // get the new byte:
    char inChar = (char)Serial1.read(); 
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    } 
  }
}



int readMux(int channel){
  int controlPin[] = {s0, s1, s2, s3};

  int muxChannel[16][4]={
    {0,0,0,0}, //channel 0
    {1,0,0,0}, //channel 1
    {0,1,0,0}, //channel 2
    {1,1,0,0}, //channel 3
    {0,0,1,0}, //channel 4
    {1,0,1,0}, //channel 5
    {0,1,1,0}, //channel 6
    {1,1,1,0}, //channel 7
    {0,0,0,1}, //channel 8
    {1,0,0,1}, //channel 9
    {0,1,0,1}, //channel 10
    {1,1,0,1}, //channel 11
    {0,0,1,1}, //channel 12
    {1,0,1,1}, //channel 13
    {0,1,1,1}, //channel 14
    {1,1,1,1}  //channel 15
  };

  //loop through the 4 sig
  for(int i = 0; i < 4; i ++){
    digitalWrite(controlPin[i], muxChannel[channel][i]);
  }
}
int buttonTouched(int i){
	readMux(i);
	long start = millis();
	long total1 =  cs_4_2.capacitiveSensor(25);
	
	if(total1 > 600){
		/**play touch tone*/
		blickTouched();
		clickSound();
		
		return 1;
	}
	return 0;
}

int countDownBegins(int i){
	int j=0;
	int an=2;
	while(j<10){
		lc.clearAll();
		
		blinkCountdown(j);
		playTone(1000, 300);
        if(GAMESTATE==5){
			GAMESTATE=0;
			return GAMESTATE;
			break;
		}
		an=checkRightClick(i,j);
        delay(100);
        if(GAMESTATE==5){
            return GAMESTATE;
			GAMESTATE=0;
			break;
		}
                an=checkRightClick(i,j);
		if(DEBUG==1){
			Serial1.println(an);
		}
		if(an == 1 || an == 0){
			return an;
		}
		
		j++;
	}
	
	
	if( j == 10 ){
                
        sendMessage(5,i,0,0);
		wrong();
		playFailMusic();
		playFailTone();

		if(DEBUG == 1){
			Serial1.println("GameOver");
		}
		GAMESTATE=0;
		resetMusic();
		lc.clearAll();
	}
	return GAMESTATE;

}
/*************chekc right click**********************/
int  checkRightClick(int i,int counter){

  int k=0;
    for(k=0;k<8;k++){
		pins[k]=buttonTouched(k);
		
		if(pins[k]==1){
			return checkIfAnswerIsCorrect(i,k,counter);
		}
	}
}

int checkIfAnswerIsCorrect(int i,int k,int counter){

	GAMESTATE=5;
	int index=0;
	for(index=0;index<sizeof(qa);index++){
		if(qa[index][0]==i){
			if(qa[index][1]==k){
                sendMessage(1,i,k,counter);
				roundSmiley();
				playWinMusic();
				playWinTone();
				lc.clearAll();
                             
				return 1;
			}
			else{  
                sendMessage(0,i,k,counter);
				roundSad();
				playFailMusic();
				playFailTone();
				lc.clearAll();
                                
				return 0;
			}
		}
	}
}




/*********************TONE/MUSIC************************/
/****** Sound *******/
void clickSound() {
	playTone(750, 500);
}


void playTone(long duration, int freq) {
    duration *= 90;
    int period = (1.0 / freq) * 1000000;
    long elapsed_time = 0;
    while (elapsed_time < duration) {
        digitalWrite(pinSpeaker,HIGH);
        delayMicroseconds(period / 2);
        digitalWrite(pinSpeaker, LOW);
        delayMicroseconds(period / 2);
        elapsed_time += (period);
    }
}

void playWinTone(){
	
	for (int thisNote = 0; thisNote < 8; thisNote++) {
	    int noteDuration = 1000/noteDurations[thisNote];
	    tone(pinSpeaker, melody[thisNote],noteDuration);
	    int pauseBetweenNotes = noteDuration * 1.30;
	    delay(pauseBetweenNotes);
	    noTone(pinSpeaker);
  	}
}

void playFailTone(){
	
	for (int thisNote = 0; thisNote < 8; thisNote++) {
	    int noteDuration = 1000/noteDurationsF[thisNote];
	    tone(pinSpeaker, melodyF[thisNote],noteDuration);
	    int pauseBetweenNotes = noteDuration * 1.30;
	    delay(pauseBetweenNotes);
	    noTone(pinSpeaker);
  	}
}

/****MUSIC System ****/
int generateRandom(){
	return random(0,4);
}

void playWinMusic(){
	/**winning sounds*/
	int getRandomPos=generateRandom();
	//mySerial.println(getRandomPos);
	int playfile=(getRandomPos%2==0)?getRandomPos:0;
	//mySerial.println(playfile);
	wtv020sd16p.asyncPlayVoice(playfile);

}
void playFailMusic(){
	int getRandomPos=generateRandom();
	int playfile=(getRandomPos%2 != 0)?getRandomPos:1;
	
	wtv020sd16p.asyncPlayVoice(playfile);
}
void stopMusic(){
	/**play next**/
	wtv020sd16p.stopVoice();
}
void resetMusic(){
	wtv020sd16p.reset();
}

/**********************LED MATRIX***************************/
/********Touch blink*********/
void blickTouched(){

    lc.clearDisplay(0);
	lc.setColumn(0,7,0x00);
    lc.setColumn(0,6,B00000000);
    lc.setColumn(0,5,0x00);
    lc.setColumn(0,4,0x18);
    lc.setColumn(0,3,0x18);
    lc.setColumn(0,2,0x00);
    lc.setColumn(0,1,B00000000);
    lc.setColumn(0,0,0x00);
    delay(100);
    lc.clearDisplay(0);
}

void displayFull(){
	lc.clearDisplay(0);
	lc.setColumn(0,7,0xFF);
    lc.setColumn(0,6,0xFF);
    lc.setColumn(0,5,0xFF);
    lc.setColumn(0,4,0xFF);
    lc.setColumn(0,3,0xFF);
    lc.setColumn(0,2,0xFF);
    lc.setColumn(0,1,0xFF);
    lc.setColumn(0,0,0xFF);
    delay(100);
    lc.clearDisplay(0);
}

void blinkCountdown(int i){
	
	char buffer [10] ;
	itoa(i,buffer,10);
	lc.writeString(0,buffer);

}

void smileHappy(){
  lc.setColumn(0,6,0x24);
  lc.setColumn(0,5,0x24);
  lc.setColumn(0,3,0x81);
  lc.setColumn(0,2,0x42);
  lc.setColumn(0,1,0x3C);
}

void smileSad(){
  lc.setColumn(0,6,0x24);
  lc.setColumn(0,5,0x24);
  lc.setColumn(0,3,0x3C);
  lc.setColumn(0,2,0x42);
  lc.setColumn(0,1,0x81);
}

void wrong(){
  lc.setColumn(0,7,0x81);
  lc.setColumn(0,6,0x42);
  lc.setColumn(0,5,0x24);
  lc.setColumn(0,4,0x18);
  lc.setColumn(0,3,0x18);
  lc.setColumn(0,2,0x24);
  lc.setColumn(0,1,0x42);
  lc.setColumn(0,0,0x81);
}

void tick(){
  lc.setColumn(0,6,0x1);
  lc.setColumn(0,5,0x2);
  lc.setColumn(0,4,0x4);
  lc.setColumn(0,3,0x88);
  lc.setColumn(0,2,0x50);
  lc.setColumn(0,1,0x20);
}

void roundSmiley(){
  lc.setColumn(0,7,0x3C);
  lc.setColumn(0,6,0x42);
  lc.setColumn(0,5,0xA5);
  lc.setColumn(0,4,0x81);
  lc.setColumn(0,3,0xA5);
  lc.setColumn(0,2,0x99);
  lc.setColumn(0,1,0x42);
  lc.setColumn(0,0,0x3C);
}

void roundSad(){
  lc.setColumn(0,7,0x3C);
  lc.setColumn(0,6,0x42);
  lc.setColumn(0,5,0xA5);
  lc.setColumn(0,4,0x81);
  lc.setColumn(0,2,0xA5);
  lc.setColumn(0,3,0x99);
  lc.setColumn(0,1,0x42);
  lc.setColumn(0,0,0x3C);

}


/************jsonMessage**************/
void sendMessage(int c, int i , int j, int k){

	//String str= "{\"result\":"+String(c)+",\"source\":"+String(i)+",\"destination\":"+String(j)+",\"duration\":"+String(k)+"}";
	String str= ""+String(c)+"-"+String(i)+"-"+String(j)+"-"+String(k)+"";

	
    if(DEBUG==1){
    	//Serial.println(str);
    	Serial.flush();
    }
	Serial1.println(str);
	//Serial.print(str);
	Serial1.flush();
	//str = String();
	delay(200);
	
}


void parseJson(String string){
	
}
