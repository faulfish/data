//
//  PlannerHeaderView.h
//  MyTestApp
//
//  Created by Richard Ting on 8/8/13.
//  Copyright (c) 2013 Richard Ting. All rights reserved.
//

#import <UIKit/UIKit.h>

#define PLANNER_HEADER_NAME_LABEL_HEIGHT 10.0f
#define PLANNER_HEADER_DATE_LABEL_HEIGHT 20.0f

@interface PlannerHeaderView : UIView

@property (strong, nonatomic) NSDate *pickedDate;

+ (NSArray *)weekdayTitleStrings;

- (void)setColumnCount:(NSUInteger)count
              startDay:(NSUInteger)startDay
    firstColumnIsEmpty:(BOOL)firstColumnIsEmpty;

@end
