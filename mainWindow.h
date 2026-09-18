#import <Metal/Metal.h>
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
    // The area inside the window we draw in
    NSView*       _videoView;
    // The view's backing layer: supplies drawable textures and presents them to the screen
    CAMetalLayer* _metalLayer;
    // Our code: owns the Metal device/queue/pipelines and draws each frame
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
    _metalLayer                 = [[CAMetalLayer alloc] init];
    _metalLayer.device          = MTLCreateSystemDefaultDevice();
    _metalLayer.opaque          = YES;
    _metalLayer.framebufferOnly = YES;
    _metalLayer.backgroundColor = CGColorGetConstantColor(kCGColorBlack);
    _metalLayer.pixelFormat     = MTLPixelFormatRGBA16Float;
    _metalLayer.colorspace      = CGColorSpaceCreateWithName(kCGColorSpaceSRGB);
    //============================================================================
    //View Setup
    //============================================================================
    NSView* contentView              = [[NSView alloc] initWithFrame:_window.contentLayoutRect];
    _window.contentView              = contentView; //Root view of our app. Holds renderer and slider
    _videoView                       = [[NSView alloc] initWithFrame:contentView.bounds];
    _videoView.layer                 = _metalLayer; //We are setting _videoView to hold our metal renderer
    _videoView.wantsLayer            = YES;
    _videoView.autoresizingMask      = NSViewWidthSizable | NSViewHeightSizable;
    //[contentView addSubview:_videoView];
    [self updateDrawableSize];
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

//===========================================================
//Sizing
//===========================================================
- (void)updateDrawableSize
{
    NSSize backingSize  = [_videoView convertSizeToBacking:_videoView.bounds.size];
    CGSize drawableSize = CGSizeMake(MAX(1.0, floor(backingSize.width)),
                                     MAX(1.0, floor(backingSize.height)));

    _metalLayer.contentsScale = _window.backingScaleFactor;
    if (!CGSizeEqualToSize(_metalLayer.drawableSize, drawableSize))
    {
        _metalLayer.drawableSize = drawableSize;
    }
}

- (void)windowDidResize:(NSNotification *)notification
{
    [self updateDrawableSize];
}

- (void)windowDidChangeBackingProperties:(NSNotification *)notification
{
    [self updateDrawableSize];
}

- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)sender
{
    return YES;
}
@end