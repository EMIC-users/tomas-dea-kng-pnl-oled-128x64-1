
/*==================[inclusions]=============================================*/

#include "inc/Graphics_OLED.h"
#include "inc/font_5x7.h"
#include "inc/font_7x9.h"
#include "inc/font_16x32.h"
#include "inc/streamOut.h"
#include "inc/streamIn.h"

/*==================[internal macros]=======================================*/

#define bit_set(var,bit)     ((var) |= ((uint64_t)1 << (bit)))
#define bit_clear(var,bit)   ((var) &= ~((uint64_t)1 << (bit)))

/*==================[internal data]=========================================*/

uint64_t Graphics_OLED_framebuffer[GFX_OLED_WIDTH];

static void putChar(char c);
static uint16_t putCharAvailable(void);
const streamOut_t streamOut_Graphic_OLED = {putChar, putCharAvailable};

static int16_t cursorX = 0;
static int16_t cursorY = 0;
static uint8_t currentFont = GFX_FONT_5X7;
static uint8_t fontWidth = 5;
static uint8_t fontHeight = 7;

static const uint8_t fontWidths[]  = {5, 7, 16};
static const uint8_t fontHeights[] = {7, 9, 32};
static const char    fontFirst[]   = {' ', ' ', '0'};
static const char    fontLast[]    = {'^', 'Z', '9'};



/*==================[init/poll]============================================*/

void Graphics_OLED_init(void)
{
    uint16_t i;
    for (i = 0; i < GFX_OLED_WIDTH; i++)
        Graphics_OLED_framebuffer[i] = 0;
    cursorX = 0;
    cursorY = 0;
    currentFont = GFX_FONT_5X7;
    fontWidth = fontWidths[GFX_FONT_5X7];
    fontHeight = fontHeights[GFX_FONT_5X7];
    SSD1322_OLED_init();
}

void Graphics_OLED_poll(void)
{


    // Flush framebuffer to the display hardware
    SSD1322_OLED_refresh(Graphics_OLED_framebuffer, GFX_OLED_WIDTH);
}

/*==================[pixel / clear]========================================*/

void Graphics_OLED_clear(void)
{
    uint16_t i;
    for (i = 0; i < GFX_OLED_WIDTH; i++)
        Graphics_OLED_framebuffer[i] = 0;
}

void Graphics_OLED_pixel(uint8_t x, uint8_t y, uint8_t set)
{
    if (x >= GFX_OLED_WIDTH || y >= GFX_OLED_HEIGHT) return;
    if (set)
        bit_set(Graphics_OLED_framebuffer[x], y);
    else
        bit_clear(Graphics_OLED_framebuffer[x], y);
}

/*==================[line]=================================================*/

void Graphics_OLED_line(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2)
{
    int16_t dx = (x2 > x1) ? (x2 - x1) : (x1 - x2);
    int16_t dy = (y2 > y1) ? (y2 - y1) : (y1 - y2);
    int16_t sx = (x1 < x2) ? 1 : -1;
    int16_t sy = (y1 < y2) ? 1 : -1;
    int16_t err = dx - dy;
    int16_t e2;
    int16_t cx = x1, cy = y1;

    for (;;)
    {
        if (cx >= 0 && cx < GFX_OLED_WIDTH && cy >= 0 && cy < GFX_OLED_HEIGHT)
            bit_set(Graphics_OLED_framebuffer[cx], cy);

        if (cx == x2 && cy == y2) break;
        e2 = 2 * err;
        if (e2 > -dy) { err -= dy; cx += sx; }
        if (e2 <  dx) { err += dx; cy += sy; }
    }
}

/*==================[rect / circle]=======================================*/



/*==================[text]=================================================*/

static void setFont(uint8_t fontId)
{
    if (fontId > 2) fontId = 0;
    currentFont = fontId;
    fontWidth = fontWidths[fontId];
    fontHeight = fontHeights[fontId];
}

static uint16_t putCharAvailable()
{
    return 1;
}

static void putChar(char c)
{
    uint8_t col, row;
    uint16_t offset;

    if (c < fontFirst[currentFont] || c > fontLast[currentFont]) return;
    offset = (uint16_t)(c - fontFirst[currentFont]) * fontWidth;

    for (col = 0; col < fontWidth; col++)
    {
        if (cursorX + col >= GFX_OLED_WIDTH) break;

        int32_t bitmap;
        switch (currentFont)
        {
            case GFX_FONT_7X9:   bitmap = font_7x9[offset + col];   break;
            case GFX_FONT_16X32: bitmap = font_16x32[offset + col]; break;
            default:             bitmap = font_5x7[offset + col];   break;
        }

        for (row = 0; row < fontHeight; row++)
        {
            if (cursorY + row >= GFX_OLED_HEIGHT) break;
            if (bitmap & ((int32_t)1 << row))
                bit_set(Graphics_OLED_framebuffer[cursorX + col], cursorY + row);
            else
                bit_clear(Graphics_OLED_framebuffer[cursorX + col], cursorY + row);
        }
    }
    cursorX += fontWidth + 1;
}

void Graphics_OLED_print(char* format, ...)
{
	va_list arg;
    va_start(arg, format);

	sendDataToStream(&streamOut_Graphic_OLED,format,arg);
	va_end(arg);

}

void Graphics_OLED_printAt(uint8_t x, uint8_t y, uint8_t fontId, char* format, ...)
{
    setFont(fontId);
    cursorX = x;
    cursorY = y;
    
    va_list arg;
    va_start(arg, format);

	sendDataToStream(&streamOut_Graphic_OLED,format,arg);

	va_end(arg);
}

/*==================[bindAt — reactive numeric fields]=====================*/



/*==================[end of file]============================================*/

