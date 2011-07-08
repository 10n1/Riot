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
    ASSERT(_system_window);
    
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
    
    [autorelease_pool release];
    
    return _system_window;
}

-(void) run
{
    NSAutoreleasePool* autorelease_pool = [[NSAutoreleasePool alloc] init];
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

@end
