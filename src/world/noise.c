#include "noise.h"

#include <math.h>
#include <stdlib.h>

#define PERM_COUNT 256
int perm[2 * PERM_COUNT] = { 0 };
float ranvec2[2 * PERM_COUNT] = { 0 };

void
perlinInit()
{
    for (int i = 0; i < 2 * PERM_COUNT; i++)
    {
        ranvec2[i] = ((float)rand() / (float)RAND_MAX - 0.5f) * 2.0f;
        perm[i] = i % PERM_COUNT;
    }
    for (int i = 0; i < 2 * PERM_COUNT; i += 2)
    {
        float l = sqrtf(
            ranvec2[i] * ranvec2[i] + ranvec2[i + 1] * ranvec2[i + 1]);
        if (l == 0.0f)
            l = 1.0f;
        ranvec2[i] /= l;
        ranvec2[i + 1] /= l;
    }

    for (int t = 0; t < 2; t++)
    {
        int* p = &perm[t * PERM_COUNT];
        for (int i = PERM_COUNT - 1; i > 0; i--)
        {
            int v = rand() % i;
            int tmp = p[i];
            p[i] = p[v];
            p[v] = tmp;
        }
    }
}

float
linearInterpolate(float c[2][2], float u, float v)
{
    float res = 0.0;
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            res += (i * u + (1.0f - i) * (1.0f - u)) * (j * v + (1.0f - j) * (1.0f - v))
                * c[i][j];
        }
    }
    return res;
}

float
perlinInterpolate(float c[2][2][2], float u, float v)
{
    float uu = u * u * (3.0f - 2.0f * u);
    float vv = v * v * (3.0f - 2.0f * v);

    float res = 0.0f;
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            float weight[2] = { u - (float)i, v - (float)j };
            res += (i * uu + (1.0f - i) * (1.0f - uu))
                * (j * vv + (1.0f - j) * (1.0f - vv))
                * (c[i][j][0] * weight[0] + c[i][j][1] * weight[1]);
        }
    }
    return res;
}

float
perlinNoise(float x, float y)
{
    float u = x - floorf(x);
    float v = y - floorf(y);

    int i = (int)(floorf(x));
    int j = (int)(floorf(y));

    float c[2][2][2];
    for (int ii = 0; ii < 2; ii++)
    {
        for (int jj = 0; jj < 2; jj++)
        {
            int idx = perm[(i + ii) & 255] ^ perm[((j + jj) & 255) + PERM_COUNT];
            for (int k = 0; k < 2; k++)
            {
                c[ii][jj][k] = ranvec2[2 * idx + k];
            }
        }
    }

    return (1.0f + perlinInterpolate(c, u, v)) * 0.5f;
}

float
perlinTurbulence(float x, float y, float amp, float freq, int depth)
{
    if (depth < 1)
        return perlinNoise(x * freq, y * freq) * amp;
    return perlinNoise(x * freq, y * freq) * amp + perlinTurbulence(x, y, amp * 0.5f, freq * 2.0f, depth - 1);
}
