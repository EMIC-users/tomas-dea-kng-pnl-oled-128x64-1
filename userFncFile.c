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
uint8_t opcMenu = 0;
uint8_t lazo = 1;
uint16_t edit = 0;
uint8_t tdfEst = 0;
uint8_t queEdita = 0;

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
    Graphics_OLED_printAt(5, 36, 0, "MARIPOSA");
    Graphics_OLED_bindAt(20, 47, 1, &pa, 1, "%3u");
    Graphics_OLED_printAt(69, 36, 0, "ESTADO");
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
    Graphics_OLED_printAt(5, 3, 0, "TOMA");
    Graphics_OLED_bindAt(20, 14, 1, &rpmToma, 1, "%4u");
    Graphics_OLED_printAt(69, 3, 0, "AIB");
    Graphics_OLED_bindAt(84, 14, 1, &rpmAib, 1, "%4u");
    Graphics_OLED_printAt(5, 36, 0, "TEMP C");
    Graphics_OLED_bindAt(24, 47, 1, &temp, 1, "%3u");
    Graphics_OLED_printAt(67, 36, 0, "PRES PSI");
    Graphics_OLED_bindAt(88, 47, 1, &pres, 1, "%3u");
}


void ConfirmScr(void)
{
    Graphics_OLED_clear();
    Graphics_OLED_rect(8, 24, 48, 24);
    Graphics_OLED_printAt(26, 31, 1, "NO");
    Graphics_OLED_rect(72, 24, 48, 24);
    Graphics_OLED_printAt(89, 31, 1, "SI");
    Graphics_OLED_printAt(22, 54, 0, "FLECHAS + OK");
}


void MenuScr(void)
{
    Graphics_OLED_clear();
    Graphics_OLED_printAt(10, 2, 0, "ARRANCAR MOTOR");
    Graphics_OLED_printAt(10, 11, 0, "PARAR MOTOR");
    Graphics_OLED_printAt(10, 20, 0, "SALIR DE FALLO");
    Graphics_OLED_printAt(10, 29, 0, "LAZO:");
    Graphics_OLED_printAt(10, 38, 0, "TDF:");
    Graphics_OLED_printAt(10, 47, 0, "MARIPOSA");
    Graphics_OLED_printAt(10, 55, 0, "RPM REF");
}


void EditScr(void)
{
    Graphics_OLED_clear();
    Graphics_OLED_rect(24, 20, 80, 24);
    Graphics_OLED_bindAt(40, 27, 1, &edit, 1, "%4u");
    Graphics_OLED_printAt(4, 52, 0, "X=CANCEL  OK=ACEPTA");
}


void onReset()
{
    LEDs_Led1_state(1);
    setTime1(2000, 'T');
    opcion = 0;
    opcMenu = 0;
    lazo = 1;
    pantalla = 0;
    MainScr1();
}


void etOut1()
{
    pI2C("OLEDUP\t1");
}


