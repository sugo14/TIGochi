#include <stdlib.h>

// #define SCREEN_WIDTH 320
// #define SCREEN_HEIGHT 240
#define SCREEN_WIDTH 60
#define SCREEN_HEIGHT 15

struct ScreenData {
    int screen[SCREEN_WIDTH][SCREEN_HEIGHT];
};

void write(struct ScreenData* screenData, int x, int y, int val) {
    screenData->screen[x][y] = val;
}

char* output(struct ScreenData* screenData) {
    char* result = (char*)malloc((SCREEN_HEIGHT * (SCREEN_WIDTH + 1) + 1) * sizeof(char));
    for (int y = 0; y < SCREEN_HEIGHT; y++) {
        for (int x = 0; x < SCREEN_WIDTH; x++) {
            int pixel = screenData->screen[x][y];
            char c = '#';
            if (pixel == 0xFF0000) { c = '&'; }
            else if (pixel == 0x00FF00) { c = '$'; }
            else if (pixel == 0x0000FF) { c = '@'; }
            result[y * (SCREEN_WIDTH + 1) + x] = pixel ? c : '.';
        }
        result[y * (SCREEN_WIDTH + 1) + SCREEN_WIDTH] = '\n';
    }
    result[SCREEN_HEIGHT * (SCREEN_WIDTH + 1)] = '\0';
    return result;
}
