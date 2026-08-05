#ifndef USERFNCFILE_H
#define USERFNCFILE_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

// includes type definitions
#include "streamOut.h"
#include "streamIn.h"

/* Function Prototypes */
void MainScr1(void);
void MainScr2(void);
void ConfirmScr(void);
void MenuScr(void);
void EditScr(void);
void onReset();
void etOut1();
void Keyboard_Nav_onPress(uint8_t key);
void Keyboard_Pad_onPress(uint8_t key);
void eI2C(char* tag, const streamIn_t* const msg);

#endif /* USERFNCFILE_H */