void Keyboard_Nav_onPress(uint8_t key)
{
    pI2C("TECLANAV\t%u", key);
    switch ((uint8_t)(pantalla))
    {
        case 0:
            if (key <= 129)
            {
                pantalla = 1;
                MainScr2();
            }
            if (key == 131)
            {
                pantalla = 2;
                MenuScr();
                if (lazo == 1)
                {
                    Graphics_OLED_printAt(46, 29, 0, "AUTO");
                }
                else
                {
                    Graphics_OLED_printAt(46, 29, 0, "MAN ");
                }
                if (tdfEst == 1)
                {
                    Graphics_OLED_printAt(42, 38, 0, "SI");
                }
                else
                {
                    Graphics_OLED_printAt(42, 38, 0, "NO");
                }
                switch ((uint8_t)(opcMenu))
                {
                    case 0:
                        Graphics_OLED_rect(1, 1, 126, 9);
                        break;
                    case 1:
                        Graphics_OLED_rect(1, 10, 126, 9);
                        break;
                    case 2:
                        Graphics_OLED_rect(1, 19, 126, 9);
                        break;
                    case 3:
                        Graphics_OLED_rect(1, 28, 126, 9);
                        break;
                    case 4:
                        Graphics_OLED_rect(1, 37, 126, 9);
                        break;
                    case 5:
                        Graphics_OLED_rect(1, 46, 126, 9);
                        break;
                    case 6:
                        Graphics_OLED_rect(1, 54, 126, 9);
                        break;
                }
            }
            break;
        case 1:
            if (key <= 129)
            {
                pantalla = 0;
                MainScr1();
            }
            if (key == 131)
            {
                pantalla = 2;
                MenuScr();
                if (lazo == 1)
                {
                    Graphics_OLED_printAt(46, 29, 0, "AUTO");
                }
                else
                {
                    Graphics_OLED_printAt(46, 29, 0, "MAN ");
                }
                if (tdfEst == 1)
                {
                    Graphics_OLED_printAt(42, 38, 0, "SI");
                }
                else
                {
                    Graphics_OLED_printAt(42, 38, 0, "NO");
                }
                switch ((uint8_t)(opcMenu))
                {
                    case 0:
                        Graphics_OLED_rect(1, 1, 126, 9);
                        break;
                    case 1:
                        Graphics_OLED_rect(1, 10, 126, 9);
                        break;
                    case 2:
                        Graphics_OLED_rect(1, 19, 126, 9);
                        break;
                    case 3:
                        Graphics_OLED_rect(1, 28, 126, 9);
                        break;
                    case 4:
                        Graphics_OLED_rect(1, 37, 126, 9);
                        break;
                    case 5:
                        Graphics_OLED_rect(1, 46, 126, 9);
                        break;
                    case 6:
                        Graphics_OLED_rect(1, 54, 126, 9);
                        break;
                }
            }
            break;
        case 2:
            if (key == 128)
            {
                if (opcMenu > 0)
                {
                    opcMenu = (uint8_t)(opcMenu - 1);
                    MenuScr();
                    if (lazo == 1)
                    {
                        Graphics_OLED_printAt(46, 29, 0, "AUTO");
                    }
                    else
                    {
                        Graphics_OLED_printAt(46, 29, 0, "MAN ");
                    }
                    if (tdfEst == 1)
                    {
                        Graphics_OLED_printAt(42, 38, 0, "SI");
                    }
                    else
                    {
                        Graphics_OLED_printAt(42, 38, 0, "NO");
                    }
                    switch ((uint8_t)(opcMenu))
                    {
                        case 0:
                            Graphics_OLED_rect(1, 1, 126, 9);
                            break;
                        case 1:
                            Graphics_OLED_rect(1, 10, 126, 9);
                            break;
                        case 2:
                            Graphics_OLED_rect(1, 19, 126, 9);
                            break;
                        case 3:
                            Graphics_OLED_rect(1, 28, 126, 9);
                            break;
                        case 4:
                            Graphics_OLED_rect(1, 37, 126, 9);
                            break;
                        case 5:
                            Graphics_OLED_rect(1, 46, 126, 9);
                            break;
                        case 6:
                            Graphics_OLED_rect(1, 54, 126, 9);
                            break;
                    }
                }
            }
            if (key == 129)
            {
                if (opcMenu < 6)
                {
                    opcMenu = (uint8_t)(opcMenu + 1);
                    MenuScr();
                    if (lazo == 1)
                    {
                        Graphics_OLED_printAt(46, 29, 0, "AUTO");
                    }
                    else
                    {
                        Graphics_OLED_printAt(46, 29, 0, "MAN ");
                    }
                    if (tdfEst == 1)
                    {
                        Graphics_OLED_printAt(42, 38, 0, "SI");
                    }
                    else
                    {
                        Graphics_OLED_printAt(42, 38, 0, "NO");
                    }
                    switch ((uint8_t)(opcMenu))
                    {
                        case 0:
                            Graphics_OLED_rect(1, 1, 126, 9);
                            break;
                        case 1:
                            Graphics_OLED_rect(1, 10, 126, 9);
                            break;
                        case 2:
                            Graphics_OLED_rect(1, 19, 126, 9);
                            break;
                        case 3:
                            Graphics_OLED_rect(1, 28, 126, 9);
                            break;
                        case 4:
                            Graphics_OLED_rect(1, 37, 126, 9);
                            break;
                        case 5:
                            Graphics_OLED_rect(1, 46, 126, 9);
                            break;
                        case 6:
                            Graphics_OLED_rect(1, 54, 126, 9);
                            break;
                    }
                }
            }
            if (key == 130)
            {
                pantalla = 0;
                MainScr1();
            }
            break;
        case 3:
            if (key == 130)
            {
                opcion = 0;
                ConfirmScr();
                switch ((uint8_t)(opcMenu))
                {
                    case 0:
                        Graphics_OLED_printAt(41, 6, 0, "ARRANCAR?");
                        break;
                    case 1:
                        Graphics_OLED_printAt(49, 6, 0, "PARAR?");
                        break;
                    case 2:
                        Graphics_OLED_printAt(34, 6, 0, "RESET FALLO?");
                        break;
                }
                Graphics_OLED_rect(10, 26, 44, 20);
            }
            if (key == 131)
            {
                opcion = 1;
                ConfirmScr();
                switch ((uint8_t)(opcMenu))
                {
                    case 0:
                        Graphics_OLED_printAt(41, 6, 0, "ARRANCAR?");
                        break;
                    case 1:
                        Graphics_OLED_printAt(49, 6, 0, "PARAR?");
                        break;
                    case 2:
                        Graphics_OLED_printAt(34, 6, 0, "RESET FALLO?");
                        break;
                }
                Graphics_OLED_rect(74, 26, 44, 20);
            }
            break;
    }
}


