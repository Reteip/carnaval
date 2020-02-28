#include <Arduino.h>
#include "looper.h"
#include "leddriver.h"
#include <JC_Button.h>


Button menuBtn(2);
Button programBtn(4);


#define LONG_PRESS 1000
#define ERROR_PRESS 10000
#define MAX_CYCLE 13
#define SLEEP_TIME 3000

enum states_t {MENU, PROGRAM, TO_PROGRAM, IN_PROGRAM_PRESS, SLEEP, ERROR};
static states_t STATE;
static uint8_t MENU_CYCLE; 
static uint8_t PROGRAM_INDEX; 
static uint8_t UPDATES_PER_SECOND;

static uint16_t PROGRAM_COUNTER; 
static uint16_t BEAT; 

void setup() 
{
    Serial.begin(9600);
    UPDATES_PER_SECOND = 100;
    InitLedDriver();
    SetStripOff();
    
    menuBtn.begin();              // initialize the button object
    programBtn.begin();

    delay( 3000 ); // power-up safety delay
    
    Serial.println("SETUP DONE");
    STATE=MENU;
    PROGRAM_INDEX=0;
    MENU_CYCLE = 6;
    BEAT = 45;
}

void loop()
{

    PROGRAM_COUNTER = PROGRAM_COUNTER + 1;
    if (PROGRAM_COUNTER == 0) Serial.println("Program Counter Reset");
    menuBtn.read(); // read the button
    programBtn.read();
    
    SwitchState();

    ExecuteState();
   
} // end loop

void ExecuteState() 
{
    switch (STATE)
    {
      case MENU:
        if (PROGRAM_COUNTER == 10) 
        {
          SetStripOff();
        }
        break;
      case PROGRAM:    
        ExecuteProgram();   
        break;
      case TO_PROGRAM:
        SetStripOff();
        break;
      case IN_PROGRAM_PRESS:
        ExecuteInProgramPress();
        break;
      case ERROR:
        ExecuteProgram();   
        break;
    }
    
    DoDelay(1000 / UPDATES_PER_SECOND);
    Show();
}

void SwitchState() 
{
    switch (STATE)
    {
      case MENU:
        if (programBtn.pressedFor(LONG_PRESS - 100))
        {
          Show_Cycle(MENU_CYCLE);
        }
        if (menuBtn.wasReleased()) 
        {
          SetStripOff();
          MENU_CYCLE+=1;
          if (MENU_CYCLE > MAX_CYCLE) MENU_CYCLE = 0;
          Serial.println("Menu up " );
          PROGRAM_COUNTER = 0;
          Show_Cycle(MENU_CYCLE);
        }
        if (programBtn.wasReleased())    // if the button was released, change the LED state
        {
          SetStripOff();
          if (MENU_CYCLE == 0) { MENU_CYCLE = MAX_CYCLE; } else
          {
            MENU_CYCLE-=1;
          }
          Serial.println("Menu down ");
          PROGRAM_COUNTER = 0;
          Show_Cycle(MENU_CYCLE);
        } else if (programBtn.pressedFor(LONG_PRESS))
        {
          Serial.println("To Program ");
          STATE = TO_PROGRAM;
          PROGRAM_COUNTER = 0;
          PROGRAM_INDEX = 0;
          

        }
        if (menuBtn.pressedFor(ERROR_PRESS)) {
          Serial.println("ERROR ");
          STATE = ERROR;
        }
        ScreenSaver();
      
      break;
     
      case PROGRAM:
        if (programBtn.isPressed()) 
        {
          ExecutePressedDuringProgram();
        }
        if (programBtn.wasReleased())  {
          STATE=IN_PROGRAM_PRESS;
        }
        if (menuBtn.wasReleased())    // if the button was released, change the LED state
        {
          SetStripOff();
          Serial.println("To Menu ");
          STATE = MENU;
        }
      break;
      case TO_PROGRAM:
        PROGRAM_INDEX = 0;

        if (programBtn.wasReleased())  {
          Serial.println("Enter program");
          STATE = PROGRAM;
        }
        if (menuBtn.wasReleased()) {
          Serial.println("Back to Menu");
          STATE = MENU;
        }
        if (programBtn.pressedFor(ERROR_PRESS)) {
          Serial.println("ERROR ");
          STATE = ERROR;
        }
      break;
      case IN_PROGRAM_PRESS:
          STATE=PROGRAM;
      break;
      case ERROR:
         BEAT = random8(10,80);
         EVERY_N_SECONDS(30) {
           MENU_CYCLE = random8(13);
           PROGRAM_INDEX = random8(3);
         }
         if (programBtn.wasReleased())  {
          Serial.println("Exit Error Program to Menu");
          STATE = MENU;
         }
        if (menuBtn.wasReleased()) {
          Serial.println("Exit Error Menu to Menu");
          STATE = MENU;
        }
      break;

    }
}

