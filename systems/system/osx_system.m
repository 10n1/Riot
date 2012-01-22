/*
 *  osx_system.c
 *  graphicsDevice
 *
 *  Created by Kyle C Weicht on 1/16/12.
 *  Copyright (c) 2012 Kyle C Weicht. All rights reserved.
 */
#include "system.h"

/* C headers */
#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>
#include <crt_externs.h>
#include <CoreFoundation/CoreFoundation.h>
/* C++ headers */
/* External headers */
/* Internal headers */

#define UNUSED_PARAMETER(param) (void)(sizeof((param)))

/*
 * Define debugBreak
 */
#if defined( _MSC_VER )
    #define debugBreak() __debugbreak()
#elif defined( __GNUC__ )
    #define debugBreak() __asm__( "int $3\n" : : )
#else
    #error Unsupported compiler
#endif

/*
 * Define assert
 */
#ifndef assert
    #define assert(condition)   \
        do                      \
        {                       \
            if(!(condition))    \
            {                   \
                debugBreak();   \
            }                   \
        } while(__LINE__ == -1)
        /* This causes warning 4127 with VC++ (conditional expression is constant) */
    #if defined( _MSC_VER )
        #pragma warning(disable:4127)
    #endif /* defined( _MSC_VER ) */
#endif

#ifndef NULL
    #if defined(__GNUG__)
        #define NULL __null
    #else
        #ifdef __cplusplus
            #define NULL 0
        #else
            #define NULL ((void*)0)
        #endif
    #endif
#endif

/*******************************************************************\
 Internal constants and types
\*******************************************************************/
struct system_t
{
    uint8_t keyState[kMAX_KEYS];
    id      delegate;
    id      window;
    id      view;
    resize_callback_t* resizeCallback;
    keyboard_callback_t*    keyboardCallback;
    
    uint32_t    flags;
    mouse_state_t   mouseState;
    int             mouseX;
    int             mouseY;
};

@interface application_t : NSApplication
@end
@interface application_delegate_t : NSObject
{
@private
    system_t*   _system;
}
@property system_t* system;
@end
@interface view_t : NSView
@end

/*******************************************************************\
 Internal variables
\*******************************************************************/

