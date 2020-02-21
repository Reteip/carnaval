#include <Arduino.h>
#include "looper.h"
#include "leddriver.h"
#include <JC_Button.h>

Button menuBtn(2);
Button programBtn(4);


#define LONG_PRESS 1000
#define ERROR_PRESS 10000
#define MAX_CYCLE 13

enum states_t {MENU, PROGRAM, TO_PROGRAM, ERROR};
static states_t STATE;
static uint8_t CYCLE; 
static uint8_t LOOPINDEX; // 255
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
    CYCLE = 0;
}

void loop()
{

    LOOPINDEX = LOOPINDEX + 1; /* motion speed */
    
    PROGRAM_COUNTER = PROGRAM_COUNTER + 1;
    if (PROGRAM_COUNTER == 0) Serial.println("Jep");
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
        Show_Cycle(CYCLE);
        break;
      case PROGRAM:    
        ExecuteProgram(CYCLE, PROGRAM_COUNTER);   
        break;
      case TO_PROGRAM:
        SetStripOff();
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
        
        if (menuBtn.wasReleased()) 
        {
          SetStripOff();
          CYCLE+=1;
          if (CYCLE > MAX_CYCLE) CYCLE = 0;
          Serial.println("Menu up " );
        }
        if (programBtn.wasReleased())    // if the button was released, change the LED state
        {
          SetStripOff();
          if (CYCLE == 0) { CYCLE = MAX_CYCLE; } else
          {
            CYCLE-=1;
          }
          Serial.println("Menu down ");
        } else if (programBtn.pressedFor(LONG_PRESS))
        {
          Serial.println("To Program ");
          STATE = TO_PROGRAM;
          PROGRAM_COUNTER = 0;
        }
        if (menuBtn.pressedFor(ERROR_PRESS)) {
          Serial.println("ERROR ");
          STATE = ERROR;
        }
 
      break;
     
      case PROGRAM:
        if (programBtn.wasReleased())  {
          PROGRAM_COUNTER = 0;
        }
        if (menuBtn.wasReleased())    // if the button was released, change the LED state
        {
          SetStripOff();
          Serial.println("To Menu ");
          STATE = MENU;
        }
      break;
      case TO_PROGRAM:
        SetStripOff();
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
