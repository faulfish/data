//
//  MasterTabBarController.m
//  iClass
//
//  Created by Richard Ting on 9/18/13.
//  Copyright (c) 2013 alan.tu. All rights reserved.
//

#import "MasterTabBarController.h"

@interface MasterTabBarController ()

@end

@implementation MasterTabBarController

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

    [self setupTabItem];
}

- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
    [self hideTabBar];
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (void)setupTabItem
{
    for (id controller in self.viewControllers) {
        if ([controller respondsToSelector:@selector(setRole:)]) {
            [controller setRole:self.role];
        }
        if ([controller respondsToSelector:@selector(setUserID:)]) {
            [controller setUserID:self.userID];
        }
        if ([controller respondsToSelector:@selector(setUserName:)]) {
            [controller setUserName:self.userName];
        }
    }
}

// Richard, workaround solution to hide tab bar white space in iOS6 device
- (void)hideTabBar
{
    for (UIView *view in self.view.subviews) {
        if(![view isKindOfClass:[UITabBar class]]) {
            [view setFrame:CGRectMake(view.frame.origin.x, view.frame.origin.y, view.frame.size.width, 660)];
        }
    }
}

@end
