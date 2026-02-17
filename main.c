#include <stdio.h>
#include <math.h>
#include <stdbool.h>

#include "mesh.h"
#include "screendata.h"

// looks down +z
// position is 0, 0, 0
// aspectRatio is x/y, fov measures x angle
struct Camera {
    float halfFOV;
    float aspectRatio;
};

struct Camera camera = {
    0.7,
    2
};
struct ScreenData screenData = {0};
struct Mesh mesh = {
    {
        {1, 1, 5},
        {-1, 1, 5},
        {-1, -1, 5}
    },
    {
        {0, 1, 2}
    }
};

int main() {
    for (int i = 0; i < 1; i++) {
        struct Triangle* triangle = &mesh.triangles[i];

        // find vertex screen coordinates
        struct Vector2 screenVertices[3];
        for (int j = 0; j < 3; j++) {
            struct Vector3* vertex = &mesh.vertexList[triangle->vertexIndices[j]];

            // find plane of vertex
            struct Vector2 maxCoords = {
                tan(camera.halfFOV) * vertex->z,
                tan(camera.halfFOV / camera.aspectRatio) * vertex->z
            };

            // normalize position of vertex on screen (0 to 1)
            struct Vector2 normalCoords = {
                vertex->x / (maxCoords.x * 2) + 0.5,
                1 - (vertex->y / (maxCoords.y * 2) + 0.5) // higher Y = lower pixel row
            };

            // find the integer pixel coordinates on screen
            // !!! THESE ARENT INTS ANYMORE !!! this seems bad
            struct Vector2 screenCoords = {
                normalCoords.x * SCREEN_WIDTH,
                normalCoords.y * SCREEN_HEIGHT
            };
            screenVertices[j] = screenCoords;
        }

        // calculate bounding box
        struct Vector2 boxMin = screenVertices[0], boxMax = screenVertices[0];
        for (int j = 1; j < 3; j++) {
            struct Vector2* screenVertex = &screenVertices[j];
            if (screenVertex->x < boxMin.x) { boxMin.x = screenVertex->x; }
            if (screenVertex->y < boxMin.y) { boxMin.y = screenVertex->y; }
            if (screenVertex->x > boxMax.x) { boxMax.x = screenVertex->x; }
            if (screenVertex->y > boxMax.y) { boxMax.y = screenVertex->y; }
        }
        if (boxMin.x < 0) { boxMin.x = 0; }
        if (boxMin.y < 0) { boxMin.y = 0; }
        if (boxMax.x >= SCREEN_WIDTH) { boxMax.x = SCREEN_WIDTH - 1; }
        if (boxMax.y >= SCREEN_HEIGHT) { boxMax.y = SCREEN_HEIGHT - 1; }

        // calculate edge vectors
        struct Vector2 edgeVectors[3];
        for (int j = 0; j < 3; j++) {
            // !!! HOW DOES THIS WORK??
            // struct Vector2* vertex1 = &screenVertices[j - 1], vertex2 = screenVertices[j];
            struct Vector2* vertex1 = &screenVertices[j];
            struct Vector2* vertex2 = &screenVertices[(j + 1) % 3];
            edgeVectors[j] = subV2(vertex2, vertex1);
        }

        // rasterize
        // !!! does this work without casting ihnc
        for (int x = boxMin.x; x <= boxMax.x; x++) {
            for (int y = boxMin.y; y <= boxMax.y; y++) {
                struct Vector2 currCoords = {x, y};

                // calculate barycentric areas
                bool withinTriangle = true;
                float bary[3];
                for (int j = 0; j < 3; j++) {
                    struct Vector2* edgeVector = &edgeVectors[j];
                    struct Vector2 currVector = subV2(&currCoords, &screenVertices[j]);
                    float cross = crossV2(&currVector, edgeVector);
                    if (cross < 0) { withinTriangle = false; }
                    bary[j] = cross;
                }
                if (!withinTriangle) { continue; }

                // normalize barycentric coords
                float sum = bary[0] + bary[1] + bary[2];
                for (int j = 0; j < 3; j++) { bary[j] /= sum; }

                // funny "shading"
                int maxBary = 0;
                for (int j = 1; j < 3; j++) {
                    if (bary[j] > bary[maxBary]) { maxBary = j; }
                }
                if (maxBary == 0) { write(&screenData, x, y, 0xFF0000); }
                else if (maxBary == 1) { write(&screenData, x, y, 0x00FF00); }
                else { write(&screenData, x, y, 0x0000FF); }
            }
        }
    }

    // print screen
    char* outputStr = output(&screenData);
    printf("%s", outputStr);

    return 0;
}
