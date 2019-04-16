// -----------------------------------------------------------------------------------
// Configuration for OnStep using a RAMPS 1.4/1.5 Shield and Arduino Mega2560 or
// compatible all-in one devices like the recommended "MKS Gen-L" and others

// ****************************************************************************************
// NOTICE: This configuration is in development/testing       *** USE AT YOUR OWN RISK  ***
// Be sure to adhere to the design limitations of the RAMPS 1.4
// with respect to voltage, current, signaling, etc... See http://reprap.org/wiki/RAMPS_1.4           
// ****************************************************************************************

/*
 * For more information on setting OnStep up see http://www.stellarjourney.com/index.php?r=site/equipment_onstep and 
 * join the OnStep Groups.io at https://groups.io/g/onstep
 * 
 * See the Pins.Ramps14.h file for detailed information on this pin map to be sure it matches your wiring *** USE AT YOUR OWN RISK ***
 *
*/

#define Ramps14_ON   //  <- turn _ON to use this configuration

#ifdef Ramps14_ON
// -------------------------------------------------------------------------------------------------------------------------
// ADJUST THE FOLLOWING TO CONFIGURE YOUR CONTROLLER FEATURES --------------------------------------------------------------


// Default speed for Serial1 com port, Default=9600
#define SERIAL_B_BAUD_DEFAULT 9600

// Mount type, default is _GEM (German Equatorial) other options are _FORK, _FORK_ALT.  _FORK switches off Meridian Flips after (1, 2 or 3 star) alignment is done.  _FORK_ALT disables Meridian Flips (1 star align.)
// _ALTAZM is for Alt/Azm mounted 'scopes (1 star align only.)
#define MOUNT_TYPE_GEM

// Strict parking, default=_OFF.  Set to _ON and unparking is only allowed if successfully parked.  Otherwise unparking is allowed if at home and not parked (the Home/Reset command ":hF#" sets this state.) 
#define STRICT_PARKING_OFF

// ST4 interface on pins A9, 40, 42, A11.  Pin A9 is RA- (West), Pin 40 is Dec- (South), Pin 42 is Dec+ (North), Pin A11 is RA+ (East.)
// ST4_ON enables the interface.  ST4_PULLUP enables the interface and any internal pullup resistors.
// It is up to you to create an interface that meets the electrical specifications of any connected device, use at your own risk.  default=_OFF
#define ST4_PULLUP
// If SEPARATE_PULSE_GUIDE_RATE_ON is used the ST4 port is limited to guide rates <= 1X except when ST4_HAND_CONTROL_ON is used.
// Additionally, ST4_HAND_CONTROL_ON enables special features: Press and hold [E]+[W] buttons for > 2 seconds...  In this mode [E] decreases and [W] increases guide rates (or if tracking isn't on yet adjusts illuminated recticule brightness.)
// [S] for Sync (or Accept if in align mode.) [N] for Tracking on/off. -OR- Press and hold [N]+[S] buttons for > 2 seconds...  In this mode [E] selects prior and [W] next user catalog item.
// [N] to do a Goto to the catalog item.  [S] for Sound on/off.  The keypad returns to normal operation after 4 seconds of inactivity.  ST4_HAND_CONTROL_ON also adds a 100ms de-bounce to all button presses.
// Finally, during a goto pressing any button aborts the slew.  If meridian flip paused at home, pressing any button continues.  default=_ON
#define ST4_HAND_CONTROL_OFF

// Separate pulse-guide rate so centering and guiding don't disturb each other, default=_ON
#define SEPARATE_PULSE_GUIDE_RATE_ON

// Guide time limit (in seconds,) default=0 (no limit.)  A safety feature, some guides are started with one command and stopped with another.  
// If the stop command is never received the guide will continue forever unless this is enabled.
#define GUIDE_TIME_LIMIT 120

