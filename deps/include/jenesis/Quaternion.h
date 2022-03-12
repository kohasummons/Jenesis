

#pragma once


#include <gl/glew.h> // holds all OpenGL type declarations

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/constants.hpp>

#include <cmath>


class Quaternion
{
public:

    // Default Constructor
    Quaternion(){}

    Quaternion(float x, float y, float z, float w)
    {
        this->x = x;
        this->y = y;
        this->z = z;
        this->w = w;

        normalize();
    }

    void normalize()
    {
        float mag = (float) sqrt(w * w + x * x + y * y + z * z);

        w /= mag;
        x /= mag;
        y /= mag;
        z /= mag;
    }


    glm::mat4 toRotationMatrix()
    {
        glm::mat4 matrix = glm::mat4(1.0f);

        const float xy = x * y;
        const float xz = x * z;
        const float xw = x * w;
        const float yz = y * z;
        const float yw = y * w;
        const float zw = z * w;
        const float xSquared = x * x;
        const float ySquared = y * y;
        const float zSquared = z * z;

        matrix[0][0] = 1 - 2 * (ySquared + zSquared);
        matrix[0][1] = 2 * (xy - zw);
        matrix[0][2] = 2 * (xz + yw);
        matrix[0][3] = 0;
        matrix[1][0] = 2 * (xy + zw);
        matrix[1][1] = 1 - 2 * (xSquared + zSquared);
        matrix[1][2] = 2 * (yz - xw);
        matrix[1][3] = 0;
        matrix[2][0] = 2 * (xz - yw);
        matrix[2][1] = 2 * (yz + xw);
        matrix[2][2] = 1 - 2 * (xSquared + ySquared);
        matrix[2][3] = 0;
        matrix[3][0] = 0;
        matrix[3][1] = 0;
        matrix[3][2] = 0;
        matrix[3][3] = 1;

        return matrix;
    }


    static Quaternion fromMatrix(glm::mat4 matrix)
    {
        float w, x, y, z;
        float diagonal = matrix[0][0] + matrix[1][1] + matrix[2][2];

        if (diagonal > 0)
        {
            float w4 = (float) (sqrt(diagonal + 1f) * 2f);
            w = w4 / 4f;
            x = (matrix[2][1] - matrix[1][2]) / w4;
            y = (matrix[0][2] - matrix[2][0]) / w4;
            z = (matrix[1][0] - matrix[0][1]) / w4;
        }
        else if ((matrix[0][0] > matrix[1][1]) && (matrix[0][0] > matrix[2][2]))
        {
            float x4 = (float) (sqrt(1f + matrix[0][0] - matrix.m11 - matrix[2][2]) * 2f);
            w = (matrix[2][1] - matrix[1][2]) / x4;
            x = x4 / 4f;
            y = (matrix[0][1] + matrix[1][0]) / x4;
            z = (matrix[0][2] + matrix[2][0]) / x4;
        }
        else if (matrix[1][1] > matrix[2][2])
        {
            float y4 = (float) (sqrt(1f + matrix[1][1] - matrix[0][0] - matrix[2][2]) * 2f);
            w = (matrix[0][2] - matrix[2][0]) / y4;
            x = (matrix[0][1] + matrix[1][0]) / y4;
            y = y4 / 4f;
            z = (matrix[1][2] + matrix[2][1]) / y4;
        }
        else
        {
            float z4 = (float) (sqrt(1f + matrix[2][2] - matrix[0][0] - matrix[1][1]) * 2f);
            w = (matrix[1][0] - matrix[0][1]) / z4;
            x = (matrix[0][2] + matrix[2][0]) / z4;
            y = (matrix[1][2] + matrix[2][1]) / z4;
            z = z4 / 4f;
        }

        return Quaternion(x, y, z, w);
    }


    static Quaternion interpolate(Quaternion a, Quaternion b, float blend)
    {
        Quaternion result = Quaternion(0, 0, 0, 1);
        float dot = a.w * b.w + a.x * b.x + a.y * b.y + a.z * b.z;
        float blendI = 1f - blend;

        if (dot < 0)
        {
            result.w = blendI * a.w + blend * -b.w;
            result.x = blendI * a.x + blend * -b.x;
            result.y = blendI * a.y + blend * -b.y;
            result.z = blendI * a.z + blend * -b.z;
        }
        else
        {
            result.w = blendI * a.w + blend * b.w;
            result.x = blendI * a.x + blend * b.x;
            result.y = blendI * a.y + blend * b.y;
            result.z = blendI * a.z + blend * b.z;
        }

        result.normalize();


        return result;
    }

private:

    float x, y, z, w;

protected:


};