/*******************************************************************\
 Internal functions
\*******************************************************************/
#define KEY_MAPPING(keyCode, sysKey) case keyCode: keyIndex = sysKey; break
static void SetKeyState(system_t* system, uint8_t key, uint8_t state)
{
    /* Reference: http://boredzo.org/blog/wp-content/uploads/2007/05/IMTx-virtual-keycodes.pdf */
    sys_key_e   keyIndex;
    
    switch(key)
    {
    /* Qwerty row */
    KEY_MAPPING(12, kSysKeyQ);
    KEY_MAPPING(13, kSysKeyW);
    KEY_MAPPING(14, kSysKeyE);
    KEY_MAPPING(15, kSysKeyR);
    KEY_MAPPING(17, kSysKeyT);
    KEY_MAPPING(16, kSysKeyY);
    KEY_MAPPING(32, kSysKeyU);
    KEY_MAPPING(34, kSysKeyI);
    KEY_MAPPING(31, kSysKeyO);
    KEY_MAPPING(35, kSysKeyP);
    
    /* Asdf row */
    KEY_MAPPING(0, kSysKeyA);
    KEY_MAPPING(1, kSysKeyS);
    KEY_MAPPING(2, kSysKeyD);
    KEY_MAPPING(3, kSysKeyF);
    KEY_MAPPING(5, kSysKeyG);
    KEY_MAPPING(4, kSysKeyH);
    KEY_MAPPING(38, kSysKeyJ);
    KEY_MAPPING(40, kSysKeyK);
    KEY_MAPPING(37, kSysKeyL);
    
    /* Zxcv row */
    KEY_MAPPING(6, kSysKeyZ);
    KEY_MAPPING(7, kSysKeyX);
    KEY_MAPPING(8, kSysKeyC);
    KEY_MAPPING(9, kSysKeyV);
    KEY_MAPPING(11, kSysKeyB);
    KEY_MAPPING(45, kSysKeyN);
    KEY_MAPPING(46, kSysKeyM);
    
    /* Numbers */
    KEY_MAPPING(18, kSysKey1);
    KEY_MAPPING(19, kSysKey2);
    KEY_MAPPING(20, kSysKey3);
    KEY_MAPPING(21, kSysKey4);
    KEY_MAPPING(23, kSysKey5);
    KEY_MAPPING(22, kSysKey6);
    KEY_MAPPING(26, kSysKey7);
    KEY_MAPPING(28, kSysKey8);
    KEY_MAPPING(25, kSysKey9);
    KEY_MAPPING(29, kSysKey0);
    
    /* Misc */
    KEY_MAPPING(53, kSysKeyEscape);
    KEY_MAPPING(56, kSysKeyShift);
    KEY_MAPPING(59, kSysKeyCtrl);
    KEY_MAPPING(58, kSysKeyAlt);
    
    /* Arrows */
    KEY_MAPPING(126, kSysKeyUp);
    KEY_MAPPING(125, kSysKeyDown);
    KEY_MAPPING(123, kSysKeyLeft);
    KEY_MAPPING(124, kSysKeyRight);
    
    /* Function keys */
    KEY_MAPPING(122, kSysKeyF1);
    KEY_MAPPING(120, kSysKeyF2);
    KEY_MAPPING(99, kSysKeyF3);
    KEY_MAPPING(118, kSysKeyF4);
    KEY_MAPPING(96, kSysKeyF5);
    KEY_MAPPING(97, kSysKeyF6);
    KEY_MAPPING(98, kSysKeyF7);
    KEY_MAPPING(100, kSysKeyF8);
    KEY_MAPPING(101, kSysKeyF9);
    KEY_MAPPING(109, kSysKeyF10);
    KEY_MAPPING(103, kSysKeyF11);
    KEY_MAPPING(111, kSysKeyF12);    
    
    default:
        return;
    }
    
    system->keyState[keyIndex] = state;
    if(system->keyboardCallback)
    {
        system->keyboardCallback(keyIndex);
    }
}
static NSString* GetAppName(void)
{
    NSDictionary*   infoDictionary = [[NSBundle mainBundle] infoDictionary];
    char**          programName;
    ProcessSerialNumber psn = {0, kCurrentProcess};
    id  name;
    
    /* Check to see if theres a plist with the BundleName */
    name = [infoDictionary objectForKey:@"CFBundleName"];
    if(name && [name isKindOfClass:[NSString class]] && [@"" isEqualToString:name])
        return name;
    
    TransformProcessType(&psn, kProcessTransformToForegroundApplication);
    SetFrontProcess(&psn);
    
    programName = _NSGetProgname();
    if(programName && *programName)
    {
        return [NSString stringWithUTF8String:*programName];
    }
    
    return NULL;
}
static NSMenu* CreateMenuBar(void)
{
    NSString*   appName = GetAppName();
    NSMenu*     menuBar = [[NSMenu alloc] init];
    NSMenuItem* appMenuItem = [menuBar addItemWithTitle:@"" action:NULL keyEquivalent:@""];
    NSMenu*     appMenu = [[NSMenu alloc] init];
    
    [appMenuItem setSubmenu:appMenu];
    [appMenu addItem:[NSMenuItem separatorItem]];
    [appMenu addItemWithTitle:[NSString stringWithFormat:@"Quit %@", appName]
                       action:@selector(terminate:)
                keyEquivalent:@"q"];
                
    return menuBar;
}
static void TurnFlagOn(uint32_t* flags, uint32_t flag)
{
    *flags |= flag;
}
static void TurnFlagOff(uint32_t* flags, uint32_t flag)
{
    *flags &= ~flag;
} 

/*******************************************************************\
 External variables
\*******************************************************************/

