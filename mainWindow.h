#import <Metal/Metal.h>
#import <MetalKit/MetalKit.h>
#import <QuartzCore/CAMetalLayer.h>
#import "AppLifecycle/Renderer.h"

#define GLOBAL_WIDTH  512
#define GLOBAL_HEIGHT 288
//===========================================================================
//We create a WindowDelegate class. Inherits from NSObject, adpots NSApplicationDelegate and NSWindowDelegate
//===========================================================================
@interface
BtWindowDel: NSObject <NSApplicationDelegate, NSWindowDelegate>
@end

//Member variables
@implementation BtWindowDel
{
    // The on-screen window: title bar, frame, screen position
    NSWindow*     _window;
	MTKView*      _metalKitView;
    Renderer*     _renderer;
}
//===========================================================
//This is our initialization, called by OS via main in NSApplication
//===========================================================
- (void)applicationDidFinishLaunching:(NSNotification *)note
{
    NSRect screenRect = [[NSScreen mainScreen] frame];
    NSRect windowRect = NSMakeRect((screenRect.size.width - GLOBAL_WIDTH) * 0.5,
                                   (screenRect.size.height - GLOBAL_HEIGHT) * 0.5,
                                   GLOBAL_WIDTH, GLOBAL_HEIGHT);
    //============================================================================
    //Window Setup
    //============================================================================
    _window = [[NSWindow alloc] initWithContentRect: windowRect
                                          styleMask: NSWindowStyleMaskTitled |
                                                     NSWindowStyleMaskClosable |
                                                     NSWindowStyleMaskMiniaturizable
                                            backing: NSBackingStoreBuffered
                                              defer: NO];

    _window.releasedWhenClosed = NO;
    _window.minSize            = NSMakeSize(GLOBAL_WIDTH, GLOBAL_HEIGHT);
    _window.backgroundColor    = NSColor.orangeColor;
    _window.title              = @"Bluetooth Selector";
    _window.delegate           = self;
    //============================================================================
    //Metal Layer Setup
    //============================================================================
    _metalKitView                 = [[MTKView alloc] initWithFrame:_window.contentLayoutRect];
    _metalKitView.device          = MTLCreateSystemDefaultDevice();
    _metalKitView.framebufferOnly = YES;
    _metalKitView.colorspace      = CGColorSpaceCreateWithName(kCGColorSpaceSRGB);
	_window.contentView           = _metalKitView;
    //============================================================================
    //Renderer Setup
    //============================================================================
    if ([NSUserDefaults.standardUserDefaults boolForKey:@"keepOnTop"])
    {
        _window.level = NSFloatingWindowLevel;
    }

    [_window makeKeyAndOrderFront: nil];
    [NSApp activate];
}

- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)sender
{
    return YES;
}
@end