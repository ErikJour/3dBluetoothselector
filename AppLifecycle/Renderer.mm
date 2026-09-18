#include "Renderer.h"
#import <QuartzCore/QuartzCore.h>
#import <Metal/Metal.h>
#import <CoreVideo/CoreVideo.h>
#include <simd/simd.h>
#include <pthread.h>

//===========================================================
//Renderer
//===========================================================
@implementation Renderer
{
    CAMetalLayer*                      _metalLayer;
    CAMetalDisplayLink*                _displayLink;
    id<MTLDevice>                      _device;
    id<MTLCommandQueue>                _commandQueue;
    id<MTLRenderPipelineState>         _pipelineState;
    CVMetalTextureCacheRef             _textureCache;
}

- (nonnull instancetype)initWithMetalLayer:(nonnull CAMetalLayer *)metalLayer
{
    self = [super init];
    if(self)
    {
        _metalLayer       = metalLayer;
        _device           = _metalLayer.device;
        _commandQueue     = [_device newCommandQueue];

        //String metal library
        NSString *libPath = [NSBundle.mainBundle.resourcePath
                             stringByAppendingPathComponent:@"shaders.metallib"];
        NSError *err = nil;
        id<MTLLibrary> lib = [_device newLibraryWithURL:[NSURL fileURLWithPath:libPath]
                                                  error:&err];
        NSAssert(lib, @"Failed to load metallib: %@", err);
        //Shader setup
        id<MTLFunction> vfn = [lib newFunctionWithName:@"vertexMain"];
        id<MTLFunction> ffn = [lib newFunctionWithName:@"fragmentMain"];
        NSAssert(vfn && ffn, @"Missing shader functions");
        //Render pipeline
        MTLRenderPipelineDescriptor *desc                    = [[MTLRenderPipelineDescriptor alloc] init];
        desc.vertexFunction                                  = vfn;
        desc.fragmentFunction                                = ffn;
        desc.colorAttachments[0].pixelFormat                 = _metalLayer.pixelFormat;
        desc.colorAttachments[0].blendingEnabled             = YES;
        desc.colorAttachments[0].sourceRGBBlendFactor        = MTLBlendFactorOne;
        desc.colorAttachments[0].destinationRGBBlendFactor   = MTLBlendFactorOneMinusSourceAlpha;
        desc.colorAttachments[0].sourceAlphaBlendFactor      = MTLBlendFactorOne;
        desc.colorAttachments[0].destinationAlphaBlendFactor = MTLBlendFactorOneMinusSourceAlpha;

        _pipelineState = [_device newRenderPipelineStateWithDescriptor:desc error:&err];
        NSAssert(_pipelineState, @"Pipeline creation failed: %@", err);
        //Object to convert video to textures
        CVReturn cacheResult = CVMetalTextureCacheCreate(kCFAllocatorDefault, nil, _device, nil, &_textureCache);
        NSAssert(cacheResult == kCVReturnSuccess, @"Unable to create video texture cache");


        _displayLink                         = [[CAMetalDisplayLink alloc] initWithMetalLayer:_metalLayer];
        _displayLink.delegate                = self;
        _displayLink.preferredFrameRateRange = CAFrameRateRangeMake(60, 120, 60);
    }

    return self;
}

- (void)dealloc
{
    [_displayLink invalidate];
    if (_textureCache) { CFRelease(_textureCache); }
}

//===========================================================
//Per-Frame Callback
//===========================================================
- (void)metalDisplayLink:(nonnull CAMetalDisplayLink *)displayLink
             needsUpdate:(nonnull CAMetalDisplayLinkUpdate *)frameUpdate
{
    @autoreleasepool
    {
        id<CAMetalDrawable> drawable     = frameUpdate.drawable;
        if (!drawable) { return; }

        CFTimeInterval callbackStart     = CACurrentMediaTime();
        CFTimeInterval presentTime       = frameUpdate.targetPresentationTimestamp;

        id<MTLCommandBuffer> cmd = [_commandQueue commandBuffer];

        MTLRenderPassDescriptor *pass        = [MTLRenderPassDescriptor renderPassDescriptor];
        pass.colorAttachments[0].texture     = drawable.texture;
        pass.colorAttachments[0].loadAction  = MTLLoadActionClear;
        pass.colorAttachments[0].storeAction = MTLStoreActionStore;
        pass.colorAttachments[0].clearColor  = MTLClearColorMake(0.0, 0.0, 0.0, 1.0);

        id<MTLRenderCommandEncoder> enc      = [cmd renderCommandEncoderWithDescriptor:pass];
        [enc setRenderPipelineState:_pipelineState];

        [cmd presentDrawable:drawable];
        [cmd commit];

        CVMetalTextureCacheFlush(_textureCache, 0);
    }
}

@end
