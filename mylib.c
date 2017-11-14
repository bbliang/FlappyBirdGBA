#include <string.h>
#include "mylib.h"

u16* videoBuffer = (u16*)0x6000000;

void setPixel(int row, int col, u16 color)
{
    videoBuffer[row * 240 + col] = color;
}

void drawRect(int row, int col, int height, int width, u16 color) {
    for (int r = 0; r < height; r++)
    {
        DMA[3].src = &color;
        DMA[3].dst = &videoBuffer[OFFSET(row + r, col, 240)];
        DMA[3].cnt = width | DMA_SOURCE_FIXED | DMA_DESTINATION_INCREMENT | DMA_ON;
    }
}

void delay(int n)
{
    volatile int size = 0;
    for (int i = 0; i < n * 100; i++)
    {
        size = size + 1;
    }
}

void waitForVBlank()
{
    while (SCANLINECOUNTER > 160);
    while (SCANLINECOUNTER < 160);
}

void drawImage3(int r, int c, int width, int height, const u16 *image)
{
    for (int row = 0; row < height; row++)
    {
        // Set the source
        DMA[3].src = image + OFFSET(row, 0, width);

        // Set the destination
        DMA[3].dst = &videoBuffer[OFFSET(r + row, c, 240)];

        // Set our additional options in the controller (cnt)
        DMA[3].cnt = width | DMA_SOURCE_INCREMENT | DMA_DESTINATION_INCREMENT | DMA_ON;
    }
}

void clear(int r, int c, int width, int height, const u16 *image) {
    for (int row = 0; row < height; ++row) {
        DMA[3].src = image + OFFSET(r + row, c, 240);
        DMA[3].dst = videoBuffer + OFFSET(r + row, c, 240);
        DMA[3].cnt = width |
                                 DMA_SOURCE_INCREMENT |
                                 DMA_DESTINATION_INCREMENT |
                                 DMA_ON;
    }
}

void drawImage3FromCol(int r, int c, int colOffset, int width, int height, const u16 *image)
{
    for (int row = 0; row < height; ++row)
    {
        DMA[3].src = image + OFFSET(row, colOffset, width);
        DMA[3].dst = videoBuffer + OFFSET(r + row, c, 240);
        DMA[3].cnt = (width - colOffset) | DMA_SOURCE_INCREMENT | DMA_DESTINATION_INCREMENT | DMA_ON;
    }
}

void drawBackground(const u16 *image)
{
    DMA[3].src = image;
    DMA[3].dst = videoBuffer;
    DMA[3].cnt = 38400 | DMA_SOURCE_INCREMENT | DMA_DESTINATION_INCREMENT | DMA_ON;
}

void drawChar(int row, int col, char ch, unsigned short color)
{
	for(int r=0; r< 8; r++)
	{
		for(int c=0; c<6; c++)
		{
			if (fontdata_6x8[OFFSET(r, c, 6) + ch*48] == 1)
			{
				setPixel(row+r, col+c, color);
			}
		}
	}
}

void drawString(int row, int col, char str[], unsigned short color)
{
	while(*str)
	{
		drawChar(row, col, *str++, color);
		col += 6;

	}
}