// RTC (Real Time Clock) support, default=_OFF. 
// Other options: RTC_DS3234 for a DS3234 on the default SPI interface pins (CS on Pin 53) or RTC_DS3231 for a DS3231 on the default I2C port (Pins 20 and 21, choose only one feature on these pins.)
// Optionally wire the SQW output to the PPS pin below.
#define RTC_DS3231
// PPS use _ON or _PULLUP to enable the input and use the built-in pullup resistor.  Sense rising edge on pin 2 for optional precision clock source (GPS, for example), default=_OFF
#define PPS_SENSE_ON

// PEC sense on Pin 2 or threshold value on Analog 1.  Use _ON or _PULLUP to enable the input/use the built-in pullup resistor (digital input) or provide a comparison value (see below) for analog operation, default=_OFF
// Analog values range from 0 to 1023 which indicate voltages from 0-3.3VDC on the analog pin, for example "PEC_SENSE 600" would detect an index when the voltage exceeds 2.92V
// With either index detection method, once triggered 60s must expire before another detection can happen.  This gives time for the index magnet to pass by the detector before another cycle begins.
// Ignored on Alt/Azm mounts.
#define PEC_SENSE_OFF
// PEC sense, rising edge (default with PEC_SENSE_STATE HIGH, use LOW for falling edge, ex. PEC_SENSE_ON) ; for optional PEC index
#define PEC_SENSE_STATE HIGH

// Switch close (to ground) on Pin 3 for optional limit sense (stops gotos and/or tracking), default=_OFF
#define LIMIT_SENSE_OFF

// Light status LED by sink to ground (Pin 11), default=_ON.
// _ON and OnStep keeps this illuminated to indicate that the controller is active.  When sidereal tracking this LED will rapidly flash
#define STATUS_LED_PINS_ON
// Light 2nd status LED by sink to ground (Pin 6), default=_OFF.
// _ON sets this to blink at 1 sec intervals when PPS is synced.  Turns off if tracking is stopped.  Turns on during gotos.
#define STATUS_LED2_PINS_OFF
// Light reticule LED by sink to ground (pin 5), default=_OFF.
// RETICULE_LED_PINS n, where n=0 to 255 activates this feature and sets default brightness
#define RETICULE_LED_PINS_OFF

// Sound/buzzer on pin 4, default=_OFF.
// Specify frequency for a piezo speaker (for example "BUZZER 2000") or use BUZZER_ON for a piezo buzzer.
#define BUZZER_OFF
// Sound state at startup, default=_ON.
#define DEFAULT_SOUND_ON

// Optionally adjust tracking rate to compensate for atmospheric refraction, default=_OFF
// can be turned on/off with the :Tr# and :Tn# commands regardless of this setting
#define TRACK_REFRACTION_RATE_DEFAULT_OFF

// Set to _OFF and OnStep will allow Syncs to change pier side for GEM mounts (on/off), default=_ON
#define SYNC_CURRENT_PIER_SIDE_ONLY_ON

// Set to _ON and OnStep will remember the last auto meridian flip setting (on/off), default=_OFF
#define REMEMBER_AUTO_MERIDIAN_FLIP_OFF

// Set to _ON and OnStep will travel directly across a meridian flip without visiting the home position (on/off), default=_OFF (only applies if pause at home is disabled)
#define MERIDIAN_FLIP_SKIP_HOME_OFF

// Set to _ON and OnStep will remember the last meridian flip pause at home setting (on/off), default=_OFF
#define REMEMBER_PAUSE_HOME_OFF

// Automatic homing; switch state changes at the home position (Pins 20 and 21, choose only one feature on these pins.)  For MOUNT_TYPE_GEM only.
#define HOME_SENSE_OFF               // Default _OFF, use _ON to have OnStep automatically detect and use home switches as follows
#define HOME_AXIS1_REVERSE_OFF       // Pin 20 state should be HIGH when clockwise of the home position (as seen from front,) reverse if necessary
#define HOME_AXIS2_REVERSE_OFF       // Pin 21 state should be HIGH when clockwise of the home position (as seen from above,) reverse if necessary

