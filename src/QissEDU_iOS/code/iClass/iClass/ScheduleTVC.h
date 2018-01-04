//
//  ScheduleTVC.h
//  iClass
//
//  Created by Richard Ting on 10/9/13.
//  Copyright (c) 2013 alan.tu. All rights reserved.
//

#import <UIKit/UIKit.h>

#define QISSEDU_HISTORY_COURSE_ID @"courseID"
#define QISSEDU_HISTORY_COURSE_DATE @"date"
#define QISSEDU_HISTORY_COURSE_TIME @"time"
#define QISSEDU_HISTORY_COURSE_WEEKDAY @"weekday"
#define QISSEDU_HISTORY_COURSE_ATTENDANCE @"attendance"
#define QISSEDU_HISTORY_COURSE_ABSENT @"absent"
#define QISSEDU_HISTORY_COURSE_ANNOUNCEMENT @"announcement"
#define QISSEDU_HISTORY_MATERIAL_ID @"materialID"
#define QISSEDU_HISTORY_MATERIAL_TITLE @"materialTitle"
#define QISSEDU_HISTORY_EXAM_ID @"examID"
#define QISSEDU_HISTORY_EXAM_TITLE @"examTitle"

@protocol ScheduleTVCDelegate <NSObject>

- (void)setCourseName:(NSString *)courseName className:(NSString *)className;

@end

@interface ScheduleTVC : UITableViewController <ScheduleTVCDelegate>

@property (nonatomic) NSUInteger role;

@end
