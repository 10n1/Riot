/*
 *  application_macosx.mm
 *  game
 *
 *  Created by Kyle C Weicht on 1/4/12.
 *  Copyright (c) 2012 Kyle C Weicht. All rights reserved.
 */
#include "application.h"
#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>


/*
 OS X Application stuff
*/
@class OSXDefaultView;

@interface OSXApplication : NSApplication
{
@private
    OSXDefaultView* _main_view;
}
@property (atomic,retain) OSXDefaultView* main_view;
@end

/*
 Main OpenGL View
 */
@interface OSXDefaultView : NSView 
{
@private
    NSApplication*      _parent_application;
    NSOpenGLContext*    _opengl_context;
}
@property (atomic,retain) NSApplication*   parent_application;
@property (atomic,retain) NSOpenGLContext* opengl_context;

-(void) initGLContext:(CGLContextObj)context;

@end

namespace
{

/*******************************************************************\
 Internal constants and types
\*******************************************************************/

void DefaultFrameCallback(void) { }

/*******************************************************************\
 Internal variables
\*******************************************************************/
char                s_executableDir[FILENAME_MAX] = {0};
NSApplication*      s_systemApplication = nullptr;
NSNib*              s_mainNib           = nullptr;
frame_callback_t*   s_frameCallback     = &DefaultFrameCallback;

application_status_e    s_status    = kNotStarted;
int                     s_shutdown  = 0;

/*******************************************************************\
 Internal functions
\*******************************************************************/

} // namespace


namespace Application
{

/*******************************************************************\
 External variables
\*******************************************************************/

/*******************************************************************\
 External functions
\*******************************************************************/
void Initialize(void)
{
    NSDictionary* infoDictionary;
    NSString* className;
    Class principalClass;
    NSString* mainNibName;
    
    /* Load the main class from the plist file */
    infoDictionary = [[NSBundle mainBundle] infoDictionary];
    className = [infoDictionary objectForKey:@"NSPrincipalClass"];
    principalClass = NSClassFromString(className);
    s_systemApplication = [principalClass sharedApplication];
    assert(s_systemApplication);
    
    /* Now load the nib file */
    mainNibName = [infoDictionary objectForKey:@"NSMainNibFile"];
    s_mainNib = [[NSNib alloc] initWithNibNamed:mainNibName bundle:[NSBundle mainBundle]];
    assert(s_mainNib);
    [s_mainNib instantiateNibWithOwner:s_systemApplication topLevelObjects:nil];
    
    /* Set current working directory to app directory */
    const char* executableDirectory = GetExecutableDirectory();
    chdir(executableDirectory);
}
void StartMainLoop(void)
{
    /* Now run the main loop, ensuring it happens on the main thread */
    if( [s_systemApplication respondsToSelector:@selector(run)] )
    {
        [s_systemApplication performSelectorOnMainThread:@selector(run) withObject:nil waitUntilDone:YES];
    }
    
    /* This technically never gets called, but is here for correctness anyway */
    //return 0;
}
void Shutdown(void)
{
    s_shutdown = 1;
}
void SetFrameCallback(frame_callback_t* callback)
{
    s_frameCallback = callback;
}

application_status_e GetStatus(void);
const char* GetExecutableDirectory(void)
{
    NSString* bundle_path = [[NSBundle mainBundle] bundlePath];
    const char* executable_path = [[bundle_path stringByDeletingLastPathComponent] UTF8String];
    
    return executable_path;
}

void* GetOSApplication(void);   // Windows: HINSTANCE
                                // OS X:    NSApplication*

} // namespace Application


/*******************************************************************\
 OS X stuff
\*******************************************************************/

/*
 Application interface
 */
@implementation OSXApplication

@synthesize main_view = _main_view;

- (id)init
{
    self = [super init];
    if (self) 
    {
        
        _running = 1;
    }

    return self;
}

-(void)dealloc
{
    [[NSNotificationCenter defaultCenter] removeObserver:self];
}

-(void)finalize
{
    [[NSNotificationCenter defaultCenter] removeObserver:self];
    [super finalize];
}

-(void) applicationWillFinishLaunching
{
}
-(void) mouseDown:(NSEvent *)theEvent       
{ 
    //s_mouse_click_callback(true,  kMouseLeft); 
    (void)sizeof(theEvent); 
}
-(void) mouseUp:(NSEvent *)theEvent         
{ 
    //s_mouse_click_callback (false, kMouseLeft); 
    (void)sizeof(theEvent); 
}
-(void) rightMouseDown:(NSEvent *)theEvent  
{ 
    //s_mouse_click_callback (true,  kMouseRight); 
    (void)sizeof(theEvent); 
}
-(void) rightMouseUp:(NSEvent *)theEvent    
{ 
    //s_mouse_click_callback (false, kMouseRight); 
    (void)sizeof(theEvent);
}
-(void) mouseMoved:(NSEvent *)theEvent          
{ 
    NSWindow*   window;
    NSRect      bounds;
    NSPoint     mouse_position;
    int         x, y;

    window = [self keyWindow];
    bounds = [[window contentView] bounds];
    mouse_position = [theEvent locationInWindow];
    
    x = (int)mouse_position.x;
    y = (int)(-mouse_position.y + bounds.size.height);
    
    //s_mouse_move_callback(x, y);
}
-(void) mouseDragged:(NSEvent *)theEvent        { [self mouseMoved:theEvent]; }
-(void) rightMouseDragged:(NSEvent *)theEvent   { [self mouseMoved:theEvent]; }
-(void) keyDown:(NSEvent *)theEvent
{
    char c = (char)[[theEvent charactersIgnoringModifiers] characterAtIndex:0];
    //s_keyboard_callback(1,c);
    (void)sizeof(theEvent);
}