// ADJUST THE FOLLOWING TO MATCH YOUR MOUNT --------------------------------------------------------------------------------
#define REMEMBER_MAX_RATE_OFF        // set to _ON and OnStep will remember rates set in the ASCOM driver, Android App, etc. default=_OFF 
#define MaxRate                   64 // microseconds per microstep default setting for gotos, can be adjusted for two times lower or higher at run-time
                                     // minimum* (fastest goto) is around 32 (Mega2560,) default=96 higher is ok
                                     // * = minimum can be lower, when both AXIS1/AXIS2_MICROSTEPS are used the compiler will warn you if it's too low

#define DegreesForAcceleration   5.0 // approximate number of degrees for full acceleration or deceleration: higher values=longer acceleration/deceleration
                                     // Default=5.0, too low (about <1) can cause gotos to never end if micro-step mode switching is enabled.
#define DegreesForRapidStop      1.0 // approximate number of degrees required to stop when requested or if limit is exceeded during a slew: higher values=longer deceleration
                                     // Default=1.0, too low (about <1) can cause gotos to never end if micro-step mode switching is enabled.

#define GUIDES_DISABLE_BACKLASH_OFF  // Set to _ON to disable backlash takeup during guiding at <= 1X, default=_OFF
#define BacklashTakeupRate        25 // backlash takeup rate (in multipules of the sidereal rate): too fast and your motors will stall,
                                     // too slow and the mount will be sluggish while it moves through the backlash
                                     // for the most part this doesn't need to be changed, but adjust when needed.  Default=25

                                     // Axis1 is for RA/Az
#define StepsPerDegreeAxis1  12800.0 // calculated as    :  stepper_steps * micro_steps * gear_reduction1 * (gear_reduction2/360)
                                     // G11              :  400           * 32          * 1               *  360/360              = 12800
                                     // Axis2 is for Dec/Alt
#define StepsPerDegreeAxis2  12800.0 // calculated as    :  stepper_steps * micro_steps * gear_reduction1 * (gear_reduction2/360)
                                     // G11              :  400           * 32          * 1               *  360/360              = 12800
                                     
                                     // PEC, number of steps for a complete worm rotation (in RA), (StepsPerDegreeAxis1*360)/gear_reduction2.  Ignored on Alt/Azm mounts.
#define StepsPerWormRotationAxis1 32000L
                                     // G11              : (12800*360)/360 = 12800

#define PECBufferSize           600  // PEC, buffer size, max should be no more than 3384, your required buffer size >= StepsPerAxis1WormRotation/(StepsPerDegeeAxis1/240)
                                     // for the most part this doesn't need to be changed, but adjust when needed.  824 seconds is the default.  Ignored on Alt/Azm mounts.

#define MinutesPastMeridianE      30 // for goto's, how far past the meridian to allow before we do a flip (if on the East side of the pier) - a half hour of RA is the default = 30.  Sometimes used for Fork mounts in Align mode.  Ignored on Alt/Azm mounts.
#define MinutesPastMeridianW      30 // as above, if on the West side of the pier.  If left alone, the mount will stop tracking when it hits the this limit.  Sometimes used for Fork mounts in Align mode.  Ignored on Alt/Azm mounts.
                                     // The above two lines can be removed and settings in EEPROM will be used instead, be sure to set the Meridian limits in control software if you do this!  
                                     // If you don't remove these lines Meridian limits will return to these defaults on power up.
#define UnderPoleLimit            12 // maximum allowed hour angle (+/-) under the celestial pole.  Default=12.  Ignored on Alt/Azm mounts.
                                     // If left alone, the mount will stop tracking when it hits this limit.  Valid range is 10 to 12 hours.
