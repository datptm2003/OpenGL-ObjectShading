#include <cmath>

struct Vec3 {
    float x, y, z;
};

struct Mat4 {
    float m[16];
};

class Matrix {
public:
    static Mat4 identityMatrix() {
        Mat4 mat = {};
        mat.m[0] = mat.m[5] = mat.m[10] = mat.m[15] = 1.0f;
        return mat;
    }

    static Mat4 perspectiveMatrix(float fov, float aspect, float near, float far) {
        Mat4 mat = {};
        float tanHalfFov = tanf(fov / 2.0f);
        mat.m[0] = 1.0f / (aspect * tanHalfFov);
        mat.m[5] = 1.0f / tanHalfFov;
        mat.m[10] = -(far + near) / (far - near);
        mat.m[11] = -1.0f;
        mat.m[14] = -(2.0f * far * near) / (far - near);
        return mat;
    }

    static Mat4 translateMatrix(float x, float y, float z) {
        Mat4 mat = identityMatrix();
        mat.m[12] = x;
        mat.m[13] = y;
        mat.m[14] = z;
        return mat;
    }

    static Mat4 lookAtMatrix(Vec3 eye, Vec3 center, Vec3 up) {
        Vec3 f = { center.x - eye.x, center.y - eye.y, center.z - eye.z };
        float fMag = sqrt(f.x * f.x + f.y * f.y + f.z * f.z);
        f = { f.x / fMag, f.y / fMag, f.z / fMag };

        Vec3 s = { up.y * f.z - up.z * f.y, up.z * f.x - up.x * f.z, up.x * f.y - up.y * f.x };
        float sMag = sqrt(s.x * s.x + s.y * s.y + s.z * s.z);
        s = { s.x / sMag, s.y / sMag, s.z / sMag };

        Vec3 u = { s.y * f.z - s.z * f.y, s.z * f.x - s.x * f.z, s.x * f.y - s.y * f.x };

        Mat4 mat = identityMatrix();
        mat.m[0] = s.x; mat.m[4] = s.y; mat.m[8] = s.z;
        mat.m[1] = u.x; mat.m[5] = u.y; mat.m[9] = u.z;
        mat.m[2] = -f.x; mat.m[6] = -f.y; mat.m[10] = -f.z;
        mat.m[12] = -(s.x * eye.x + s.y * eye.y + s.z * eye.z);
        mat.m[13] = -(u.x * eye.x + u.y * eye.y + u.z * eye.z);
        mat.m[14] = f.x * eye.x + f.y * eye.y + f.z * eye.z;
        return mat;
    }

};