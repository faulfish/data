//
//  AttendanceInfoAPI.m
//  iClassAPI
//
//  Created by admin on 13/8/23.
//  Copyright (c) 2013年 admin. All rights reserved.
//

#import "AttendanceInfoAPI.h"

static NSString * const attendanceServletUrl = @"https://qissedufirst.appspot.com/testapp/attendance";

@implementation AttendanceInfoAPI

@synthesize utils = _utils;
@synthesize url;
@synthesize request;
@synthesize responseString;

- (HTTPUtils *)utils
{
    if (!_utils) _utils = [[HTTPUtils alloc] init];
    return _utils;
}

- (NSString *) getSeatingInfo: (NSString *) courseID
                         date: (NSString *) date
                        error: (NSError **) error
{
    url = [NSURL URLWithString:attendanceServletUrl];
    request = [ASIFormDataRequest requestWithURL:url];
    request = [self.utils fillHeader:request];
    [request setRequestMethod:@"POST"];
    [request addRequestHeader:@"API" value:@"getSeatingInfo"];
    [request setPostFormat: ASIMultipartFormDataPostFormat];
    [request setPostValue: courseID forKey:@"course_ID"];
    [request setPostValue: date forKey:@"date"];
    
    [request startSynchronous];
    
    NSData *data = request.responseData;
    responseString = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];
    
    if(error){
        *error = [request error];
    }
    
    [request clearDelegatesAndCancel];
    
    return responseString;
}

- (NSString *) createAttendanceData: (NSString *) courseID
                          studentID: (NSString *) studentID
                             status: (BOOL) status
                               note: (NSString *) note
                               date: (NSString *) date
                              error: (NSError **) error
{
    url = [NSURL URLWithString:attendanceServletUrl];
    
    request = [ASIFormDataRequest requestWithURL:url];
    request = [self.utils fillHeader:request];
    [request setRequestMethod:@"POST"];
    [request addRequestHeader:@"API" value:@"createAttendance"];
    [request setPostFormat: ASIMultipartFormDataPostFormat];
    [request setPostValue: courseID forKey:@"course_ID"];
    [request setPostValue: studentID forKey:@"student_ID"];
    
    if (status) {
        [request setPostValue: @"True" forKey:@"status"];
    } else {
        [request setPostValue: @"False" forKey:@"status"];
    }
    
    [request setPostValue: note forKey:@"note"];
    [request setPostValue: date forKey:@"date"];
    
    [request startSynchronous];
    
    NSString *result = [[request responseHeaders] objectForKey:@"postResponseHeader"];
    
    if (error){
        *error = [request error];
    }
    
    [request clearDelegatesAndCancel];
    
    return result;
}

- (NSString *) editAttendanceData: (NSString *) attendanceID
                           status: (BOOL) status
                             note: (NSString *) note
                            error: (NSError **) error
{
    url = [NSURL URLWithString:attendanceServletUrl];
    request = [ASIFormDataRequest requestWithURL:url];
    request = [self.utils fillHeader:request];
    [request setRequestMethod:@"POST"];
    [request addRequestHeader:@"API" value:@"editAttendance"];
    [request setPostFormat: ASIMultipartFormDataPostFormat];
    [request setPostValue: attendanceID forKey:@"attendanceID"];
    if (status) {
        [request setPostValue: @"True" forKey:@"status"];
    } else {
        [request setPostValue: @"False" forKey:@"status"];
    }
    [request setPostValue: note forKey:@"note"];
    
    [request startSynchronous];
    
    NSString *result = [[request responseHeaders] objectForKey:@"postResponseHeader"];
    
    if (error)
        *error = [request error];
    
    [request clearDelegatesAndCancel];
    
    return result;
}

- (NSString *) getAttendanceByStudent: (NSString *) courseID
                            studentID: (NSString *) studentID
                                 date: (NSString *) date
                                error: (NSError **) error
{
    url = [NSURL URLWithString:attendanceServletUrl];
    request = [ASIFormDataRequest requestWithURL:url];
    request = [self.utils fillHeader:request];
    [request setRequestMethod:@"POST"];
    [request addRequestHeader:@"API" value:@"getAttendance"];
    [request setPostFormat: ASIMultipartFormDataPostFormat];
    [request setPostValue: courseID forKey:@"course_ID"];
    [request setPostValue: studentID forKey:@"student_ID"];
    [request setPostValue: date forKey:@"date"];
    
    [request startSynchronous];
    
    NSData *data = request.responseData;
    responseString = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];
    
    if(error){
        *error = [request error];
    }
    
    [request clearDelegatesAndCancel];
    
    return responseString;
}

- (NSString *) getAttendanceHistory: (NSString *) date
                          className: (NSString *) className
                         courseName: (NSString *) courseName
                              error: (NSError **) error
{
    url = [NSURL URLWithString:attendanceServletUrl];
    request = [ASIFormDataRequest requestWithURL:url];
    request = [self.utils fillHeader:request];
    [request setRequestMethod:@"POST"];
    [request addRequestHeader:@"API" value:@"getAttendanceHistory"];
    [request setPostFormat: ASIMultipartFormDataPostFormat];
    [request setPostValue: date forKey:@"date"];
    [request setPostValue: className forKey:@"className"];
    [request setPostValue: courseName forKey:@"courseName"];
    
    [request startSynchronous];
    
    NSData *data = request.responseData;
    responseString = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];
    
    if(error){
        *error = [request error];
    }
    
    [request clearDelegatesAndCancel];
    
    return responseString;
}

@end