#define MinDec                   -91 // minimum allowed declination, default = -91 (off)  Ignored on Alt/Azm mounts.
#define MaxDec                   +91 // maximum allowed declination, default =  91 (off)  Ignored on Alt/Azm mounts.
                                     // For example, a value of +80 would stop gotos/tracking near the north celestial pole.
                                     // For a Northern Hemisphere user, this would stop tracking when the mount is in the polar home position but
                                     // that can be easily worked around by doing an alignment once and saving a park position (assuming a 
                                     // fork/yoke mount with meridian flips turned off by setting the minutesPastMeridian values to cover the whole sky)
#define MaxAzm                   180 // Alt/Az mounts only. +/- maximum allowed Azimuth, default =  180.  Allowed range is 180 to 360

// AXIS1/2 STEPPER DRIVER CONTROL ------------------------------------------------------------------------------------------
// Axis1: Pins A0,A1 = Step,Dir (RA/Azm)
// Axis2: Pins A6,A7 = Step,Dir (Dec/Alt)

// Reverse the direction of movement.  Adjust as needed or reverse your wiring so things move in the right direction
#define AXIS1_REVERSE_OFF            // RA/Azm axis
#define AXIS2_REVERSE_OFF            // Dec/Alt axis

// Stepper driver Enable support, just wire Enable to Pins 38 (Axis1) and A2 (Axis2) and OnStep will pull these HIGH to disable stepper drivers on startup and when Parked or Homed.
// An Align, Sync, or Un-Park will enable the drivers.  Adjust below if you need these pulled LOW to disable the drivers.
#define AXIS1_DISABLE HIGH
#define AXIS2_DISABLE HIGH

// For equatorial mounts, _ON powers down the Declination axis when it's not being used to help lower power use.  During low rate guiding (<=1x) the axis stays enabled
// for 10 minutes after any guide on either axis.  Otherwise, the Dec axis is disabled (powered off) 10 seconds after movement stops.
#define AXIS2_AUTO_POWER_DOWN_OFF

// Basic stepper driver mode setup . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
// If used, this requires connections M0, M1, and M2 on Pins 23,25,27 for Axis1 (RA/Azm) and Pins 31,33,35 for Axis2 (Dec/Alt.)
// Stepper driver models are as follows: (for example AXIS1_DRIVER_MODEL DRV8825,) A4988, LV8729, RAPS128, S109, ST820, TMC2208, TMC2130 (spreadCycle,) 
// TMC2130_QUIET (stealthChop tracking,) TMC2130_VQUIET (full stealthChop mode,) add _LOWPWR for 50% power during tracking (for example: TMC2130_QUIET_LOWPWR)
#define AXIS1_DRIVER_MODEL_OFF      // Axis1 (RA/Azm):  Default _OFF, Stepper driver model (see above)
#define AXIS1_MICROSTEPS_OFF        // Axis1 (RA/Azm):  Default _OFF, Microstep mode when the scope is doing sidereal tracking (for example: AXIS1_MICROSTEPS 32)
#define AXIS1_MICROSTEPS_GOTO_OFF   // Axis1 (RA/Azm):  Default _OFF, Optional microstep mode used during gotos (for example: AXIS1_MICROSTEPS_GOTO 2)
#define AXIS2_DRIVER_MODEL_OFF      // Axis2 (Dec/Alt): Default _OFF, Stepper driver model (see above)
#define AXIS2_MICROSTEPS_OFF        // Axis2 (Dec/Alt): Default _OFF, Microstep mode when the scope is doing sidereal tracking
#define AXIS2_MICROSTEPS_GOTO_OFF   // Axis2 (Dec/Alt): Default _OFF, Optional microstep mode used during gotos
// Note: you can replace this section with the contents of "AdvancedStepperSetup.txt" . . . . . . . . . . . . . . . . . . . 

// Stepper driver Fault detection, default=_OFF.
// other settings are TMC2130.  The SPI interface (on M0/M1/M2/Aux) can be used to detect errors on the TMC2130.
#define AXIS1_FAULT_OFF
#define AXIS2_FAULT_OFF

