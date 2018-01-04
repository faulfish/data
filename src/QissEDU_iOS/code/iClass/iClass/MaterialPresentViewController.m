//
//  MaterialPresentViewController.m
//  iClass
//
//  Created by alan.tu on 8/21/13.
//  Copyright (c) 2013 alan.tu. All rights reserved.
//

#import "MaterialPresentViewController.h"
#import "MaterialPageViewController.h"
#import "MaterialPageDatasource.h"
#import "ClassTabBarController.h"
#import "ClassViewController.h"
#import "QDiscover/Shape_UDP_Sender.h"
#import "QDiscover/DSShapeManager.h"
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

@interface MaterialPresentViewController () {
    //
}
@property (weak, nonatomic) IBOutlet PaintingView *paintView;

@property (strong, nonatomic) MaterialPageDatasource *modelController;

@end

@implementation MaterialPresentViewController

@synthesize modelController = _modelController;

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
    self.splitViewController.delegate = self;
    //[self.view setNeedsLayout];

    // Do any additional setup after loading the view, typically from a nib.
    // Configure the page view controller and add it as a child view controller.
    self.pageViewController = [[UIPageViewController alloc] initWithTransitionStyle:UIPageViewControllerTransitionStyleScroll navigationOrientation:UIPageViewControllerNavigationOrientationHorizontal options:nil];
    self.pageViewController.delegate = self;

    [self showContentWithContentID:self.materialID];

    if (_dsm != nil) {
        [_dsm registerDelegate:self];
    }
    
    //by default, go to zero page
    UIViewController *controller = [_modelController viewControllerAtIndex:0 storyboard:self.storyboard];
    [self.pageViewController setViewControllers:@[controller]
                                      direction:UIPageViewControllerNavigationDirectionForward
                                       animated:YES
                                     completion:nil];
    
    // Define a starting color
    CGColorRef color = [UIColor colorWithHue:(CGFloat)2.0 / (CGFloat)kPaletteSize
                                  saturation:kSaturation
                                  brightness:kBrightness
                                       alpha:1.0].CGColor;
    const CGFloat *components = CGColorGetComponents(color);
    
    [_paintView layoutSubviews];
	// Defer to the OpenGL view to set the brush color
	[_paintView setBrushColorWithRed:components[0] green:components[1] blue:components[2]];
    _paintView.hidden = YES;
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (BOOL)splitViewController:(UISplitViewController *)svc shouldHideViewController:(UIViewController *)vc inOrientation:(UIInterfaceOrientation)orientation {
    return YES;
}

- (IBAction)unwindToThumbnail:(UIStoryboardSegue *)segue {
    //
}

- (MaterialPageDatasource *)modelController
{
    // Return the model controller object, creating it if necessary.
    // In more complex implementations, the model controller may be passed to the view controller.
    if (!_modelController) {
        _modelController = [[MaterialPageDatasource alloc] init];
    }
    return _modelController;
}

#pragma mark - UIPageViewController delegate methods

 - (void)pageViewController:(UIPageViewController *)pageViewController didFinishAnimating:(BOOL)finished previousViewControllers:(NSArray *)previousViewControllers transitionCompleted:(BOOL)completed {

     NSLog(@"page changed");
}

