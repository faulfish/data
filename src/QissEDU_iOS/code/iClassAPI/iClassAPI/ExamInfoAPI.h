//
//  ExamInfoAPI.h
//  iClassAPI
//
//  Created by admin on 13/9/5.
//  Copyright (c) 2013年 admin. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "HTTPUtils.h"

@interface ExamInfoAPI : NSObject<ASIHTTPRequestDelegate>
{
    HTTPUtils *utils;
    NSURL *url;
    ASIFormDataRequest *request;
    NSString *responseString;
}

@property (strong, nonatomic) HTTPUtils *utils;
@property (strong, nonatomic) NSURL *url;
@property (strong, nonatomic) ASIFormDataRequest *request;
@property (strong, nonatomic) NSString *responseString;

- (NSString *) editExamComment: (NSString *) examResultID
                       comment: (NSString *) comment
                         error: (NSError **) error;

- (NSString *) getExamScoreList: (NSString *) examScheduleID
                          error: (NSError **) error;

- (NSString *) submitExam: (NSString *) studentID
           examScheduleID: (NSString *) scheduleID
            studentAnswer: (NSString *) answer
                    error: (NSError **) error;

- (NSString *) getStudentExamResult: (NSString *) studentID
                           courseID: (NSString *) courseID
                               date: (NSString *) date
                              error: (NSError **) error;

- (NSString *) getExamAnswer: (NSString *) examID
                       error: (NSError **) error;


@end

