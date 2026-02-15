struct Vector3 {
    float x, y, z;
};

struct Triangle {
    int vertexIndices[3];
};

struct Mesh {
    struct Vector3 vertexList[3];
    struct Triangle triangles[1];
};
