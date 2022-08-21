#ifndef NOISE_H
#define NOISE_H

float pseudoRandom (float x, float z);
float interpolate (float x, float z);

float smoothNoise (float x, float z, float step);
float noise (float x, float z, float min, float amplitude, float frequency);

#endif  // NOISE_H
 