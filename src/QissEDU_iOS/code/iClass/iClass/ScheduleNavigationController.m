//
//  ScheduleNavigationController.m
//  iClass
//
//  Created by Richard Ting on 10/9/13.
//  Copyright (c) 2013 alan.tu. All rights reserved.
//

#import "ScheduleNavigationController.h"
#import "ScheduleTVC.h"
#import "UIColor+HexString.h"

@interface ScheduleNavigationController ()
@property (weak, readwrite, nonatomic) id<ScheduleTVCDelegate> courseHistoryDelegate;
@end

@implementation ScheduleNavigationController

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
    [self setupChileViewControllersWithRole:self.role];
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
    [self setupNavigationBarWithRole:self.role];
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

- (void)setupChileViewControllersWithRole:(NSUInteger) role
{
    for (id controller in self.childViewControllers) {
        if ([controller isKindOfClass:[ScheduleTVC class]]) {
            self.courseHistoryDelegate = controller;
        }

        if ([controller respondsToSelector:@selector(setRole:)]) {
            [controller setRole:role];
        }
    }
}

@end
