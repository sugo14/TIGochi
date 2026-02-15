#include <stdio.h>
#include <math.h>
#include <stdbool.h>

#include "mesh.h"
#include "screendata.h"

// looks down z
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

            // if out of screen, continue
            // ! TEMP
            if (
                normalCoords.x < 0 || normalCoords.x > 1 ||
                normalCoords.y < 0 || normalCoords.y > 1
            ) { continue; }

            // find the integer pixel coordinates on screen
            // !!! THESE ARENT INTS ANYMORE !!!
            struct Vector2 screenCoords = {
                normalCoords.x * SCREEN_WIDTH,
                normalCoords.y * SCREEN_HEIGHT
            };

            // save integer pixel coordinates
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
                // calculate cross product with each edge vector
                bool withinTriangle = true;
                for (int j = 0; j < 3; j++) {
                    struct Vector2* edgeVector = &edgeVectors[j];
                    struct Vector2 currVector = subV2(&currCoords, &screenVertices[j]);
                    float cross = crossV2(&currVector, edgeVector);
                    // printf("(%f, %f), (%f, %f), %f\n", currVector.x, currVector.y, edgeVector->x, edgeVector->y, cross);
                    if (cross < 0) { withinTriangle = false; }
                }
                if (!withinTriangle) { continue; }
                write(&screenData, x, y);
            }
        }
    }

    // print screen
    char* outputStr = output(&screenData);
    printf("%s", outputStr);

    return 0;
}
