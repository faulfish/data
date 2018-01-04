//
//  MaterialPageViewController.m
//  iClass
//
//  Created by alan.tu on 9/3/13.
//  Copyright (c) 2013 alan.tu. All rights reserved.
//

#import "MaterialPageViewController.h"
#import "PaintingView.h"
#import "CoreGraphics/CGColor.h"

//CONSTANTS:

#define kBrightness             1.0
#define kSaturation             0.45

#define kPaletteHeight			30
#define kPaletteSize			5
#define kMinEraseInterval		0.5

// Padding for margins
#define kLeftMargin				10.0
#define kTopMargin				10.0
#define kRightMargin			10.0

@interface MaterialPageViewController ()
@property (weak, nonatomic) IBOutlet UIImageView *image;
@property (weak, nonatomic) IBOutlet PaintingView *paintView;

@end

@implementation MaterialPageViewController

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Custom initialization
    }
    return self;
}

- (void)viewDidLoad
{
    [super viewDidLoad];
	// Do any additional setup after loading the view.

    NSString *filepath = (NSString*)_dataObject;
    NSLog(@"%@",_dataObject);
    //UIImage *image = [UIImage imageNamed:filepath];
    UIImage *image = [[UIImage alloc] initWithContentsOfFile:filepath];
    _image.image = image;
    
    /*
    // Define a starting color
    CGColorRef color = [UIColor colorWithHue:(CGFloat)2.0 / (CGFloat)kPaletteSize
                                  saturation:kSaturation
                                  brightness:kBrightness
                                       alpha:1.0].CGColor;
    const CGFloat *components = CGColorGetComponents(color);
    
    [_paintView layoutSubviews];
	// Defer to the OpenGL view to set the brush color
	[_paintView setBrushColorWithRed:components[0] green:components[1] blue:components[2]];
     */
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

@end
