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

/* User Variables */
uint16_t rpmMotor = 0;
uint16_t rpmRef = 600;
uint16_t rpmToma = 0;
uint16_t rpmAib = 0;
uint16_t pa = 0;
uint16_t pres = 0;
uint16_t temp = 0;
uint8_t tdf = 0;
uint8_t pantalla = 0;
uint8_t opcion = 0;
uint8_t estado = 0;

void MenuScr(void)
{
    Graphics_OLED_clear();
    Graphics_OLED_printAt(44, 3, 0, "MENU");
    Graphics_OLED_line(0, 13, 127, 13);
    Graphics_OLED_rect(4, 22, 119, 16);
    Graphics_OLED_printAt(12, 27, 0, "ARRANCAR MOTOR");
    Graphics_OLED_printAt(6, 52, 0, "OK=ENTRAR   X=SALIR");
}


void ConfirmScr(void)
{
    Graphics_OLED_clear();
    Graphics_OLED_printAt(26, 6, 0, "ARRANCAR?");
    Graphics_OLED_rect(8, 24, 48, 24);
    Graphics_OLED_printAt(26, 31, 1, "SI");
    Graphics_OLED_rect(72, 24, 48, 24);
    Graphics_OLED_printAt(89, 31, 1, "NO");
    Graphics_OLED_printAt(22, 54, 0, "FLECHAS + OK");
}


void onReset()
{
    LEDs_Led1_state(1);
    pI2C("OLEDUP\t1");
    pantalla = 0;
    MainScr1();
}


void Keyboard_Nav_onPress(uint8_t key)
{
    pI2C("TECLANAV\t%u", key);
    if (pantalla == 0)
    {
        pantalla = 1;
        MainScr2();
    }
    else
    {
        pantalla = 0;
        MainScr1();
    }
}


void Keyboard_Pad_onPress(uint8_t key)
{
    pI2C("TECLAPAD\t%u", key);
}


void eI2C(char* tag, const streamIn_t* const msg)
{
    if (strncmp(tag, "RPMMOTOR", 8) == 0)
    {
        rpmMotor = streamIn_t_ptr_to_uint16_t((streamIn_t*)msg);
    }
    else if (strncmp(tag, "RPMTOMA", 7) == 0)
    {
        rpmToma = streamIn_t_ptr_to_uint16_t((streamIn_t*)msg);
    }
    else if (strncmp(tag, "RPMAIB", 6) == 0)
    {
        rpmAib = streamIn_t_ptr_to_uint16_t((streamIn_t*)msg);
    }
    else if (strncmp(tag, "RPMREF", 6) == 0)
    {
        rpmRef = streamIn_t_ptr_to_uint16_t((streamIn_t*)msg);
    }
    else if (strncmp(tag, "PAPOS", 5) == 0)
    {
        pa = streamIn_t_ptr_to_uint16_t((streamIn_t*)msg);
    }
    else if (strncmp(tag, "TEMP", 4) == 0)
    {
        temp = streamIn_t_ptr_to_uint16_t((streamIn_t*)msg);
    }
    else if (strncmp(tag, "PRES", 4) == 0)
    {
        pres = streamIn_t_ptr_to_uint16_t((streamIn_t*)msg);
    }
    else if (strncmp(tag, "ESTADO", 6) == 0)
    {
        estado = streamIn_t_ptr_to_uint8_t((streamIn_t*)msg);
        if (pantalla == 1)
        {
            switch ((uint8_t)(estado))
            {
                case 0:
                    Graphics_OLED_printAt(66, 48, 0, "APAGADO   ");
                    break;
                case 1:
                    Graphics_OLED_printAt(66, 48, 0, "ARRANCANDO");
                    break;
                case 2:
                    Graphics_OLED_printAt(66, 48, 0, "EN MARCHA ");
                    break;
                case 3:
                    Graphics_OLED_printAt(66, 48, 0, "PARANDO   ");
                    break;
                case 4:
                    Graphics_OLED_printAt(66, 48, 0, "FALLO     ");
                    break;
            }
        }
    }
    else
    {
        /* default case - no action */
    }
}


void MainScr1(void)
{
    Graphics_OLED_clear();
    Graphics_OLED_line(0, 0, 127, 0);
    Graphics_OLED_line(0, 63, 127, 63);
    Graphics_OLED_line(0, 0, 0, 63);
    Graphics_OLED_line(127, 0, 127, 63);
    Graphics_OLED_line(0, 31, 127, 31);
    Graphics_OLED_line(63, 0, 63, 63);
    Graphics_OLED_printAt(5, 3, 0, "MOTOR");
    Graphics_OLED_bindAt(20, 14, 1, &rpmMotor, 1, "%4u");
    Graphics_OLED_printAt(67, 3, 0, "REF");
    Graphics_OLED_bindAt(84, 14, 1, &rpmRef, 1, "%4u");
    Graphics_OLED_printAt(5, 36, 0, "TOMA");
    Graphics_OLED_bindAt(20, 47, 1, &rpmToma, 1, "%4u");
    Graphics_OLED_printAt(69, 36, 0, "AIB");
    Graphics_OLED_bindAt(84, 47, 1, &rpmAib, 1, "%4u");
}


void MainScr2(void)
{
    Graphics_OLED_clear();
    Graphics_OLED_line(0, 0, 127, 0);
    Graphics_OLED_line(0, 63, 127, 63);
    Graphics_OLED_line(0, 0, 0, 63);
    Graphics_OLED_line(127, 0, 127, 63);
    Graphics_OLED_line(0, 31, 127, 31);
    Graphics_OLED_line(63, 0, 63, 63);
    Graphics_OLED_printAt(5, 3, 0, "MARIPOSA");
    Graphics_OLED_bindAt(24, 14, 1, &pa, 1, "%3u");
    Graphics_OLED_printAt(67, 3, 0, "PRES PSI");
    Graphics_OLED_bindAt(88, 14, 1, &pres, 1, "%3u");
    Graphics_OLED_printAt(5, 36, 0, "TEMP C");
    Graphics_OLED_bindAt(24, 47, 1, &temp, 1, "%3u");
    Graphics_OLED_printAt(69, 36, 0, "ESTADO");
}



