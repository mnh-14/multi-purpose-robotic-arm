//
// Collected from: https://github.com/prestonsn/PCA9685-Multi-Channel-Servo-Controller-Driver-for-AVR-ATmega
//



#ifndef pca9685_h
#define pca9685_h

#define SERVO0 0x06

#define NEUTRAL_PULSE 1500
#define MAX_PULSE 2100
#define MIN_PULSE 900
#define MAX_ANGLE 90



/*
 * Configure PCA9685 slave device
 *
 * Sets the slave device MODE1, PRESCALE (controls the output frequency) and
 * MODE2 register values.
 *
 * Arguments:
 *      address    The PCA9685 hardwired 6-bit slave device address.
 *
 *      freq       Output driver frequency (in Hz). Must be a value between
 *                 24 Hz and 1526 Hz
 */
void pca9685_init(uint8_t address, uint16_t freq);

/*
 * Set the servo horn angle
 *
 * Sets the slave device MODE1, PRESCALE (controls the output frequency) and
 * MODE2 register values.
 *
 * Arguments:
 *      servoNum    The index of the servo motor. A number between 0 and 15.
 *
 *      angle       Angle in degrees. Must be a value between -90 and 90
 *                  degrees.
 */
void pca9685_servo(uint8_t servoNum, float angle);




// Md Nafis Hussain: Defines
// 

// 
enum delta { O1, O_1, O_25, O_5, O_01, O_05 };

#define MIN_VAL
#define MAX_VAL
#define SCALING_FACTOR 10000L
#define SCALE_ROUNDUP 5000
#define ANGLE_0 1024000L
#define ANGLE_180 5120000L
#define ANGLE_90 3072000L
#define ANGLE_VALUE_RANGE_WIDTH 4096000L
#define DELTA_ANGLE_0_01 227
#define DELTA_ANGLE_0_05 1138
#define DELTA_ANGLE_0_1 2276
#define DELTA_ANGLE_0_5 11378
#define DELTA_ANGLE_0_25 5689
#define DELTA_ANGLE_1 22756

#define ANGLE_LOW 0
#define ANGLE_HIGH 18000
#define ANGLE_RANGE_WIDTH 180

/* #define ANGLE_0 2048000L
#define ANGLE_180 4096000L
#define ANGLE_90 3072000L
#define DELTA_ANGLE_0_01 114
#define DELTA_ANGLE_0_05 569
#define DELTA_ANGLE_0_1 1138
#define DELTA_ANGLE_0_5 5689
#define DELTA_ANGLE_0_25 2845
#define DELTA_ANGLE_1 11378 */

/*
 * Set the servo horn angle
 * Only for 50Hz signals
 *
 * Sets the slave device MODE1, PRESCALE (controls the output frequency) and
 * MODE2 register values.
 *
 * Arguments:
 *      servoNum     The index of the servo motor. A number between 0 and 15.
 *
 *      angle_value  Angle value, the value required to get the desired angle. Must be between ANGLE_0 and ANGLE_180.
 *                   The values must also be scaled up by SCALING_FACTOR as it will be scaled down in this function
 */

uint32_t find_angle_value(uint16_t angle);
void pca9685_servo_with_value(uint8_t servoNum, uint32_t angle_value);
void pca9685_init_50(uint8_t address);
uint32_t delta_value(enum delta val);

#endif