/*******************************************************************\
 External functions
\*******************************************************************/
system_t* sysCreate(void)
{
    NSAutoreleasePool* autoreleasePool = [[NSAutoreleasePool alloc] init];
    id          delegate = NULL;
    system_t*   system = NULL;
    NSString*   appDirectory = NULL;
    NSMenu*     mainMenu = NULL;
    
    /* Application */
    [application_t sharedApplication];
    appDirectory = [[NSBundle mainBundle] resourcePath];
    if(access([appDirectory cStringUsingEncoding:NSUTF8StringEncoding], R_OK) == 0)
    {
        chdir([appDirectory cStringUsingEncoding:NSUTF8StringEncoding] );
    }
    mainMenu = CreateMenuBar();
    [NSApp setMainMenu:mainMenu];
    [NSApp finishLaunching];
    
    /* Create delegate */
    delegate = [[application_delegate_t alloc] init];
    [NSApp setDelegate:delegate];
    
    /*
     * Create system object
     */
    system = malloc(sizeof(system_t));
    memset(system, 0, sizeof(*system));
    
    [delegate setSystem:system];
    system->delegate = delegate;
    system->flags |= kSysRunning;
    
    [autoreleasePool release];
    
    return system;
}
void sysShutdown(system_t* system)
{
    UNUSED_PARAMETER(system);
}
void sysCreateWindow(system_t* system, int width, int height)
{
    NSAutoreleasePool* autorelease_pool = [[NSAutoreleasePool alloc] init];
    
    NSRect          window_rect;
    NSUInteger      window_style_mask;
    NSWindow*       systemWindow = NULL;
    NSRect          fullscreenRect;
    int             origin_x;
    int             origin_y;
    
    /* do some math to move the window to the top right corner */
    fullscreenRect = [[NSScreen mainScreen] frame];
    origin_x = 0;
    origin_y = (int)(fullscreenRect.origin.y + fullscreenRect.size.height);
    window_rect = NSMakeRect(origin_x, origin_y, width, height);
    window_style_mask = NSTitledWindowMask | NSResizableWindowMask;
    systemWindow = [[NSWindow alloc] initWithContentRect:window_rect 
                                                styleMask:window_style_mask 
                                                  backing:NSBackingStoreBuffered 
                                                    defer:YES];
    
    [systemWindow setContentView:[[view_t alloc] init]];
    [systemWindow setDelegate:system->delegate];
    [systemWindow setAcceptsMouseMovedEvents:TRUE];
    [systemWindow makeKeyAndOrderFront:nil];
    
    system->window = systemWindow;
    
    [autorelease_pool release];
}
void sysPollSystemEvents(system_t* system)
{
    NSEvent* event;
    UNUSED_PARAMETER(system);
    /* Now run the main loop, ensuring it happens on the main thread */
    do
    {
        event = [NSApp nextEventMatchingMask:NSAnyEventMask 
                                   untilDate:[NSDate distantPast] 
                                      inMode:NSDefaultRunLoopMode 
                                     dequeue:YES];
        if(event)
        {
            [NSApp sendEvent:event];
        }
    }while(event);
}

int sysGetFlag(system_t* system, sys_flag_e flag)
{
    return system->flags & flag;
}
void* sysGetWindow(system_t* system)
{
    return system->window;
}
void sysSetResizeCallback(system_t* system, resize_callback_t* callback)
{
    system->resizeCallback = callback;
}
void sysSetKeyboardCallback(system_t* system, keyboard_callback_t* callback)
{
    system->keyboardCallback = callback;
}
void sysSetFlag(system_t* system, sys_flag_e flag, int value)
{
    if(value)
        system->flags |= flag;
    else
        system->flags &= ~flag;
}
int sysGetKeyState(system_t* system, sys_key_e key)
{
    return system->keyState[key];
}

void sysGetMousePosition(system_t* system, int* x, int* y)
{
    *x = system->mouseX;
    *y = system->mouseY;
}
mouse_state_t sysGetMouseState(system_t* system)
{
    return system->mouseState;
}
sys_mb_return_e sysMessageBox(  system_t* system, 
                                const char* header, 
                                const char* message, 
                                sys_mb_type_e type)
{
    /*convert the strings from char* to CFStringRef */
    CFStringRef header_ref  = CFStringCreateWithCString(NULL, header, kCFStringEncodingASCII);
    CFStringRef message_ref = CFStringCreateWithCString(NULL, message, kCFStringEncodingASCII);
    CFStringRef okButtonText = (type == kOkCancel) ? NULL : CFSTR("Retry");

    CFOptionFlags result;  /*result code from the message box */
   
    /*launch the message box */
    CFUserNotificationDisplayAlert( 0.0f, /* no timeout */
                                    kCFUserNotificationNoteAlertLevel, /*change it depending message_type flags ( MB_ICONASTERISK.... etc.) */
                                    NULL, /*icon url, use default, you can change it depending message_type flags */
                                    NULL, /*not used */
                                    NULL, /*localization of strings */
                                    header_ref, /*header text  */
                                    message_ref, /*message text */
                                    okButtonText, /*default "ok" text in button */
                                    CFSTR("Cancel"), /*alternate button title */
                                    NULL, /*other button title, null--> no other button */
                                    &result /*response flags */ );

    /*Clean up the strings */
    CFRelease( header_ref );
    CFRelease( message_ref );

    /*Convert the result */
    if(result == kCFUserNotificationDefaultResponse)
        return (type == kOkCancel) ? kOk : kRetry;
    
    return kCancel;
    UNUSED_PARAMETER(system);
}

