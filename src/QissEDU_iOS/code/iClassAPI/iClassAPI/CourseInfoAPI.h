//
//  CourseInfoAPI.h
//  iClassAPI
//
//  Created by admin on 13/8/23.
//  Copyright (c) 2013年 admin. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "HTTPUtils.h"

@interface CourseInfoAPI : NSObject<ASIHTTPRequestDelegate>
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

- (NSString *) getCourseAllInfo: (NSString *)classID error: (NSError **) error;
- (NSString *) createSemesterData: (NSString *) year
                     semesterType: (NSString *) type
                semesterStartTime: (NSString *) startTime
                  semesterEndTime: (NSString *) endTime
                            error: (NSError **) error;
- (NSString *) getCourseInfo: (NSString *) courseID
                        date: (NSString *) date error: (NSError **) error;
- (NSString *) createAnnouncement: (NSString *) announcement
                        className: (NSString *) className
                         courseID: (NSString *) courseID
                             date: (NSString *) date
                            error: (NSError **) error;
- (NSString *) getCourseHistory: (NSString *) date
                      className: (NSString *) className
                     courseName: (NSString *) courseName
                          error: (NSError **) error;
@end