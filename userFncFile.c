#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "inc/userFncFile.h"
#include "inc/systemTimer.h"
#include "inc/led_Led1.h"
#include "inc/led_Led2.h"
#include "inc/timer_api1.h"
#include "inc/conversionFunctions.h"
#include "inc/Graphics_OLED.h"
#include "inc/Edit_Edit1.h"
#include "inc/Keyboard_Pad.h"
#include "inc/Keyboard_Nav.h"
#include "inc/EMICBus.h"

void onReset()
{
    Graphics_OLED_clear();
    Graphics_OLED_printAt(0, 0, 1, "RPM:");
    Graphics_OLED_printAt(0, 22, 1, "TEMP:");
    Graphics_OLED_printAt(0, 44, 1, "PRES:");
}


void eI2C(char* tag, const streamIn_t* const msg)
{
    if (strncmp(tag, "RPMMOTOR", 8) == 0)
    {
        Graphics_OLED_printAt(60, 0, 1, "$r     ", msg);
    }
    else if (strncmp(tag, "TEMP", 4) == 0)
    {
        Graphics_OLED_printAt(60, 22, 1, "$r     ", msg);
    }
    else if (strncmp(tag, "PRES", 4) == 0)
    {
        Graphics_OLED_printAt(60, 44, 1, "$r     ", msg);
    }
    else
    {
        /* default case - no action */
    }
}


void Keyboard_Nav_onPress(uint8_t key)
{
    pI2C("TECLANAV\t%u", key);
    Graphics_OLED_printAt(0, 54, 1, "TECLANAV:%u   ", key);
}


void Keyboard_Pad_onPress(uint8_t key)
{
    pI2C("TECLAPAD\t%u", key);
    Graphics_OLED_printAt(0, 54, 1, "TECLAPAD:%u   ", key);
}



