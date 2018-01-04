//
//  AttendanceInfoAPI.h
//  iClassAPI
//
//  Created by admin on 13/8/23.
//  Copyright (c) 2013年 admin. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "HTTPUtils.h"

@interface AttendanceInfoAPI : NSObject<ASIHTTPRequestDelegate>
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

- (NSString *) getSeatingInfo: (NSString *) courseID
                            date: (NSString *) date
                           error: (NSError **) error;
- (NSString *) createAttendanceData: (NSString *) courseID
                    studentID: (NSString *) studentID
                       status: (BOOL) status
                         note: (NSString *) note
                         date: (NSString *) date
                        error: (NSError **) error;
- (NSString *) editAttendanceData: (NSString *) attendanceID
                           status: (BOOL) status
                             note: (NSString *) note
                            error: (NSError **) error;
- (NSString *) getAttendanceByStudent: (NSString *) courseID
                            studentID: (NSString *) studentID
                                 date: (NSString *) date
                                error: (NSError **) error;

- (NSString *) getAttendanceHistory: (NSString *) date
                          className: (NSString *) className
                         courseName: (NSString *) courseName
                              error: (NSError **) error;

@end
