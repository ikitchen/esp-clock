#if !defined(SEGMENTS_H)
#define SEGMENTS_H
#include <LiquidCrystal_I2C.h>

#define SEGMENTS_WIDTH 3
#define SEGMENTS_HEIGHT 2
#define SEGMENTS_SIZE (SEGMENTS_WIDTH * SEGMENTS_HEIGHT)

#define SEG_TOP_LEFT 0
#define SEG_TOP_RIGHT 1
#define SEG_BOTTOM_LEFT 2
#define SEG_BOTTOM_RIGHT 3
#define SEG_BAR_3_2 4
#define SEG_BAR_1_3 5
#define SEG_BAR_TOP 6
#define SEG_BAR_BOTTOM 7
#define SEG_BAR_DARK 255
#define SEG_SPACE ' '

class Segments
{
private:
    LiquidCrystal_I2C *lcd;

    const char ch_empty[SEGMENTS_SIZE] = {
        SEG_SPACE,
        SEG_SPACE,
        SEG_SPACE,
        SEG_SPACE,
        SEG_SPACE,
        SEG_SPACE,
    };

    const char ch_0[SEGMENTS_SIZE] = {
        SEG_TOP_LEFT,
        SEG_BAR_TOP,
        SEG_TOP_RIGHT,
        SEG_BOTTOM_LEFT,
        SEG_BAR_BOTTOM,
        SEG_BOTTOM_RIGHT,
    };

    const char ch_1[SEGMENTS_SIZE] = {
        SEG_SPACE,
        SEG_BAR_TOP,
        SEG_TOP_RIGHT,
        SEG_SPACE,
        SEG_SPACE,
        SEG_BAR_DARK,
    };

    const char ch_2[SEGMENTS_SIZE] = {
        SEG_BAR_TOP,
        SEG_BAR_3_2,
        SEG_TOP_RIGHT,
        SEG_BOTTOM_LEFT,
        SEG_BAR_1_3,
        SEG_BAR_BOTTOM,
    };

    const char ch_3[SEGMENTS_SIZE] = {
        SEG_BAR_TOP,
        SEG_BAR_3_2,
        SEG_TOP_RIGHT,
        SEG_BAR_BOTTOM,
        SEG_BAR_1_3,
        SEG_BOTTOM_RIGHT,
    };

    const char ch_4[SEGMENTS_SIZE] = {
        SEG_TOP_LEFT,
        SEG_SPACE,
        SEG_TOP_RIGHT,
        SEG_BAR_TOP,
        SEG_BAR_TOP,
        SEG_BAR_DARK,
    };

    const char ch_5[SEGMENTS_SIZE] = {
        SEG_TOP_LEFT,
        SEG_BAR_3_2,
        SEG_BAR_TOP,
        SEG_BAR_BOTTOM,
        SEG_BAR_1_3,
        SEG_BOTTOM_RIGHT,
    };

    const char ch_6[SEGMENTS_SIZE] = {
        SEG_TOP_LEFT,
        SEG_BAR_3_2,
        SEG_BAR_TOP,
        SEG_BOTTOM_LEFT,
        SEG_BAR_1_3,
        SEG_BOTTOM_RIGHT,
    };

    const char ch_7[SEGMENTS_SIZE] = {
        SEG_TOP_LEFT,
        SEG_BAR_TOP,
        SEG_TOP_RIGHT,
        SEG_SPACE,
        SEG_SPACE,
        SEG_BAR_DARK,
    };

    const char ch_8[SEGMENTS_SIZE] = {
        SEG_TOP_LEFT,
        SEG_BAR_3_2,
        SEG_TOP_RIGHT,
        SEG_BOTTOM_LEFT,
        SEG_BAR_1_3,
        SEG_BOTTOM_RIGHT,
    };

    const char ch_9[SEGMENTS_SIZE] = {
        SEG_TOP_LEFT,
        SEG_BAR_3_2,
        SEG_TOP_RIGHT,
        SEG_BAR_BOTTOM,
        SEG_BAR_1_3,
        SEG_BOTTOM_RIGHT,
    };