/*
 Application interface
 */
 
@implementation application_t
- (void)sendEvent:(NSEvent *)event
{
    if([event type] == NSKeyUp && ([event modifierFlags] & NSCommandKeyMask))
    {
        [[self keyWindow] sendEvent:event];
    }
    else
    {
        [super sendEvent:event];
    }
}
@end


@implementation application_delegate_t

@synthesize system = _system;

-(BOOL)windowShouldClose:(id)window
{
    UNUSED_PARAMETER(window);
    return NO;
}
-(NSApplicationTerminateReply)applicationShouldTerminate:(NSApplication*)sender
{
    UNUSED_PARAMETER(sender);
    TurnFlagOff(&_system->flags, (uint32_t)kSysRunning);
    return NSTerminateNow;
}
-(void)windowDidBecomeKey:(NSNotification*)notification
{
    UNUSED_PARAMETER(notification);
}

@end

@implementation view_t

- (BOOL)isOpaque
{
    return YES;
}

- (BOOL)canBecomeKeyView
{
    return YES;
}

- (BOOL)acceptsFirstResponder
{
    return YES;
}
- (void)reshape
{
    application_delegate_t* delegate = [[self window] delegate];
    system_t*               system = [delegate system];
    NSRect bounds = [self bounds];
    if(system->resizeCallback)
        system->resizeCallback((int)bounds.size.width, (int)bounds.size.height);
    (void)sizeof(bounds);
}
- (void)keyDown:(NSEvent *)theEvent
{
    application_delegate_t* delegate = [[self window] delegate];
    system_t*               system = [delegate system];
    unsigned short key = [theEvent keyCode];
    SetKeyState(system, (uint8_t)key, 1);
}
-(void)keyUp:(NSEvent *)theEvent
{
    application_delegate_t* delegate = [[self window] delegate];
    system_t*               system = [delegate system];
    unsigned short key = [theEvent keyCode];
    SetKeyState(system, (uint8_t)key, 0);
}
-(void)mouseMoved:(NSEvent *)theEvent
{
    application_delegate_t* delegate = [[self window] delegate];
    system_t*               system = [delegate system];
    NSRect  bounds = [self bounds];
    NSPoint mousePosition = [theEvent locationInWindow];
    int x = (int)mousePosition.x;
    int y = (int)(bounds.size.height-mousePosition.y);
    
    system->mouseX = x;
    system->mouseY = y;
}
-(void)mouseDragged:(NSEvent *)theEvent
{
    [self mouseMoved:theEvent];
}
-(void)rightMouseDragged:(NSEvent *)theEvent
{
    [self mouseMoved:theEvent];
}
-(void)mouseDown:(NSEvent *)theEvent
{
    application_delegate_t* delegate = [[self window] delegate];
    system_t*               system = [delegate system];
    system->mouseState |= kSysMouseLeft;    
    UNUSED_PARAMETER(theEvent);
}

-(void)rightMouseDown:(NSEvent *)theEvent:(NSEvent *)theEvent
{
    application_delegate_t* delegate = [[self window] delegate];
    system_t*               system = [delegate system];
    system->mouseState |= kSysMouseLeft;
    UNUSED_PARAMETER(theEvent);
}
-(void)mouseUp:(NSEvent *)theEvent
{
    application_delegate_t* delegate = [[self window] delegate];
    system_t*               system = [delegate system];
    system->mouseState &= ~kSysMouseLeft;    
    UNUSED_PARAMETER(theEvent);
}

-(void)rightMouseUp:(NSEvent *)theEvent:(NSEvent *)theEvent
{
    application_delegate_t* delegate = [[self window] delegate];
    system_t*               system = [delegate system];
    system->mouseState &= ~kSysMouseLeft;
    UNUSED_PARAMETER(theEvent);
}

@end
