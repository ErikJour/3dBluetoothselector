#import <QuartzCore/QuartzCore.h>
#define PATH_TO_VIDEOS    "Desktop/Consulting/SoulEqmacOS/VideoResources"
#define BACKGROUND_DAY    "DayLoop_Comp.mov"
#define BACKGROUND_NIGHT  "NiteLoop_Comp.mov"
#define DECK_DAY_NOFIRE   "Deck_Day_NoFire_AlphaCH_Comp_V2.mov"
#define DECK_DAY_FIRE     "Deck_Day_Fire_Final_AlphaCH_Comp.mov"
#define DECK_NIGHT_NOFIRE "Deck_Night_NoFire_AlphaCH_Comp.mov"
#define DECK_NIGHT_FIRE   "Deck_Night_Fire_AlphaCH_Comp_V2.mov"
#define WINDOW_DAY        "Deck_Day_WindowCut_AlphaCH_Comp.mov"
#define WINDOW_NIGHT      "Deck_Night_WindowCut_AlphaCH_Comp.mov"

@interface Renderer : NSObject <CAMetalDisplayLinkDelegate>

- (nonnull instancetype)initWithMetalLayer:(nonnull CAMetalLayer *)metalLayer;

- (void)metalDisplayLink:(nonnull CAMetalDisplayLink *)link needsUpdate:(nonnull CAMetalDisplayLinkUpdate *)update;

@property (atomic) BOOL nightMode;

@property (atomic) BOOL fireOn;

// Set from the window delegate. A covered window throttles the display link to ~3 fps,
// so the stats line says when the numbers were taken with nothing on screen.
@property (atomic) BOOL windowVisible;

@end
