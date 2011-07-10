//
//  osx_application.m
//  Cosmos
//
//  Created by Kyle Weicht on 7/5/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import "osx_application.h"
#include "assert.h"

int ApplicationMain( int argc, const char** argv )
{
    NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
    
    // Load the main class from the plist file
    NSDictionary* infoDictionary = [[NSBundle mainBundle] infoDictionary];
    Class principalClass = NSClassFromString([infoDictionary objectForKey:@"NSPrincipalClass"]);
    NSApplication* applicationObject = [principalClass sharedApplication];
    
    // Now load the nib file
    NSString* mainNibName = [infoDictionary objectForKey:@"NSMainNibFile"];
    NSNib* mainNib = [[NSNib alloc] initWithNibNamed:mainNibName bundle:[NSBundle mainBundle]];
    [mainNib instantiateNibWithOwner:applicationObject topLevelObjects:nil];
    
    // Now run the main loop, ensuring it happens on the main thread
    if( [applicationObject respondsToSelector:@selector(run)] )
    {
        [applicationObject performSelectorOnMainThread:@selector(run) withObject:nil waitUntilDone:YES];
    }
    
    // This technically never gets called, but is here for correctness anyway
    [mainNib release];
    [pool release];
    
    return 0;
}


/*
Application interface
*/
@implementation OSXApplication

- (id)init
{
    NSAutoreleasePool* autorelease_pool = [[NSAutoreleasePool alloc] init];;
    
    self = [super init];
    if (self) 
    {
        // Initialization code here.
        _system_window  = nullptr;
        
        _running = 1;
    }
    
    [autorelease_pool release];
    
    return self;
}

-(void)dealloc
{
    [[NSNotificationCenter defaultCenter] removeObserver:self];
    [_system_window release];
    
    [super dealloc];
}

-(void)finalize
{
    [[NSNotificationCenter defaultCenter] removeObserver:self];
    [super finalize];
}

-(void*) CreateWindowWithWidth:(int)width 
                        Height:(int)height 
                    Fullscreen:(bool)fullscreen
{
    ASSERT(_system_window == nullptr);
    
    NSAutoreleasePool* autorelease_pool = [[NSAutoreleasePool alloc] init];;
    
    NSRect      window_rect;
    NSUInteger  window_style_mask;
    
    if(fullscreen)
    {
        window_style_mask = NSBorderlessWindowMask;
        window_rect = [[NSScreen mainScreen] frame];
    }
    else
    {
        // do some math to move the window to the top right corner
        NSRect fullscreen_rect = [[NSScreen mainScreen] frame];
        int origin_x = 0;
        int origin_y = fullscreen_rect.origin.y + fullscreen_rect.size.height;
        window_rect = NSMakeRect(origin_x, origin_y, width, height);
        window_style_mask = NSTitledWindowMask | NSResizableWindowMask;
    }
    
    _system_window = [[NSWindow alloc] initWithContentRect:window_rect 
                                                 styleMask:window_style_mask 
                                                   backing:NSBackingStoreBuffered 
                                                     defer:YES];
    
    [_system_window setAcceptsMouseMovedEvents:TRUE];
    _main_view = [[OSXDefaultView alloc] initWithFrame:NSMakeRect(0, 0, width, height)];
    [_main_view setParent_application:self];
    [_system_window setContentView:_main_view];
    [_system_window makeKeyAndOrderFront:self];
    
    [autorelease_pool release];
    
    return _system_window;
}

-(void) run
{
    NSAutoreleasePool* autorelease_pool = [[NSAutoreleasePool alloc] init];
    [self CreateWindowWithWidth:1280 Height:720 Fullscreen:false];
    [self finishLaunching];
    
    do 
    {
        [autorelease_pool drain];
        autorelease_pool = [[NSAutoreleasePool alloc] init];
        
        for( ;; )
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
    } while (_running);
    
    [autorelease_pool release];
}

-(void) keyDown:(NSEvent *)theEvent
{    
    char c = [[theEvent charactersIgnoringModifiers] characterAtIndex:0];
    
    printf( "%c", c);
}
-(void) keyUp:(NSEvent *)theEvent
{
    
}
-(void) mouseDown:(NSEvent *)theEvent
{
    
}
-(void) mouseUp:(NSEvent *)theEvent
{
    
}
-(void) mouseMoved:(NSEvent *)theEvent
{
    
}

@end

@implementation OSXDefaultView

@synthesize parent_application = _parent_application;
@synthesize opengl_context = _opengl_context;

-(id) init
{    
    NSAutoreleasePool* autorelease_pool = [[NSAutoreleasePool alloc] init];
    
    self = [super init];
    if( self )
    {
        _parent_application = nullptr;
        _opengl_context     = nullptr;
        
        [[NSNotificationCenter defaultCenter] addObserver:self 
                                                 selector:@selector(reshape) 
                                                     name:NSViewGlobalFrameDidChangeNotification 
                                                   object:self];
    }
    
    [autorelease_pool release];
    
    return self;
}

-(id) initWithFrame:(NSRect)frameRect
{    
    NSAutoreleasePool* autorelease_pool = [[NSAutoreleasePool alloc] init];
    
    self = [super initWithFrame:frameRect];
    
    if( self )
    {
        _parent_application = nullptr;
        _opengl_context     = nullptr;
        
        [[NSNotificationCenter defaultCenter] addObserver:self 
                                                 selector:@selector(reshape) 
                                                     name:NSViewGlobalFrameDidChangeNotification 
                                                   object:self];
    }
    
    [autorelease_pool release];
    
    return self;
}

-(void) dealloc
{
    [[NSNotificationCenter defaultCenter] removeObserver:self
                                                    name:NSViewGlobalFrameDidChangeNotification 
                                                  object:self];
    
    [super dealloc];
}

-(void) lockFocus
{
    [super lockFocus];
    
    if( [_opengl_context view] != self )
    {
        [_opengl_context setView:self];
    }
}

-(void) reshape
{    
    NSAutoreleasePool* autorelease_pool = [[NSAutoreleasePool alloc] init];
    
    NSRect bounds = [self bounds];
    // TODO: Something with the bounds
    
    [autorelease_pool release];
}

-(void) drawRect:(NSRect)dirtyRect
{
    
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
    [_parent_application keyDown:theEvent];
}

-(void) keyUp:(NSEvent *)theEvent
{
    [_parent_application keyUp:theEvent];
}

-(void) mouseDown:(NSEvent *)theEvent
{
    [_parent_application mouseDown:theEvent ];   
}
-(void) mouseUp:(NSEvent *)theEvent
{
    [_parent_application mouseUp:theEvent];   
}
-(void) mouseMoved:(NSEvent *)theEvent
{
    [_parent_application mouseMoved:theEvent];
}

@end
