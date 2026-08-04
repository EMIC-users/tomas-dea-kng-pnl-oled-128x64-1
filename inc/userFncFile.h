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
void MenuScr(void);
void ConfirmScr(void);
void MainScr1(void);
void MainScr2(void);
void onReset();
void Keyboard_Nav_onPress(uint8_t key);
void Keyboard_Pad_onPress(uint8_t key);
void eI2C(char* tag, const streamIn_t* const msg);

#endif /* USERFNCFILE_H */

