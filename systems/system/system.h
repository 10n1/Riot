/*
 *  system.h
 *  graphicsDevice
 *
 *  Created by Kyle C Weicht on 1/16/12..
 *  Copyright (c) 2012 Kyle C Weicht. All rights reserved.
 */
#ifndef graphicsDevice_system_h
#define graphicsDevice_system_h

#ifdef __cplusplus /* If this is a C++ compiler, use C linkage */
extern "C" {
#endif


/*******************************************************************\
 External constants and types
\*******************************************************************/
typedef enum 
{
    kSysRunning = 0x1
} sys_flag_e;

typedef enum
{
    kOkCancel,
    kRetryCancel
} sys_mb_type_e;

typedef enum
{
    kOk,
    kCancel,
    kRetry
} sys_mb_return_e;

typedef enum
{
    kSysMouseLeft   = 0x1,
    kSysMouseRight  = 0x2,
    kSysMouseMiddle = 0x4
} sys_mouse_button_e;

typedef enum
{
    kSysKey0,
    kSysKey1,
    kSysKey2,
    kSysKey3,
    kSysKey4,
    kSysKey5,
    kSysKey6,
    kSysKey7,
    kSysKey8,
    kSysKey9,

    kSysKeyA,
    kSysKeyB,
    kSysKeyC,
    kSysKeyD,
    kSysKeyE,
    kSysKeyF,
    kSysKeyG,
    kSysKeyH,
    kSysKeyI,
    kSysKeyJ,
    kSysKeyK,
    kSysKeyL,
    kSysKeyM,
    kSysKeyN,
    kSysKeyO,
    kSysKeyP,
    kSysKeyQ,
    kSysKeyR,
    kSysKeyS,
    kSysKeyT,
    kSysKeyU,
    kSysKeyV,
    kSysKeyW,
    kSysKeyX,
    kSysKeyY,
    kSysKeyZ,

    kSysKeySpace,

    kSysKeyEscape,
    kSysKeyShift,
    kSysKeyCtrl,
    kSysKeyAlt,

    kSysKeyUp,
    kSysKeyDown,
    kSysKeyRight,
    kSysKeyLeft,

    kSysKeyF1,
    kSysKeyF2,
    kSysKeyF3,
    kSysKeyF4,
    kSysKeyF5,
    kSysKeyF6,
    kSysKeyF7,
    kSysKeyF8,
    kSysKeyF9,
    kSysKeyF10,
    kSysKeyF11,
    kSysKeyF12,

    kMAX_KEYS
} sys_key_e;

typedef struct system_t system_t;
typedef void (void_callback_t)(void);
typedef void (resize_callback_t)(int,int);
typedef void (keyboard_callback_t)(sys_key_e);
typedef unsigned int mouse_state_t;

/*******************************************************************\
 Variables
\*******************************************************************/

/*******************************************************************\
 External functions
\*******************************************************************/
system_t* sysCreate(void);
void sysShutdown(system_t* system);
void sysCreateWindow(system_t* system, int width, int height);
void sysPollSystemEvents(system_t* system);

void sysSetResizeCallback(system_t* system, resize_callback_t* callback);
void sysSetKeyboardCallback(system_t* system, keyboard_callback_t* callback);

void* sysGetWindow(system_t* system);
int sysGetKeyState(system_t* system, sys_key_e key);
int sysGetFlag(system_t* system, sys_flag_e flag);
void sysSetFlag(system_t* system, sys_flag_e flag, int value);
void sysGetMousePosition(system_t* system, int* x, int* y);
mouse_state_t sysGetMouseState(system_t* system);

sys_mb_return_e sysMessageBox(  system_t* system, 
                                const char* header, 
                                const char* message, 
                                sys_mb_type_e type);
                                
#ifdef __cplusplus /* If this is a C++ compiler, use C linkage */
} // extern "C" {
#endif


#endif /* include guard */
