#include <stdlib.h>

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

struct ScreenData {
    int screen[SCREEN_WIDTH][SCREEN_HEIGHT];
};

char* output(struct ScreenData* screenData) {
    char* result = (char*)malloc((SCREEN_HEIGHT * (SCREEN_WIDTH + 1) + 1) * sizeof(char));
    for (int y = 0; y < SCREEN_HEIGHT; y++) {
        for (int x = 0; x < SCREEN_WIDTH; x++) {
            result[y * (SCREEN_WIDTH + 1) + x] = screenData->screen[x][y] ? '#' : ' ';
        }
        result[y * (SCREEN_WIDTH + 1) + SCREEN_WIDTH] = '\n';
    }
    result[SCREEN_HEIGHT * (SCREEN_WIDTH + 1)] = '\0';
    return result;
}
