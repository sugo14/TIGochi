#include <math.h>    // ! check if i need this
#include <stdbool.h> // ! check if i need this

// ! TEMP: printing to screen test
#include <stdlib.h>
#include <ti/getcsc.h>
#include <ti/screen.h>
#include <sys/rtc.h>
#include <sys/lcd.h>
#include <sys/util.h>

#include "mesh.h"
#include "screendata.h"

#define swap(a, b, T) T tmp = a; a = b; b = tmp

#define float_bitshift 12
#define to_float(a) ((a) << float_bitshift)
#define to_int(a) ((a) >> float_bitshift)

#define min(a, b) (((a) < (b)) ? (a) : (b))

void FillScreen(uint8_t color) { memset(lcd_Ram, color, LCD_SIZE); }

// looks down +z
// position is 0, 0, 0
// aspectRatio is x/y, fov measures x angle
struct Camera {
    float halfFOV;
    float aspectRatio;
};

struct Camera camera = {
    0.7,
    LCD_WIDTH / LCD_HEIGHT
};
struct ScreenData screenData = {0};
struct Mesh mesh = {
	{
		{1.000000, -1.000000, -1.000000},
		{1.000000, -1.000000, 1.000000},
		{-1.000000, -1.000000, 1.000000},
		{-1.000000, -1.000000, -1.000000},
		{1.000000, 1.000000, -1.000000},
		{1.000000, 1.000000, 1.000000},
		{-1.000000, 1.000000, 1.000000},
		{-1.000000, 1.000000, -1.000000},
	},
	{
		{1, 2, 3},
		{7, 6, 5},
		{4, 5, 1},
		{5, 6, 2},
		{2, 6, 7},
		{0, 3, 7},
		{0, 1, 3},
		{4, 7, 5},
		{0, 4, 1},
		{1, 5, 2},
		{3, 2, 7},
		{4, 0, 7},
	}
};

