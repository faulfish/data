//
//  Planner.h
//  iClass
//
//  Created by Richard Ting on 9/5/13.
//  Copyright (c) 2013 alan.tu. All rights reserved.
//

#import <Foundation/Foundation.h>

#define QISSEDU_COURSE_ID @"courseID"
#define QISSEDU_COURSE_NAME @"courseName"
#define QISSEDU_CLASS_NAME @"className"
#define QISSEDU_COURSE_STARTDATE @"courseStartDate"
#define QISSEDU_COURSE_ENDDATE @"courseEndDate"
#define QISSEDU_COURSE_STARTTIME @"courseStartTime"
#define QISSEDU_COURSE_ENDTIME @"courseEndTime"
#define QISSEDU_COURSE_WEEKDAY @"courseWeekday"

@interface Planner : NSObject

@property (strong, nonatomic) NSMutableArray *courses;

- (void)setupWithCourses:(NSArray *)courses;

@end