- (UIPageViewControllerSpineLocation)pageViewController:(UIPageViewController *)pageViewController spineLocationForInterfaceOrientation:(UIInterfaceOrientation)orientation
{
    if (UIInterfaceOrientationIsPortrait(orientation)) {
        // In portrait orientation: Set the spine position to "min" and the page view controller's view controllers array to contain just one view controller. Setting the spine position to 'UIPageViewControllerSpineLocationMid' in landscape orientation sets the doubleSided property to YES, so set it to NO here.
        UIViewController *currentViewController = self.pageViewController.viewControllers[0];
        NSArray *viewControllers = @[currentViewController];
        [self.pageViewController setViewControllers:viewControllers direction:UIPageViewControllerNavigationDirectionForward animated:YES completion:NULL];
        
        self.pageViewController.doubleSided = NO;
        return UIPageViewControllerSpineLocationMin;
    }
    
    // In landscape orientation: Set set the spine location to "mid" and the page view controller's view controllers array to contain two view controllers. If the current page is even, set it to contain the current and next view controllers; if it is odd, set the array to contain the previous and current view controllers.
    MaterialPageViewController *currentViewController = self.pageViewController.viewControllers[0];
    NSArray *viewControllers = nil;
    
    NSUInteger indexOfCurrentViewController = [self.modelController indexOfViewController:currentViewController];
    if (indexOfCurrentViewController == 0 || indexOfCurrentViewController % 2 == 0) {
        UIViewController *nextViewController = [self.modelController pageViewController:self.pageViewController viewControllerAfterViewController:currentViewController];
        viewControllers = @[currentViewController, nextViewController];
    } else {
        UIViewController *previousViewController = [self.modelController pageViewController:self.pageViewController viewControllerBeforeViewController:currentViewController];
        viewControllers = @[previousViewController, currentViewController];
    }
    [self.pageViewController setViewControllers:viewControllers direction:UIPageViewControllerNavigationDirectionForward animated:YES completion:NULL];
    
    
    return UIPageViewControllerSpineLocationMid;
}

- (void) setDsm:(DSShapeManager *)dsm {
    _dsm = dsm;
    if ([self modelController] != nil) {
        [_modelController setDsm:dsm];
    }
}

- (IBAction)doFree:(UIButton *)sender {
    [[self.pageViewController.view.subviews objectAtIndex:0] setScrollEnabled:FALSE];
    [_paintView erase];
    _paintView.hidden = NO;
}

- (IBAction)doErase:(UIButton *)sender {
    [[self.pageViewController.view.subviews objectAtIndex:0] setScrollEnabled:FALSE];
    [_paintView erase];
}

- (IBAction)doColor:(UIButton *)sender {
    [[self.pageViewController.view.subviews objectAtIndex:0] setScrollEnabled:FALSE];
}

- (IBAction)doLine:(UIButton *)sender {
    [[self.pageViewController.view.subviews objectAtIndex:0] setScrollEnabled:FALSE];
}

- (IBAction)doUndo:(UIButton *)sender {
    [[self.pageViewController.view.subviews objectAtIndex:0] setScrollEnabled:TRUE];
    _paintView.hidden = YES;
}

- (IBAction)toThumbnail:(UIButton *)sender {
    [self.materialSplitViewDelegate toThumbnail];
}

- (IBAction)hideMasterView:(UIButton *)sender {
    sender.selected = !sender.selected;
    [self.materialSplitViewDelegate setMasterViewHidden:sender.selected];
}

- (void)showContentWithContentID:(NSString *)contentID
{
    self.materialID = contentID;
    self.modelController = nil;
    self.modelController.materialID = contentID;
    MaterialPageViewController *startingViewController = [self.modelController viewControllerAtIndex:0 storyboard:self.storyboard];
    if (startingViewController) {
        NSArray *viewControllers = @[startingViewController];
        [self.pageViewController setViewControllers:viewControllers direction:UIPageViewControllerNavigationDirectionForward animated:NO completion:NULL];

        self.pageViewController.dataSource = self.modelController;

        // Set the page view controller's bounds using an inset rect so that self's view is visible around the edges of the pages.
        CGRect pageViewRect = self.presentView.bounds;
        self.pageViewController.view.frame = pageViewRect;
        startingViewController.view.frame = pageViewRect;

        [self addChildViewController:self.pageViewController];
        [self.presentView addSubview:self.pageViewController.view];
        [self.pageViewController didMoveToParentViewController:self];

        [startingViewController.view setNeedsDisplay];
        [self.pageViewController.view setNeedsLayout];

        [self.pageViewController didMoveToParentViewController:self];

        // Add the page view controller's gesture recognizers to the book view controller's view so that the gestures are started more easily.
        self.view.gestureRecognizers = self.pageViewController.gestureRecognizers;
    }
}

//shapedelegate
- (void) eventChanegePage:(NSInteger)pageID {
    NSLog(@"get the changed event %d", pageID);
}

@end