    void printSegments(const char *segs, uint8_t startX, uint8_t startY)
    {
        for (uint8_t i = 0; i < SEGMENTS_SIZE; i++)
        {
            uint8_t x = startX + i % SEGMENTS_WIDTH;
            uint8_t y = startY + i / SEGMENTS_WIDTH;
            lcd->setCursor(x, y);
            lcd->print((char)segs[i]);
        }
    }

    void printColon(uint8_t startX, uint8_t startY)
    {
        lcd->setCursor(startX, startY);
        lcd->print((char)SEG_BAR_BOTTOM);
        lcd->setCursor(startX, startY + 1);
        lcd->print((char)SEG_BAR_BOTTOM);
    }

public:
    Segments(LiquidCrystal_I2C *lcd)
    {
        this->lcd = lcd;
    }

    void printChar(char ch, uint8_t x, uint8_t y)
    {
        switch (ch)
        {
        case '0':
            printSegments(ch_0, x, y);
            break;
        case '1':
            printSegments(ch_1, x, y);
            break;
        case '2':
            printSegments(ch_2, x, y);
            break;
        case '3':
            printSegments(ch_3, x, y);
            break;
        case '4':
            printSegments(ch_4, x, y);
            break;
        case '5':
            printSegments(ch_5, x, y);
            break;
        case '6':
            printSegments(ch_6, x, y);
            break;
        case '7':
            printSegments(ch_7, x, y);
            break;
        case '8':
            printSegments(ch_8, x, y);
            break;
        case '9':
            printSegments(ch_9, x, y);
            break;
        case ':':
            printColon(x, y);
            break;

        default:
            printSegments(ch_empty, x, y);
            break;
        }
    }

    void initChars()
    {
        uint8_t topLeft[8] = {
            0b00111,
            0b01111,
            0b11111,
            0b11111,
            0b11111,
            0b11111,
            0b11111,
            0b11111,
        };
        uint8_t topRight[8] = {
            0b11100,
            0b11110,
            0b11111,
            0b11111,
            0b11111,
            0b11111,
            0b11111,
            0b11111,
        };
        uint8_t bottomLeft[8] = {
            0b11111,
            0b11111,
            0b11111,
            0b11111,
            0b11111,
            0b11111,
            0b01111,
            0b00111,
        };
        uint8_t bottomRight[8] = {
            0b11111,
            0b11111,
            0b11111,
            0b11111,
            0b11111,
            0b11111,
            0b11110,
            0b11100,
        };
        uint8_t bar3_2[8] = {
            0b11111,
            0b11111,
            0b11111,
            0b00000,
            0b00000,
            0b00000,
            0b11111,
            0b11111,
        };
        uint8_t bar1_3[8] = {
            0b11111,
            0b00000,
            0b00000,
            0b00000,
            0b00000,
            0b11111,
            0b11111,
            0b11111,
        };
        uint8_t barTop[8] = {
            0b11111,
            0b11111,
            0b11111,
            0b00000,
            0b00000,
            0b00000,
            0b00000,
            0b00000,
        };
        uint8_t barBottom[8] = {
            0b00000,
            0b00000,
            0b00000,
            0b00000,
            0b00000,
            0b11111,
            0b11111,
            0b11111,
        };

        lcd->createChar(SEG_TOP_LEFT, topLeft);
        lcd->createChar(SEG_TOP_RIGHT, topRight);
        lcd->createChar(SEG_BOTTOM_LEFT, bottomLeft);
        lcd->createChar(SEG_BOTTOM_RIGHT, bottomRight);
        lcd->createChar(SEG_BAR_3_2, bar3_2);
        lcd->createChar(SEG_BAR_1_3, bar1_3);
        lcd->createChar(SEG_BAR_TOP, barTop);
        lcd->createChar(SEG_BAR_BOTTOM, barBottom);
    }
};

#endif // SEGMENTS_H
