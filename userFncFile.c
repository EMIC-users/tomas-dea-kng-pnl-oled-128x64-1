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
    OLED_clear();
    OLED_printAt(0, 0, 1, "RPM:");
    OLED_printAt(0, 22, 1, "TEMP:");
    OLED_printAt(0, 44, 1, "PRES:");
}


void eI2C(char* tag, const streamIn_t* const msg)
{
    if (strncmp(tag, "RPMMOTOR", 8) == 0)
    {
        OLED_printAt(56, 0, 1, "$r", msg);
    }
    else if (strncmp(tag, "TEMP", 4) == 0)
    {
        OLED_printAt(56, 22, 1, "$r", msg);
    }
    else if (strncmp(tag, "PRES", 4) == 0)
    {
        OLED_printAt(56, 44, 1, "$r", msg);
    }
    else
    {
        /* default case - no action */
    }
}



