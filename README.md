# Weather_Station
Wheather station made with ESP32 Wroom + BME280 + TFT 2.4 parallel

You will need TFT_eSPI libraries from Bodmer and change User_Setup.h like this :

#define TFT_PARALLEL_8_BIT
#define ILI9341_DRIVER
#define TFT_HEIGHT 320 // ST7789 240 x 320
// ESP32 pins used for the parallel interface TFT

#define TFT_RST  13  // Reset pin
#define TFT_CS   12  // Chip select control pin
#define TFT_DC   14  // (RS) Data Command control pin - must use a pin in the range 0-31
#define TFT_WR   27  // Write strobe control pin - must use a pin in the range 0-31
#define TFT_RD   26

#define TFT_D2   15
#define TFT_D3   2
#define TFT_D4   25
#define TFT_D5   4
#define TFT_D6   16
#define TFT_D7   17

#define TFT_D0   5  // Must use pins in the range 0-31 for the data bus
#define TFT_D1   18  // so a single register write sets/clears all bits

//FONTS
#define LOAD_GLCD   // Font 1. Original Adafruit 8 pixel font needs ~1820 bytes in FLASH
#define LOAD_FONT2  // Font 2. Small 16 pixel high font, needs ~3534 bytes in FLASH, 96 characters
#define LOAD_FONT4  // Font 4. Medium 26 pixel high font, needs ~5848 bytes in FLASH, 96 characters
#define LOAD_FONT6  // Font 6. Large 48 pixel font, needs ~2666 bytes in FLASH, only characters 1234567890:-.apm
#define LOAD_FONT7  // Font 7. 7 segment 48 pixel font, needs ~2438 bytes in FLASH, only characters 1234567890:.
#define LOAD_FONT8  // Font 8. Large 75 pixel font needs ~3256 bytes in FLASH, only characters 1234567890:-.
#define LOAD_GFXFF  // FreeFonts. Include access to the 48 Adafruit_GFX free fonts FF1 to FF48 and custom fonts
#define SMOOTH_FONT
