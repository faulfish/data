//
//  PlannerView.h
//  iClass
//
//  Created by Richard Ting on 8/27/13.
//  Copyright (c) 2013 alan.tu. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "CourseInfoTVC.h"

@interface PlannerView : UIView

#define PLANNER_VIEW_CORNER_RADIUS 8.0
#define PLANNER_VIEW_DATE_HEIGHT 48.0
#define PLANNER_VIEW_TITLE_HEIGHT 56.0
#define PLANNER_VIEW_NOON_HEIGHT 15.0

#define PLANNER_VIEW_COURSE_COLUMN 5
#define PLANNER_VIEW_COURSE_ROW 8

#define PLANNER_VIEW_GRID_LINE_WIDTH 1.0

@property (strong, nonatomic) NSDate *pickedDate;
@property (strong, nonatomic) UIColor *courseCellColor;
@property (strong, nonatomic) NSMutableArray *courses;
@property (nonatomic) NSUInteger role;
@property (strong, nonatomic) NSString *userID;
@property (strong, nonatomic) NSString *userName;
@property (weak, nonatomic) id<CourseInfoTVCDelegate> courseInfoDelegate;

- (void)drawCourseWithName:(NSString *)name weekday:(NSUInteger)weekday startHour:(NSUInteger)hour duration:(NSUInteger)duration;

@end

@protocol PlannerViewDelegate <NSObject>

- (void)plannerView:(PlannerView *)plannerView didSelectCourse:(NSString *)courseId;

@end
