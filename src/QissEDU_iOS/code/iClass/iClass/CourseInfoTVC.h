//
//  CourseInfoTVC.h
//  iClass
//
//  Created by Richard Ting on 9/13/13.
//  Copyright (c) 2013 alan.tu. All rights reserved.
//

#import <UIKit/UIKit.h>

@protocol CourseInfoTVCDelegate <NSObject>

- (void)setCourseId:(NSString *)courseId courseName:(NSString *)courseName className:(NSString *)className date:(NSDate *)date period:(NSString *)period;

@end

@interface CourseInfoTVC : UITableViewController <CourseInfoTVCDelegate>

#define QISSEDU_COURSEINFO_ATTENDANCE @"Attendance"
#define QISSEDU_COURSEINFO_ABSENT @"Absent"
#define QISSEDU_COURSEINFO_ANNOUNCEMENT @"Announcement"

#define QISSEDU_COURSEINFO_EXAM_ID @"examID"
#define QISSEDU_COURSEINFO_EXAM_TITLE @"examTitle"
#define QISSEDU_COURSEINFO_EXAM_URL @"examURL"
#define QISSEDU_COURSEINFO_EXAM_SCHEDULE_ID @"examScheduleID"

#define QISSEDU_COURSEINFO_MATERIAL_ID @"materialID"
#define QISSEDU_COURSEINFO_MATERIAL_TITLE @"materialTitle"
#define QISSEDU_COURSEINFO_MATERIAL_URL @"materialURL"

@property (strong, nonatomic) NSString *courseId;
@property (strong, nonatomic) NSDate *courseDate;
@property (strong, nonatomic) NSString *coursePeriod;
@property (strong, nonatomic) NSString *courseName;
@property (strong, nonatomic) NSString *className;

@property (nonatomic) NSUInteger role;

@end
