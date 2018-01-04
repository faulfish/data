//
//  Planner.m
//  iClass
//
//  Created by Richard Ting on 9/5/13.
//  Copyright (c) 2013 alan.tu. All rights reserved.
//

#import "Planner.h"
#import "Course.h"
#import "NSDate+String.h"

@implementation Planner

- (NSMutableArray *)courses
{
    if (!_courses) _courses = [[NSMutableArray alloc] init];
    return _courses;
}

- (void)setupWithCourses:(NSArray *)courses
{
    if ([courses count] > 0) {
        Course *course;
        NSInteger integer;

        for (int i = 0; i < [courses count]; i++) {
            course = [[Course alloc] init];
            course.courseId = [courses[i][QISSEDU_COURSE_ID] description];
            [[NSScanner scannerWithString:[courses[i][QISSEDU_COURSE_WEEKDAY] description]] scanInteger:&integer];
            course.weekday = integer;

            course.courseName = [courses[i][QISSEDU_COURSE_NAME] description];
            course.className = [courses[i][QISSEDU_CLASS_NAME] description];
            course.startDate = [NSDate dateWithString:[courses[i][QISSEDU_COURSE_STARTDATE] description]];
            course.endDate = [NSDate dateWithString:[courses[i][QISSEDU_COURSE_ENDDATE] description]];
            course.startTime = [NSDate timeWithString:[courses[i][QISSEDU_COURSE_STARTTIME] description]];
            course.endTime = [NSDate timeWithString:[courses[i][QISSEDU_COURSE_ENDTIME] description]];

            [self.courses addObject:course];
        }
    }
}

@end
