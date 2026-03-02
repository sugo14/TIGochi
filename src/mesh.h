struct Vector2 {
    float x, y;
};

struct Vector2 subV2(struct Vector2* v1, struct Vector2* v2) {
    struct Vector2 res;
    res.x = v1->x - v2->x;
    res.y = v1->y - v2->y;
    return res;
}

float dotV2(struct Vector2* v1, struct Vector2* v2) {
    return v1->x * v2->x + v1->y * v2->y;
}

float crossV2(struct Vector2* v1, struct Vector2* v2) {
    return v1->x * v2->y - v1->y * v2->x;
}

struct Vector2Int {
    int x, y;
};

struct Vector2Int subV2I(struct Vector2Int* v1, struct Vector2Int* v2) {
    struct Vector2Int res;
    res.x = v1->x - v2->x;
    res.y = v1->y - v2->y;
    return res;
}

float dotV2I(struct Vector2Int* v1, struct Vector2Int* v2) {
    return v1->x * v2->x + v1->y * v2->y;
}

float crossV2I(struct Vector2Int* v1, struct Vector2Int* v2) {
    return v1->x * v2->y - v1->y * v2->x;
}

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
