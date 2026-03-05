#include <stdlib.h>

// for text
// #define SCREEN_WIDTH 60
// #define SCREEN_HEIGHT 15
#define SCREEN_WIDTH LCD_WIDTH
#define SCREEN_HEIGHT LCD_HEIGHT

struct ScreenData {
    // int screen[SCREEN_WIDTH][SCREEN_HEIGHT];
};

static inline void write(struct ScreenData* screenData, int x, int y, int val) {
    // if (x < 0 || x >= LCD_WIDTH) { return; }
    // if (y < 0 || y >= LCD_HEIGHT) { return; }

    uint16_t* lcd_Ram_i = (uint16_t*)lcd_Ram;
    lcd_Ram_i += (y * SCREEN_WIDTH + x);
    *lcd_Ram_i = 0xFFFF;
}

void fill_row(struct ScreenData* screenData, int x, int y, int amt, uint16_t val) {
    uint16_t* lcd_Ram_i = (uint16_t*)lcd_Ram;
    lcd_Ram_i += (y * SCREEN_WIDTH + x);
    for (int i = 0; i < amt; i++) {
        *lcd_Ram_i = val;
        lcd_Ram_i++;
    }
}

// char* output(struct ScreenData* screenData) {
//     char* result = (char*)malloc((SCREEN_HEIGHT * (SCREEN_WIDTH + 1) + 1) * sizeof(char));
//     for (int y = 0; y < SCREEN_HEIGHT; y++) {
//         for (int x = 0; x < SCREEN_WIDTH; x++) {
//             int pixel = screenData->screen[x][y];
//             char c = '#';
//             if (pixel == 0xFF0000) { c = '&'; }
//             else if (pixel == 0x00FF00) { c = '$'; }
//             else if (pixel == 0x0000FF) { c = '@'; }
//             result[y * (SCREEN_WIDTH + 1) + x] = pixel ? c : '.';
//         }
//         result[y * (SCREEN_WIDTH + 1) + SCREEN_WIDTH] = '\n';
//     }
//     result[SCREEN_HEIGHT * (SCREEN_WIDTH + 1)] = '\0';
//     return result;
// }
