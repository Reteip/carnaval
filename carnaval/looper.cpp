#include <Arduino.h>
#include "looper.h"
#include "leddriver.h"
#include <JC_Button.h>
#include "rgbtohsv.h"

Button menuBtn(2);
Button programBtn(4);


#define LONG_PRESS 1000
#define ERROR_PRESS 10000
#define MAX_CYCLE 13

enum states_t {MENU, PROGRAM, TO_PROGRAM, IN_PROGRAM_PRESS, ERROR};
static states_t STATE;
static uint8_t CYCLE; 
static uint8_t PROGRAM_INDEX; 
static uint8_t UPDATES_PER_SECOND;

static uint16_t PROGRAM_COUNTER; // 65536

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
    STATE=PROGRAM;
    PROGRAM_INDEX=3;
    CYCLE = 7;
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
        SetStripOff();
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
          Show_Cycle(CYCLE);
        }
        if (menuBtn.wasReleased()) 
        {
          SetStripOff();
          CYCLE+=1;
          if (CYCLE > MAX_CYCLE) CYCLE = 0;
          Serial.println("Menu up " );
          PROGRAM_COUNTER = 0;
          Show_Cycle(CYCLE);
        }
        if (programBtn.wasReleased())    // if the button was released, change the LED state
        {
          SetStripOff();
          if (CYCLE == 0) { CYCLE = MAX_CYCLE; } else
          {
            CYCLE-=1;
          }
          Serial.println("Menu down ");
          PROGRAM_COUNTER = 0;
          Show_Cycle(CYCLE);
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
      
      break;
     
      case PROGRAM:
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
	 switch (CYCLE){
	  case 0:
		  BiertjeProgram(PROGRAM_COUNTER);
		break;
		//DiscoNormal
		case 1:		
		// Serial.println("Case 1, solid white");

		// fill_solid( currentPalette, 16, CRGB::Yellow);
		// currentBlending = NOBLEND;
		// FillLEDsFromPaletteColors( 0);
		 break;
		case 2:
		//Defibrillator
		break;
		case 3:
		//Heartbeat
		// currentPalette = RainbowColors_p;
		// currentBlending = LINEARBLEND;
    
  	// 	FillLEDsFromPaletteColors( programCounter);

		break;
		case 4:
		//DiscoHooker
		
		break;
		case 5:
		//DiscoBlinkOnProgramButton
		break;

		case 6 ... 13:
      Serial.println(CYCLE);
      
	  	SolidColorProgram(MenuColors[CYCLE - 6], PROGRAM_INDEX, PROGRAM_COUNTER);
		break;

		default:
		break;
	 }
}

void ExecuteInProgramPress()
{
	 switch (CYCLE){
	  case 0:
		  SetStripOff();
      PROGRAM_COUNTER = 0;
		break;
    case 1 ... 5:
		break;
		case 6 ... 13:
      PROGRAM_COUNTER = 0;
      PROGRAM_INDEX+=1;
		break;
	 }
}