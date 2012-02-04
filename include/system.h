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
    int PollEvents(void);
    void GetWindowSize(int* width, int* height);

    // Input support
    int GetKeyState(Key::Enum key);
    int GetMouseButtonState(Mouse::Enum button);
    void GetMousePosition(int* x, int* y);

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