void ExecuteProgram()
{
	 switch (MENU_CYCLE){
	  case 0:
		  BiertjeProgram(PROGRAM_COUNTER);
		break;
		case 1:		
		//DiscoNormal
      FillAllLedsSolid(CHSV(PROGRAM_INDEX, 255, 255));
      if (PROGRAM_COUNTER % BEAT == 0)
      {
        PROGRAM_INDEX+=64;
      }
		break;
		case 3:
		//Heartbeat
      if (PROGRAM_INDEX <= 240) PROGRAM_INDEX += 15;
      else PROGRAM_INDEX = 255;
              
      FillAllLedsSolid(CRGB(255 - PROGRAM_INDEX, 0, 0));//CHSV(0, 255, max(0, 255 - 10 * PROGRAM_COUNTER)));
      //if (PROGRAM_COUNTER == 26) PROGRAM_COUNTER = 0;
		break;
		case 4:
		//DiscoHooker
      if (PROGRAM_INDEX == 0)
      {
        RainbowProgram(PROGRAM_INDEX);
        PROGRAM_INDEX+=1;
      }
      if (PROGRAM_COUNTER % BEAT == 0)
      {
        ShiftLedsAround(0, NUM_LEDS - 1);
      }
		break;
		case 6 ... 13:
      Serial.println(MENU_CYCLE);
      
	  	SolidColorProgram(MenuColors[MENU_CYCLE - 6], PROGRAM_INDEX, PROGRAM_COUNTER);
		break;

		default:
		break;
	 }
}

void ExecuteInProgramPress()
{
	 switch (MENU_CYCLE){
	  case 0:
		  SetStripOff();
      PROGRAM_COUNTER = 0;
		break;
    case 1:
    // 200 20 = 100ms 1000 / 200 = 5 * 20 = 
      if (PROGRAM_COUNTER < 200);
      {
        BEAT = PROGRAM_COUNTER;
      }
      PROGRAM_COUNTER = 0;
      PROGRAM_INDEX += 64;
		break;
    case 2:
      SetStripOff();
    break;
    case 3:
      SetStripOff();
      PROGRAM_COUNTER = 0;
    case 4:
      if (PROGRAM_COUNTER < 100);
      {
        BEAT = PROGRAM_COUNTER;
      }
      PROGRAM_COUNTER = 0;
    break;
    case 5:
      SetStripOff();
      PROGRAM_INDEX += 64;
    break;
		case 6 ... 13:
      PROGRAM_COUNTER = 0;
      PROGRAM_INDEX+=1;
		break;
	 }
}

void ExecutePressedDuringProgram()
{
  switch (MENU_CYCLE){
    case 2:
      //Defibrillator
      
      EVERY_N_MILLISECONDS(30) {  
        if (PROGRAM_INDEX == 0)
        {
          FillAllLedsSolid(CRGB::Black);
          PROGRAM_INDEX = 1;
        } else
        {
          FillAllLedsSolid(CRGB::White);
          PROGRAM_INDEX = 0;
        }
		  }
    break;
    case 3:
      if (PROGRAM_INDEX < 240) PROGRAM_INDEX = 0;
      else PROGRAM_INDEX = 160;
    break;
    case 5:
      FillAllLedsSolid(CHSV(PROGRAM_INDEX, 255, 255));
    break;
    default:
    break;
  }
	 
}