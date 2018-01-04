//
//  DetailNavigationController.h
//  iClass
//
//  Created by Richard Ting on 9/16/13.
//  Copyright (c) 2013 alan.tu. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "CourseInfoTVC.h"

@interface DetailNavigationController : UINavigationController

@property (weak, readonly, nonatomic) id<CourseInfoTVCDelegate> courseInfoTVCDelegate;
@property (nonatomic) NSUInteger role;

@end