void Keyboard_Pad_onPress(uint8_t key)
{
    pI2C("TECLAPAD\t%u", key);
    if (key == 13)
    {
        switch ((uint8_t)(pantalla))
        {
            case 2:
                switch ((uint8_t)(opcMenu))
                {
                    case 0:
                        pantalla = 3;
                        opcion = 0;
                        ConfirmScr();
                        switch ((uint8_t)(opcMenu))
                        {
                            case 0:
                                Graphics_OLED_printAt(41, 6, 0, "ARRANCAR?");
                                break;
                            case 1:
                                Graphics_OLED_printAt(49, 6, 0, "PARAR?");
                                break;
                            case 2:
                                Graphics_OLED_printAt(34, 6, 0, "RESET FALLO?");
                                break;
                        }
                        Graphics_OLED_rect(10, 26, 44, 20);
                        break;
                    case 1:
                        pantalla = 3;
                        opcion = 0;
                        ConfirmScr();
                        switch ((uint8_t)(opcMenu))
                        {
                            case 0:
                                Graphics_OLED_printAt(41, 6, 0, "ARRANCAR?");
                                break;
                            case 1:
                                Graphics_OLED_printAt(49, 6, 0, "PARAR?");
                                break;
                            case 2:
                                Graphics_OLED_printAt(34, 6, 0, "RESET FALLO?");
                                break;
                        }
                        Graphics_OLED_rect(10, 26, 44, 20);
                        break;
                    case 2:
                        pantalla = 3;
                        opcion = 0;
                        ConfirmScr();
                        switch ((uint8_t)(opcMenu))
                        {
                            case 0:
                                Graphics_OLED_printAt(41, 6, 0, "ARRANCAR?");
                                break;
                            case 1:
                                Graphics_OLED_printAt(49, 6, 0, "PARAR?");
                                break;
                            case 2:
                                Graphics_OLED_printAt(34, 6, 0, "RESET FALLO?");
                                break;
                        }
                        Graphics_OLED_rect(10, 26, 44, 20);
                        break;
                    case 3:
                        if (lazo == 1)
                        {
                            lazo = 0;
                        }
                        else
                        {
                            lazo = 1;
                        }
                        pI2C("LAZO\t%u", lazo);
                        MenuScr();
                        if (lazo == 1)
                        {
                            Graphics_OLED_printAt(46, 29, 0, "AUTO");
                        }
                        else
                        {
                            Graphics_OLED_printAt(46, 29, 0, "MAN ");
                        }
                        if (tdfEst == 1)
                        {
                            Graphics_OLED_printAt(42, 38, 0, "SI");
                        }
                        else
                        {
                            Graphics_OLED_printAt(42, 38, 0, "NO");
                        }
                        switch ((uint8_t)(opcMenu))
                        {
                            case 0:
                                Graphics_OLED_rect(1, 1, 126, 9);
                                break;
                            case 1:
                                Graphics_OLED_rect(1, 10, 126, 9);
                                break;
                            case 2:
                                Graphics_OLED_rect(1, 19, 126, 9);
                                break;
                            case 3:
                                Graphics_OLED_rect(1, 28, 126, 9);
                                break;
                            case 4:
                                Graphics_OLED_rect(1, 37, 126, 9);
                                break;
                            case 5:
                                Graphics_OLED_rect(1, 46, 126, 9);
                                break;
                            case 6:
                                Graphics_OLED_rect(1, 54, 126, 9);
                                break;
                        }
                        break;
                    case 4:
                        if (tdfEst == 1)
                        {
                            pI2C("TDFOFF\t1");
                        }
                        else
                        {
                            pI2C("TDFON\t1");
                        }
                        break;
                    case 5:
                        queEdita = 0;
                        edit = 0;
                        pantalla = 4;
                        EditScr();
                        Graphics_OLED_printAt(28, 4, 0, "MARIPOSA");
                        break;
                    case 6:
                        queEdita = 1;
                        edit = 0;
                        pantalla = 4;
                        EditScr();
                        Graphics_OLED_printAt(32, 4, 0, "RPM REF");
                        break;
                }
                break;
            case 3:
                if (opcion == 1)
                {
                    switch ((uint8_t)(opcMenu))
                    {
                        case 0:
                            pI2C("START\t1");
                            break;
                        case 1:
                            pI2C("STOP\t1");
                            break;
                        case 2:
                            pI2C("RESET\t1");
                            break;
                    }
                }
                pantalla = 0;
                MainScr1();
                break;
            case 4:
                if (queEdita == 0)
                {
                    pI2C("PAREF\t%u", edit);
                }
                else
                {
                    rpmRef = edit;
                    pI2C("RPMREF\t%u", edit);
                }
                pantalla = 0;
                MainScr1();
                break;
        }
    }
    if (key == 44)
    {
        pantalla = 0;
        MainScr1();
    }
    if (pantalla == 4)
    {
        if (key >= 48)
        {
            if (key <= 57)
            {
                edit = edit * 10 + key - 48;
                if (edit > 1500)
                {
                    edit = 1500;
                }
            }
        }
    }
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
    else if (strncmp(tag, "TDFEST", 6) == 0)
    {
        tdfEst = streamIn_t_ptr_to_uint8_t((streamIn_t*)msg);
    }
    else if (strncmp(tag, "ESTADO", 6) == 0)
    {
        estado = streamIn_t_ptr_to_uint8_t((streamIn_t*)msg);
        if (pantalla == 0)
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
                    if (tdfEst == 1)
                    {
                        Graphics_OLED_printAt(66, 48, 0, "MARCHA TDF");
                    }
                    else
                    {
                        Graphics_OLED_printAt(66, 48, 0, "EN MARCHA ");
                    }
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



