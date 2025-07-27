#ifndef _ANGLE_CONTROL_H_
#define _ANGLE_CONTROL_H_
#include <avr/io.h>
#include <avr/pgmspace.h>

#define TOTAL_ANGLE 91
#define ANGLE_OFFSET 0
#define ANGLE_SCALE 100
#define CACHE_SIZE 6


const int16_t theta_2_to_3[91] PROGMEM = {
    6330, 6331, 6332, 6334, 6337, 6341, 6346, 6352, 6358, 6366, 6374, 
    6383, 6393, 6404, 6416, 6428, 6441, 6456, 6470, 6486, 6503, 6520, 
    6538, 6557, 6577, 6597, 6618, 6640, 6663, 6686, 6710, 6735, 6760, 
    6786, 6813, 6841, 6869, 6897, 6927, 6956, 6987, 7018, 7050, 7082, 
    7114, 7148, 7181, 7216, 7250, 7286, 7321, 7358, 7394, 7431, 7469, 
    7507, 7545, 7584, 7623, 7662, 7702, 7742, 7782, 7823, 7864, 7905, 
    7947, 7989, 8031, 8073, 8116, 8159, 8202, 8245, 8289, 8332, 8376, 
    8420, 8464, 8508, 8553, 8597, 8642, 8686, 8731, 8776, 8820, 8865, 
    8910, 8955, 9000
};


typedef struct{
    int16_t theta2;
    int16_t theta3;
    uint8_t valid;
} cache_entry;

uint8_t cache_idx = 0;
uint8_t angle_idx = 5;

cache_entry cache[CACHE_SIZE] = {{0, 6330, 1}, {100,6331, 1}, {200,6332, 1}, {300, 6334, 1}, {400, 6337, 1}, {500,6341, 1}};


void load_cache(){
    cache[0] = cache[cache_idx];
    cache_idx = 0;
    int i = 1;
    for(; i<CACHE_SIZE; i++){
        angle_idx++;
        if(angle_idx == TOTAL_ANGLE) angle_idx = 0;
        int16_t theta3 = pgm_read_word(&theta_2_to_3[angle_idx]);
        cache[i] = {(short)(angle_idx*ANGLE_SCALE), theta3, 1};
    }
}


int8_t search_cache(int16_t angle, int16_t *range0, int16_t *range1, int16_t * theta_low){
    for(; cache_idx+1 < CACHE_SIZE; cache_idx++){
        if(cache[cache_idx].theta2 < angle) continue;
        if(cache[cache_idx+1].theta2 > angle) continue;;
        *range0 = cache[cache_idx].theta3;
        *range1 = cache[cache_idx+1].theta3;
        *theta_low = cache[cache_idx].theta2;
        return 0x01;
    }
    cache_idx++;
    load_cache();
    return search_cache(angle, range0, range1, theta_low);
}


int16_t find_theta3(int16_t theta2){
    int16_t lower, upper, theta_low;
    search_cache(theta2, &lower, &upper, &theta_low);
    int16_t val = lower + ((theta2 * theta_low) * (upper - lower) / ANGLE_SCALE);
    return val;
}


#endif