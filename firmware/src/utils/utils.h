#pragma once

/*value means brightness*/
void convertSpeedToValue();

void regulateFPS(uint32_t ts);

void convertHUEandVALtoRGB();

double mapValueto255(double value, double inMin, double inMax);



