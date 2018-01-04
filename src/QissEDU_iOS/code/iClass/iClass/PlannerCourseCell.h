//
//  PlannerCourseCell.h
//  iClass
//
//  Created by Richard Ting on 9/24/13.
//  Copyright (c) 2013 alan.tu. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface PlannerCourseCell : UILabel

@property (strong, nonatomic)NSString *courseId;
@property (strong, nonatomic)NSString *courseName;
@property (strong, nonatomic)NSString *className;
@property (strong, nonatomic)NSString *period;

@end
