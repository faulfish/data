//
//  PlannerViewController.h
//  MyTestApp
//
//  Created by Richard Ting on 8/8/13.
//  Copyright (c) 2013 Richard Ting. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "CourseInfoTVC.h"

@interface PlannerViewController : UIViewController
@property (strong, nonatomic) NSDate *pickedDate;
@property (nonatomic) NSUInteger role;
@property (strong, nonatomic) NSString *userID;
@property (strong, nonatomic) NSString *userName;
@property (weak, nonatomic) id<CourseInfoTVCDelegate> courseInfoDelegate;
@end
