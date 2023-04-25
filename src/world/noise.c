#include "noise.h"

#include <math.h>


// float pseudoRandom (float x, float z)
// {   
//     int n = x + z*1242;
//     n = (n<<13) ^ n;
//     return (0.5 * ((n * (n*n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0f);
// }

float prandom (float x, float z) 
{
    float n = sin((127.1f + 269.5f)*x + (311.7f + 183.3f)*z)*43758.5453123f;
    return 2.0f*(n - floorf(n)) - 1.0f;
}

float interpolate (float x, float z) 
{
    float ft = prandom(x, z) * M_PI;
    return (1.0f - cosf(ft)) * 0.5f;
}

float smoothNoise (float x, float z) 
{
    float cy = (interpolate(x - 1.0f, z - 1.0f) + interpolate(x + 1.0f, z - 1.0f) + 
                    interpolate(x + 1.0f, z + 1.0f) + interpolate(x - 1.0f, z + 1.0f)) * 0.0625;     
    float sy   = (interpolate(x - 1.0f, z) + interpolate(x + 1.0f, z) + 
                    interpolate(x, z - 1.0f) + interpolate(x, z + 1.0f)) * 0.125;
    float y  = (interpolate(x, z)) * 0.25;

    return cy + sy + y;
}

float noise (float x, float z, float amp, float frq) 
{   
    return interpolate(x * frq, z * frq) * amp;

    float total = 0.0f;
    const float p = 0.5;
    const int octaves = 5;
    
    for (int i = 0; i < octaves; i++) 
    {   
        amp *= p;
        frq /= p;

        total += smoothNoise(x * frq, z * frq) * amp;
    }

    return total;
}