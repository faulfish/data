//
//  DetailNavigationController.m
//  iClass
//
//  Created by Richard Ting on 9/16/13.
//  Copyright (c) 2013 alan.tu. All rights reserved.
//

#import "DetailNavigationController.h"
#import "UIColor+HexString.h"

@interface DetailNavigationController ()
@property (weak, readwrite, nonatomic) id<CourseInfoTVCDelegate> courseInfoTVCDelegate;
@end

@implementation DetailNavigationController

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

    [self setupNavigationBarWithRole:self.role];
    [self setupTopViewControllerWithRole:self.role];
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (void)setupNavigationBarWithRole:(NSUInteger) role
{
    if (role == 0) {
        [self.navigationBar setTintColor:[UIColor colorWithHexString:@"#BA6DB3"]];
        [self.navigationBar setBackgroundImage:[UIImage imageNamed:@"bk_planner_titlebar_right.png"] forBarMetrics:UIBarMetricsDefault];
    } else {
        [self.navigationBar setTintColor:[UIColor colorWithHexString:@"#6FB3C1"]];
        [self.navigationBar setBackgroundImage:[UIImage imageNamed:@"bk_planner_titlebar_right_b.png"] forBarMetrics:UIBarMetricsDefault];
    }
}

- (void)setupTopViewControllerWithRole:(NSUInteger) role
{
    id controller = self.topViewController;
    if ([controller isKindOfClass:[CourseInfoTVC class]]) {
        self.courseInfoTVCDelegate = controller;

        if ([controller respondsToSelector:@selector(setRole:)]) {
            [controller setRole:role];
        }
    }
}

@end
