#ifndef NOISE_H
#define NOISE_H

void
perlinInit();

float
perlinNoise(float x, float y);

float
perlinTurbulence(float x, float y, float amp, float freq, int depth);

#endif // NOISE_H
