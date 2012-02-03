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
//typedef enum 
//{
//    kSysRunning = 0x1
//} sys_flag_e;
//
//typedef enum
//{
//    kOkCancel,
//    kRetryCancel
//} sys_mb_type_e;
//
//typedef enum
//{
//    kOk,
//    kCancel,
//    kRetry
//} sys_mb_return_e;
//
//typedef enum
//{
//    kSysMouseLeft   = 0x1,
//    kSysMouseRight  = 0x2,
//    kSysMouseMiddle = 0x4
//} sys_mouse_button_e;
//
//typedef enum
//{
//    kSysKey0,
//    kSysKey1,
//    kSysKey2,
//    kSysKey3,
//    kSysKey4,
//    kSysKey5,
//    kSysKey6,
//    kSysKey7,
//    kSysKey8,
//    kSysKey9,
//
//    kSysKeyA,
//    kSysKeyB,
//    kSysKeyC,
//    kSysKeyD,
//    kSysKeyE,
//    kSysKeyF,
//    kSysKeyG,
//    kSysKeyH,
//    kSysKeyI,
//    kSysKeyJ,
//    kSysKeyK,
//    kSysKeyL,
//    kSysKeyM,
//    kSysKeyN,
//    kSysKeyO,
//    kSysKeyP,
//    kSysKeyQ,
//    kSysKeyR,
//    kSysKeyS,
//    kSysKeyT,
//    kSysKeyU,
//    kSysKeyV,
//    kSysKeyW,
//    kSysKeyX,
//    kSysKeyY,
//    kSysKeyZ,
//
//    kSysKeySpace,
//
//    kSysKeyEscape,
//    kSysKeyShift,
//    kSysKeyCtrl,
//    kSysKeyAlt,
//
//    kSysKeyUp,
//    kSysKeyDown,
//    kSysKeyRight,
//    kSysKeyLeft,
//
//    kSysKeyF1,
//    kSysKeyF2,
//    kSysKeyF3,
//    kSysKeyF4,
//    kSysKeyF5,
//    kSysKeyF6,
//    kSysKeyF7,
//    kSysKeyF8,
//    kSysKeyF9,
//    kSysKeyF10,
//    kSysKeyF11,
//    kSysKeyF12,
//
//    kMAX_KEYS
//} sys_key_e;
//
//typedef struct system_t system_t;
//typedef void (void_callback_t)(void);
//typedef void (resize_callback_t)(int,int);
//typedef void (keyboard_callback_t)(sys_key_e);
//typedef unsigned int mouse_state_t;
//
///*******************************************************************\
// Variables
//\*******************************************************************/
//
///*******************************************************************\
// External functions
//\*******************************************************************/
//system_t* sysCreate(void);
//void sysShutdown(system_t* system);
//void sysCreateWindow(system_t* system, int width, int height);
//void sysPollSystemEvents(system_t* system);
//void sysStop(system_t* system);
//
//void sysSetResizeCallback(system_t* system, resize_callback_t* callback);
//void sysSetKeyboardCallback(system_t* system, keyboard_callback_t* callback);
//
//void* sysGetWindow(system_t* system);
//int sysGetKeyState(system_t* system, sys_key_e key);
//int sysGetFlag(system_t* system, sys_flag_e flag);
//void sysSetFlag(system_t* system, sys_flag_e flag, int value);
//void sysGetMousePosition(system_t* system, int* x, int* y);
//mouse_state_t sysGetMouseState(system_t* system);
//void sysGetWindowSize(system_t* system, int* width, int* height);
//const char* sysGetExecutableDirectory(void);
//
//sys_mb_return_e sysMessageBox(  system_t* system, 
//                                const char* header, 
//                                const char* message, 
//                                sys_mb_type_e type);

namespace System
{
    namespace MessageBoxResult
    {    
        enum Enum
        {
            kOk,
            kCancel,
            kRetry
        };

    }
    namespace MessageBoxType
    {
        enum Enum
        {
            kOkCancel,
            kRetryCancel
        };

    }
    namespace Mouse
    {
        enum Enum
        {
            kLeft   = 0x1,
            kRight  = 0x2,
            kMiddle = 0x4
        };
    }
    namespace Key
    {
        enum Enum
        {
            k0,
            k1,
            k2,
            k3,
            k4,
            k5,
            k6,
            k7,
            k8,
            k9,

            kA,
            kB,
            kC,
            kD,
            kE,
            kF,
            kG,
            kH,
            kI,
            kJ,
            kK,
            kL,
            kM,
            kN,
            kO,
            kP,
            kQ,
            kR,
            kS,
            kT,
            kU,
            kV,
            kW,
            kX,
            kY,
            kZ,

            kSpace,

            kEscape,
            kShift,
            kCtrl,
            kAlt,

            kUp,
            kDown,
            kRight,
            kLeft,

            kF1,
            kF2,
            kF3,
            kF4,
            kF5,
            kF6,
            kF7,
            kF8,
            kF9,
            kF10,
            kF11,
            kF12,

            kMAX_KEYS
        };
    }

    // Standard stuff
    void Init(int window, int windowWidth, int windowHeight);
    void Shutdown(void);
    int IsRunning(void);
    void Stop(void);

    // Window stuff
    void* GetWindow(void);
    void PollEvents(void);
    void GetWindowSize(int* width, int* height);

    // Input support
    int GetKeyState(Key::Enum key);
    int GetMouseButtonState(Mouse::Enum button);
    int GetMousePosition(int* x, int* y);

    // Utility stuff
    void ChangeWorkingDir(const char* dir);
    const char* GetWorkingDir(void);
    const char* GetExecutableDir(void);

    // Misc
    MessageBoxResult::Enum MessageBox(  const char* header, 
                                        const char* message, 
                                        MessageBoxType::Enum type);
}

#ifdef __cplusplus /* If this is a C++ compiler, use C linkage */
} // extern "C" {
#endif


#endif /* include guard */