-(void) run
{
    [self finishLaunching];
    
    do 
    {
        for(int i=0;i<8;++i) /* Only process 8 messages at a time 
                                before allowing the engine to run */
        {
            NSEvent* event = [self nextEventMatchingMask:NSAnyEventMask
                                               untilDate:[NSDate distantPast]
                                                  inMode:NSDefaultRunLoopMode
                                                 dequeue:YES];
            
            if( event == nil ) 
                break;
            
            [self sendEvent:event];
            [self updateWindows];
            
            if( !_running )
                break;
        }
        
        /*
         */
        s_frameCallback();
        /*
         */
        if(s_shutdown)
            break;
        
    } while (_running);
}

-(void) terminate:(id)sender
{    
    /*
     */
    s_status = kStopped;
    
    [super terminate:sender];
}

@end


@implementation OSXDefaultView

@synthesize parent_application = _parent_application;
@synthesize opengl_context = _opengl_context;

-(id) init
{    
    NSNotificationCenter*   defaultCenter = NULL;
    
    self = [super init];
    if( self )
    {
        _parent_application = NULL;
        _opengl_context     = NULL;
        
        defaultCenter = [NSNotificationCenter defaultCenter];
        [defaultCenter  addObserver:self 
                           selector:@selector(reshape) 
                               name:NSViewGlobalFrameDidChangeNotification 
                             object:self];
    }
    
    return self;
}

-(id) initWithFrame:(NSRect)frameRect
{    
    NSNotificationCenter*   defaultCenter = NULL;
    
    self = [super initWithFrame:frameRect];
    
    if( self )
    {
        _parent_application = NULL;
        _opengl_context     = NULL;
        
        defaultCenter = [NSNotificationCenter defaultCenter];
        [defaultCenter addObserver:self 
                          selector:@selector(reshape) 
                              name:NSViewGlobalFrameDidChangeNotification 
                            object:self];
    }
    
    return self;
}

-(void) dealloc
{
    NSNotificationCenter* defaultCenter = [NSNotificationCenter defaultCenter];
    [defaultCenter removeObserver:self
                             name:NSViewGlobalFrameDidChangeNotification 
                           object:self];
}

-(void) lockFocus
{
    [super lockFocus];
    
    if( [_opengl_context view] != self )
    {
        [_opengl_context setView:self];
    }
    
    [_opengl_context makeCurrentContext];
}

-(void) reshape
{
    NSRect bounds = [self bounds];
    [_opengl_context update];
    //s_resize_callback((int)bounds.size.width, (int)bounds.size.height);
    (void)sizeof(bounds);
}

-(void) initGLContext:(CGLContextObj)context
{
    _opengl_context = [[NSOpenGLContext alloc] initWithCGLContextObj:context];
    [_opengl_context setView:self];
}

-(void) drawRect:(NSRect)dirtyRect
{
    (void)sizeof(dirtyRect); 
}

-(BOOL) acceptsFirstResponder
{
    return YES;
}

-(BOOL) setAcceptsMouseMovedEvents
{
    return YES;
}

-(void) keyDown:(NSEvent *)theEvent
{
    [_parent_application performSelector:@selector(keyDown:) withObject:theEvent];
}

-(void) keyUp:(NSEvent *)theEvent
{
    [_parent_application performSelector:@selector(keyUp:) withObject:theEvent];
}

-(void) mouseDown:(NSEvent *)theEvent       { [_parent_application performSelector:@selector(mouseDown:) withObject:theEvent]; }
-(void) mouseUp:(NSEvent *)theEvent         { [_parent_application performSelector:@selector(mouseUp:) withObject:theEvent]; }
-(void) rightMouseDown:(NSEvent *)theEvent  { [_parent_application performSelector:@selector(rightMouseDown:) withObject:theEvent]; }
-(void) rightMouseUp:(NSEvent *)theEvent    { [_parent_application performSelector:@selector(rightMouseUp:) withObject:theEvent]; }

-(void) mouseMoved:(NSEvent *)theEvent          { [_parent_application performSelector:@selector(mouseMoved:) withObject:theEvent];}
-(void) mouseDragged:(NSEvent *)theEvent        { [_parent_application performSelector:@selector(mouseDragged:) withObject:theEvent]; }
-(void) rightMouseDragged:(NSEvent *)theEvent   { [_parent_application performSelector:@selector(rightMouseDragged:) withObject:theEvent]; }

@end

