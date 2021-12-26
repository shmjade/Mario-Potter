#include "../Headers/all_headers.h"

float center_x(char text[], SRC media, float fontSize){
    return (SCREEN_WIDTH - MeasureTextEx(media.fonteHP, text, fontSize, 0).x)/2;
}