// ------------------------------------------------------------------------------------------------------------------------
// CAMERA ROTATOR OR ALT/AZ DE-ROTATION -----------------------------------------------------------------------------------
// Pins 46,48 = Step,Dir
#define ROTATOR_OFF                  // use _ON to enable the rotator (for any mount type,) default=_OFF (this is also a de-rotator for MOUNT_TYPE_ALTAZM mounts.)
#define MaxRateAxis3               8 // this is the minimum number of milli-seconds between micro-steps, default=8
#define StepsPerDegreeAxis3     64.0 // calculated as    :  stepper_steps * micro_steps * gear_reduction1 * (gear_reduction2/360)
                                     // Rotator          :  24            * 8           * 20              *  6/360                = 64
                                     // For de-rotation of Alt/Az mounts a quick estimate of the required resolution (in StepsPerDegree)
                                     // would be an estimate of the circumference of the useful imaging circle in (pixels * 2)/360
#define MinAxis3                -180 // minimum allowed rotator angle, default = -180
#define MaxAxis3                 180 // maximum allowed rotator angle, default =  180
#define AXIS3_REVERSE_OFF            // reverse the direction of Axis3 rotator movement.
#define AXIS3_DISABLE HIGH           // Pin A8 (Aux3.)  Default HIGH, for common stepper drivers.
#define AXIS3_AUTO_POWER_DOWN_OFF    // use _ON if you want to power down the motor at stand-still.  Default _OFF.

// FOCUSER1 ---------------------------------------------------------------------------------------------------------------
// Pins 26,28 = Step,Dir
#define FOCUSER1_OFF                 // use _ON to enable this focuser, default=_OFF
#define MaxRateAxis4               8 // this is the minimum number of milli-seconds between steps, default=8.  In DC motor mode PWM frequency.
#define StepsPerMicrometerAxis4  0.5 // figure this out by testing or other means.
#define MinAxis4               -25.0 // minimum allowed position in millimeters, default = -25.0
#define MaxAxis4                25.0 // maximum allowed position in millimeters, default =  25.0
#define AXIS4_REVERSE_OFF            // reverse the direction of focuser movement.
#define AXIS4_DISABLE_OFF            // Pin 24 (Aux4.)  Default _OFF, use HIGH for common stepper drivers
#define AXIS4_AUTO_POWER_DOWN_OFF    // use _ON if you want to power down the motor at stand-still.  Default _OFF.  Ignored in DC motor mode.
#define AXIS4_DC_MODE_OFF            // enable DC focuser instead of a stepper motor.  Automatically uses Phase 1 if enabled.  Default _OFF.

// FOCUSER2 ---------------------------------------------------------------------------------------------------------------
// Pins 36,34 = Step,Dir
#define FOCUSER2_OFF                 // use _ON to enable this focuser, default=_OFF
#define MaxRateAxis5               8 // this is the minimum number of milli-seconds between steps, default=8.  In DC motor mode PWM frequency.
#define StepsPerMicrometerAxis5  0.5 // figure this out by testing or other means.
#define MinAxis5               -25.0 // minimum allowed position in millimeters, default = -25.0
#define MaxAxis5                25.0 // maximum allowed position in millimeters, default =  25.0
#define AXIS5_REVERSE_OFF            // reverse the direction of focuser movement.
#define AXIS5_DISABLE_OFF            // Pin 30 (Aux5.) Default _OFF, use HIGH for common stepper drivers
#define AXIS5_AUTO_POWER_DOWN_OFF    // use _ON if you want to power down the motor at stand-still.  Default _OFF.  Ignored in DC motor mode.
#define AXIS5_DC_MODE_OFF            // enable DC focuser instead of a stepper motor.  Automatically uses Phase 2 if enabled.  Default _OFF.

// THAT'S IT FOR USER CONFIGURATION!

// -------------------------------------------------------------------------------------------------------------------------
#define FileVersionConfig 2
#include "src/pinmaps/Pins.Ramps14.h"
#endif