int main() {
    FillScreen(0x00);

    float tanHalfFOV = tan(camera.halfFOV);
    float tanHalfFOVy = tan(camera.halfFOV / camera.aspectRatio);
    
    for (int i = 0; i < 12; i++) {
        struct Triangle* triangle = &mesh.triangles[i];

        // find vertex screen coordinates
        struct Vector2Int screenVertices[3];
        for (int j = 0; j < 3; j++) {
            struct Vector3 vertex = mesh.vertexList[triangle->vertexIndices[j]];
            vertex.z += 5; // !!! TEMP

            // find plane of vertex
            struct Vector2 maxCoords = {
                tanHalfFOV * vertex.z,
                tanHalfFOVy * vertex.z
            };

            // normalize position of vertex on screen (0 to 1)
            struct Vector2 normalCoords = {
                vertex.x / (maxCoords.x * 2) + 0.5,
                1 - (vertex.y / (maxCoords.y * 2) + 0.5) // higher Y = lower pixel row
            };

            // find the integer pixel coordinates on screen
            // !!! THESE ARENT INTS ANYMORE !!! this seems bad
            struct Vector2Int screenCoords = {
                normalCoords.x * SCREEN_WIDTH,
                normalCoords.y * SCREEN_HEIGHT
            };
            screenVertices[j] = screenCoords;
        }

        // ------ BARYCENTRIC RASTERIZATION ------ //
        // ! this is not updated with Vector2Int

        // // calculate bounding box
        // struct Vector2Int boxMin = screenVertices[0], boxMax = screenVertices[0];
        // for (int j = 1; j < 3; j++) {
        //     struct Vector2Int* screenVertex = &screenVertices[j];
        //     if (screenVertex->x < boxMin.x) { boxMin.x = screenVertex->x; }
        //     if (screenVertex->y < boxMin.y) { boxMin.y = screenVertex->y; }
        //     if (screenVertex->x > boxMax.x) { boxMax.x = screenVertex->x; }
        //     if (screenVertex->y > boxMax.y) { boxMax.y = screenVertex->y; }
        // }
        // if (boxMin.x < 0) { boxMin.x = 0; }
        // if (boxMin.y < 0) { boxMin.y = 0; }
        // if (boxMax.x >= SCREEN_WIDTH) { boxMax.x = SCREEN_WIDTH - 1; }
        // if (boxMax.y >= SCREEN_HEIGHT) { boxMax.y = SCREEN_HEIGHT - 1; }

        // // calculate edge vectors
        // struct Vector2 edgeVectors[3];
        // for (int j = 0; j < 3; j++) {
        //     // !!! HOW DOES THIS WORK??
        //     // struct Vector2* vertex1 = &screenVertices[j - 1], vertex2 = screenVertices[j];
        //     struct Vector2* vertex1 = &screenVertices[j];
        //     struct Vector2* vertex2 = &screenVertices[(j + 1) % 3];
        //     edgeVectors[j] = subV2(vertex2, vertex1);
        // }

        // // rasterize
        // // !!! does this work without casting ihnc
        // for (int x = boxMin.x; x <= boxMax.x; x++) {
        //     for (int y = boxMin.y; y <= boxMax.y; y++) {
        //         struct Vector2 currCoords = {x, y};

        //         // calculate barycentric areas
        //         bool withinTriangle = true;
        //         float bary[3];
        //         for (int j = 0; j < 3; j++) {
        //             struct Vector2* edgeVector = &edgeVectors[j];
        //             struct Vector2 currVector = subV2(&currCoords, &screenVertices[j]);
        //             float cross = crossV2(&currVector, edgeVector);
        //             if (cross < 0) { withinTriangle = false; }
        //             bary[j] = cross;
        //         }
        //         if (!withinTriangle) { continue; }

        //         // normalize barycentric coords
        //         float sum = bary[0] + bary[1] + bary[2];
        //         for (int j = 0; j < 3; j++) { bary[j] /= sum; }

        //         // funny "shading"
        //         int maxBary = 0;
        //         for (int j = 1; j < 3; j++) {
        //             if (bary[j] > bary[maxBary]) { maxBary = j; }
        //         }
        //         if (maxBary == 0) { write(&screenData, x, y, 0xFF0000); }
        //         else if (maxBary == 1) { write(&screenData, x, y, 0x00FF00); }
        //         else { write(&screenData, x, y, 0x0000FF); }
        //     }
        // }

        // ------ SCANLINE RASTERIZATION ------ //

        // // sort vertices from top to bottom
        // struct Vector2Int v0 = screenVertices[0];
        // struct Vector2Int v1 = screenVertices[1];
        // struct Vector2Int v2 = screenVertices[2];
        // if (v2.y < v1.y) { swap(v1, v2, struct Vector2Int); }
        // if (v1.y < v0.y) { swap(v0, v1, struct Vector2Int); }
        // if (v2.y < v1.y) { swap(v1, v2, struct Vector2Int); }

        // // long edge x and inv slope
        // unsigned int xl_f = to_float(v0.x);
        // unsigned int dxdyl_f = to_float(v2.x - v0.x) / (v2.y - v0.y);

        // // top short edge x and inv slope
        // unsigned int xs_f = to_float(v0.x);
        // unsigned int dxdys_f = to_float(v1.x - v0.x) / (v1.y - v0.y);

        // // down the long edge and top short edge
        // for (unsigned int y = v0.y; y <= v1.y; y++) {
        //     unsigned int xl = to_int(xl_f);
        //     unsigned int xs = to_int(xs_f);

        //     if (xl > xs) { swap(xl, xs, int); }
        //     fill_row(&screenData, xl, y, xs - xl + 1, 0x77);

        //     xl_f += dxdyl_f;
        //     xs_f += dxdys_f;
        // }

        // // bottom short edge x and inv slope
        // xs_f = to_float(v1.x);
        // dxdys_f = to_float(v2.x - v1.x) / (v2.y - v1.y);

        // // down the long edge and bottom short edge
        // for (unsigned int y = v1.y; y <= v2.y; y++) {
        //     unsigned int xl = to_int(xl_f);
        //     unsigned int xs = to_int(xs_f);

        //     if (xl > xs) { swap(xl, xs, int); }
        //     fill_row(&screenData, xl, y, xs - xl + 1, 0x77);

        //     xl_f += dxdyl_f;
        //     xs_f += dxdys_f;
        // }

        // ------ WIREFRAME BRESENHAMS ------ //

        // render each edge
        for (int j = 0; j < 3; j++) {
            struct Vector2Int v1 = screenVertices[j];
            struct Vector2Int v2 = screenVertices[(j + 1) % 3];
            int dy = v2.y - v1.y;
            if (dy < 0) {
                swap(v1, v2, struct Vector2Int);
                dy = -dy;
            }

            // handle horizontal line edge case
            if (dy == 0) {
                fill_row(&screenData, ((v1.x < v2.x) ? v1.x : v2.x), v1.y, abs(v2.x - v1.x), 0xFFFFFF);
                continue;
            }

            int dx = v2.x - v1.x;
            int adx = abs(dx), sx = (dx > 0) ? 1 : -1;
            int x = v1.x, accum = 0;

            for (int y = v1.y; y < v2.y; y++) {
                write(&screenData, x, y, 0xFFFFFF);
                accum += adx;
                while (accum >= dy && dx != 0) {
                    accum -= dy;
                    x += sx;
                    write(&screenData, x, y, 0xFFFFFF);
                }
            }
        }
    }

    while (!os_GetCSC());

    // print screen
    // char* outputStr = output(&screenData);
    // dbg_printf("%s", outputStr);

    return 0;
}
