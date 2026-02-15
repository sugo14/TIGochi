#include <stdio.h>

#include <math.h>

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
        for (int j = 0; j < 3; j++) {
            struct Vector3* vertex = &mesh.vertexList[triangle->vertexIndices[j]];

            // find plane of vertex
            float maxX = tan(camera.halfFOV) * vertex->z;
            float maxY = tan(camera.halfFOV / camera.aspectRatio) * vertex->z;

            // normalize position of vertex on screen (0 to 1)
            float normalX = vertex->x / (maxX * 2) + 0.5;
            float normalY = vertex->y / (maxY * 2) + 0.5;

            // if out of screen, continue
            if (normalX < 0 || normalX > 1 || normalY < 0 || normalY > 1)
                { continue; }

            // find the integer pixel coordinates on screen
            int coordsX = normalX * SCREEN_WIDTH;
            int coordsY = normalY * SCREEN_HEIGHT;

            // write coordinates
            write(&screenData, coordsX, coordsY);
        }
    }

    // print screen
    char* outputStr = output(&screenData);
    printf("%s", outputStr);

    return 0;
}
