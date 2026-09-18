#import <AppKit/AppKit.h>
#import "mainWindow.h"
//===========================================================
//Main Loop
//===========================================================
static WindowDelegate *gWindowDelegate = nil;

int main(int argc, const char *argv[])
{
    NSApplication *app = [NSApplication sharedApplication];
    gWindowDelegate    = [[WindowDelegate alloc] init];
    app.delegate       = gWindowDelegate;

    return NSApplicationMain(argc, argv);
}