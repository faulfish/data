//
//  CourseTVC.h
//  iClass
//
//  Created by Richard Ting on 8/22/13.
//  Copyright (c) 2013 alan.tu. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "ScheduleTVC.h"

#define QISSEDU_CLASS_ID @"classID"
#define QISSEDU_CLASS_NAME @"className"
#define QISSEDU_COURSE_ID @"courseID"
#define QISSEDU_COURSE_NAME @"courseName"
#define QISSEDU_MATERIAL_ID @"materialID"
#define QISSEDU_MATERIAL_TITLE @"materialTitle"

@interface CourseTVC : UITableViewController

@property (nonatomic) NSUInteger role;
@property (strong, nonatomic) NSString *userID;
@property (strong, nonatomic) NSString *userName;
@property (strong, nonatomic) NSArray *courses;
@property (weak, nonatomic) id<ScheduleTVCDelegate> courseHistoryDelegate;

@end
