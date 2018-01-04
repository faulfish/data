//
//  ScheduleNavigationController.h
//  iClass
//
//  Created by Richard Ting on 10/9/13.
//  Copyright (c) 2013 alan.tu. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "ScheduleTVC.h"

@interface ScheduleNavigationController : UINavigationController

@property (nonatomic) NSUInteger role;
@property (weak, readonly, nonatomic) id<ScheduleTVCDelegate> courseHistoryDelegate;

@end
