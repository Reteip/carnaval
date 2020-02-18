#include <Arduino.h>
#include "looper.h"
#include "leddriver.h"
#include <JC_Button.h>

Button menuBtn(2);
Button gameBtn(4);

#define UPDATES_PER_SECOND 100
#define LONG_PRESS 1000
#define ERROR_PRESS 10000
#define MAX_CYCLE 11

enum states_t {MENU, GAME, TO_GAME, ERROR};

void setup() 
{
    Serial.begin(9600);
    
    InitLedDriver();
    
    menuBtn.begin();              // initialize the button object
    gameBtn.begin();

    delay( 3000 ); // power-up safety delay
    
    Serial.println("SETUP DONE");
    
    SetStripOff();
}

void loop()
{
    static uint8_t CYCLE; 
    static uint8_t LOOPINDEX;

    LOOPINDEX = LOOPINDEX + 1; /* motion speed */

    menuBtn.read(); // read the button
    gameBtn.read();
    
    SwitchState();

    ExecuteState();
   
} // end loop

void ExecuteState() 
{
    static states_t STATE;
    static uint8_t CYCLE; 
    static uint8_t LOOPINDEX;

    switch (STATE)
    {
      case MENU:
        Show_Cycle(CYCLE);
        break;
      case GAME:    
        RainbowProgram(LOOPINDEX);    
        break;
      case TO_GAME:

      case ERROR:
        RainbowProgram(LOOPINDEX);        
        break;
      default:
        break;
    }
    
    DoDelay(1000 / UPDATES_PER_SECOND);
    Show();
}

void SwitchState() 
{
    static states_t STATE;
    static uint8_t CYCLE; 
    static uint8_t LOOPINDEX;

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
        if (gameBtn.wasReleased())    // if the button was released, change the LED state
        {
          SetStripOff();
          if (CYCLE == 0) { CYCLE = MAX_CYCLE; } else
          {
            CYCLE-=1;
          }
          Serial.println("Menu down ");
        } else if (gameBtn.pressedFor(LONG_PRESS))
        {
          SetStripOff();
          Serial.println("To Game ");
          STATE = TO_GAME;
        }
        if (menuBtn.pressedFor(ERROR_PRESS)) {
          Serial.println("ERROR ");
          STATE = ERROR;
        }
 
      break;
     
      case GAME:
        RainbowProgram(LOOPINDEX);        
        if (menuBtn.wasReleased())    // if the button was released, change the LED state
        {
          SetStripOff();
          Serial.println("To Menu ");
          STATE = MENU;
        }
        Show();
        DoDelay(1000 / UPDATES_PER_SECOND);
      break;
      case TO_GAME:
        SetStripOff();
        if (gameBtn.wasReleased())  {
          Serial.println("Enter game");
          STATE = GAME;
        }
        if (menuBtn.wasReleased()) {
          Serial.println("Back to Menu");
          STATE = MENU;
        }
        if (gameBtn.pressedFor(ERROR_PRESS)) {
          Serial.println("ERROR ");
          STATE = ERROR;
        }
      break;
      case ERROR:
         if (gameBtn.wasReleased())  {
          Serial.println("Exit Error Game to Menu");
          STATE = MENU;
         }
        if (menuBtn.wasReleased()) {
          Serial.println("Exit Error Menu to Menu");
          STATE = MENU;
        }
      break;

    }
}