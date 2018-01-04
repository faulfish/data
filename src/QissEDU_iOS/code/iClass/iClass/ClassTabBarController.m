//
//  ClassTabBarController.m
//  iClass
//
//  Created by alan.tu on 8/20/13.
//  Copyright (c) 2013 alan.tu. All rights reserved.
//

#import "ClassTabBarController.h"

@interface ClassTabBarController ()

@end

@implementation ClassTabBarController

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
    
    //1. hide toolbar
    [self.tabBar setHidden:YES];
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

// Richard, workaround solution to hide tab bar white space in iOS6 device
- (void)hideTabBar
{
    CGRect rect = [self.view frame];
    for (UIView *view in self.view.subviews) {
        if(![view isKindOfClass:[UITabBar class]]) {
            [view setFrame:CGRectMake(view.frame.origin.x, view.frame.origin.y, view.frame.size.width, rect.size.height)];
        }
    }
}

- (void)setDsm:(DSShapeManager *)dsm {
    for (UIViewController *controller in self.childViewControllers) {
        if ([controller respondsToSelector:@selector(setDsm:)]) {
            [controller performSelector:@selector(setDsm:) withObject:dsm];
        }
    }
    _dsm = dsm;
}

- (void)setContentID:(NSString *)contentID
{
    for (UIViewController *controller in self.childViewControllers) {
        if ([controller respondsToSelector:@selector(setContentID:)]) {
            [controller performSelector:@selector(setContentID:) withObject:contentID];
        }
    }
    _contentID = contentID;
}

- (void)setMaterialID:(NSString *)materialID
{
    for (UIViewController *controller in self.childViewControllers) {
        if ([controller respondsToSelector:@selector(setMaterialID:)]) {
            [controller performSelector:@selector(setMaterialID:) withObject:materialID];
        }
    }
    _materialID = materialID;
}

- (void)setMaterialTitle:(NSString *)materialTitle
{
    for (UIViewController *controller in self.childViewControllers) {
        if ([controller respondsToSelector:@selector(setMaterialTitle:)]) {
            [controller performSelector:@selector(setMaterialTitle:) withObject:materialTitle];
        }
    }
    _materialTitle = materialTitle;
}

- (void)setCourseID:(NSString *)courseID
{
    for (UIViewController *controller in self.childViewControllers) {
        if ([controller respondsToSelector:@selector(setCourseID:)]) {
            [controller performSelector:@selector(setCourseID:) withObject:courseID];
        }
    }
    _courseID = courseID;
}

- (void)setCourseDate:(NSString *)courseDate
{
    for (UIViewController *controller in self.childViewControllers) {
        if ([controller respondsToSelector:@selector(setCourseDate:)]) {
            [controller performSelector:@selector(setCourseDate:) withObject:courseDate];
        }
    }
    _courseDate = courseDate;
}

- (void)setExamID:(NSString *)examID
{
    for (UIViewController *controller in self.childViewControllers) {
        if ([controller respondsToSelector:@selector(setExamID:)]) {
            [controller performSelector:@selector(setExamID:) withObject:examID];
        }
    }
    _examID = examID;
}

- (void)setExamTitle:(NSString *)examTitle
{
    for (UIViewController *controller in self.childViewControllers) {
        if ([controller respondsToSelector:@selector(setExamTitle:)]) {
            [controller performSelector:@selector(setExamTitle:) withObject:examTitle];
        }
    }
    _examTitle = examTitle;
}

- (void)setExamScheduleID:(NSString *)examScheduleID
{
    for (UIViewController *controller in self.childViewControllers) {
        if ([controller respondsToSelector:@selector(setExamScheduleID:)]) {
            [controller performSelector:@selector(setExamScheduleID:) withObject:examScheduleID];
        }
    }
    _examScheduleID = examScheduleID;
}

@end
