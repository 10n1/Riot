//
//  osx_application.h
//  Cosmos
//
//  Created by Kyle Weicht on 7/5/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//
#pragma once

#import <Cocoa/Cocoa.h>
#import <Foundation/Foundation.h>
#include "global.h"

// Replacement for NSApplicationMain
int ApplicationMain( int argc, const char** argv );

@class OSXDefaultView;
@interface OSXApplication : NSApplication
{
@private
    NSWindow*       _system_window;    
    OSXDefaultView* _main_view;
}

-(void*) CreateWindowWithWidth:(int)width 
                        Height:(int)height 
                    Fullscreen:(bool)fullscreen;
-(void) keyDown:(NSEvent *)theEvent;
-(void) keyUp:(NSEvent *)theEvent;
-(void) mouseDown:(NSEvent *)theEvent;
-(void) mouseUp:(NSEvent *)theEvent;
-(void) mouseMoved:(NSEvent *)theEvent;

@end


@interface OSXDefaultView : NSView 
{
@private
    NSApplication*      _parent_application;
    NSOpenGLContext*    _opengl_context;
}
@property (retain) NSApplication* parent_application;
@property (retain) NSOpenGLContext* opengl_context;

@end