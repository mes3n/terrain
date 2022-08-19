#include "noise.h"

#include <math.h>

#ifndef M_PI
#define M_PI 3.1415926536
#endif  // M_PI

float pseudoRandom (float x, float z) 
{   
    int n = x + z * 57;
    n = (n<<13) ^ n;
    return (0.5 * ((n * (n*n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0f);
}

float interpolate (float x, float z) 
{
    float val = pseudoRandom(x, z);

    float ft = val * M_PI;
    float f = (1.0f - cosf(ft)) * 0.5;

    return f;
}

float smoothNoise (float x, float z, float step) 
{
    float cornersy = (interpolate(x - step, z - step) + interpolate(x + step, z - step) + 
                    interpolate(x + step, z + step) + interpolate(x - step, z + step)) * 0.0625;     
    float sidesy   = (interpolate(x - step, z) + interpolate(x + step, z) + 
                    interpolate(x, z - step) + interpolate(x, z + step)) * 0.125;
    float centery  = (interpolate(x, z)) * 0.25;

    return cornersy + sidesy + centery;
}

float noise (float x, float z, float min, float amplitude, float frequency) 
{   
    float total = min;
    static const float persitance = 0.5;
    static const int octaves = 5;
    
    for (int i = 0; i < octaves; i++) 
    {   
        amplitude *= persitance;
        frequency /= persitance;

        total += smoothNoise(x * frequency, z * frequency, frequency) * amplitude;
    }

    return total;
}