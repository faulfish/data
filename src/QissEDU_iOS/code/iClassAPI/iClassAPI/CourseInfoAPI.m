//
//  CourseInfoAPI.m
//  iClassAPI
//
//  Created by admin on 13/8/23.
//  Copyright (c) 2013年 admin. All rights reserved.
//

#import "CourseInfoAPI.h"

static NSString * const courseServletUrl = @"https://qissedufirst.appspot.com/testapp/course";

@implementation CourseInfoAPI

@synthesize utils = _utils;
@synthesize url;
@synthesize request;
@synthesize responseString;

- (HTTPUtils *)utils
{
    if (!_utils) _utils = [[HTTPUtils alloc] init];
    return _utils;
}

- (NSString *) getCourseAllInfo: (NSString *)classID error: (NSError **) error
{
    url = [NSURL URLWithString:courseServletUrl];
    
    request = [ASIFormDataRequest requestWithURL:url];
    request = [self.utils fillHeader:request];
    [request setRequestMethod:@"POST"];
    [request addRequestHeader:@"API" value:@"getCourseAllInfo"];
    [request setPostFormat: ASIMultipartFormDataPostFormat];
    [request setPostValue: classID forKey:@"Class_ID"];
    
    [request startSynchronous];
    /*
     BOOL success = ([request responseStatusCode] == 200);
     success = [request didUseCachedResponse];
     NSLog(@"success: %@",(success ? @"YES" : @"NO"));
     */
    
    NSData *data = request.responseData;
    responseString = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];
    
    if(error){
        *error = [request error];
    }
    
    [request clearDelegatesAndCancel];
    
    return responseString;
}

- (NSString *) createSemesterData: (NSString *) year
                     semesterType: (NSString *) type
                semesterStartTime: (NSString *) startTime
                  semesterEndTime: (NSString *) endTime
                            error: (NSError **) error
{
    url = [NSURL URLWithString:courseServletUrl];
    
    request = [ASIFormDataRequest requestWithURL:url];
    request = [self.utils fillHeader:request];
    [request setRequestMethod:@"POST"];
    [request addRequestHeader:@"API" value:@"createSemesterData"];
    [request setPostFormat: ASIMultipartFormDataPostFormat];
    [request setPostValue: year forKey:@"academicYear"];
    [request setPostValue: type forKey:@"semesterType"];
    [request setPostValue: startTime forKey:@"startDate"];
    [request setPostValue: endTime forKey:@"endDate"];
    
    [request startSynchronous];
    
    NSString *result = [[request responseHeaders] objectForKey:@"postResponseHeader"];
    
    if (error){
        *error = [request error];
    }
    
    [request clearDelegatesAndCancel];
    
    return result;
}


- (NSString *)getCourseInfo: (NSString *) courseID date: (NSString *) date error: (NSError **) error
{
    url = [NSURL URLWithString:courseServletUrl];
    
    request = [ASIFormDataRequest requestWithURL:url];
    request = [self.utils fillHeader:request];
    [request setRequestMethod:@"POST"];
    [request addRequestHeader:@"API" value:@"getCourseInfo"];
    [request setPostFormat: ASIMultipartFormDataPostFormat];
    [request setPostValue: courseID forKey:@"courseID"];
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

- (NSString *) createAnnouncement: (NSString *) announcement
                        className: (NSString *) className
                         courseID: (NSString *) courseID
                             date: (NSString *) date
                            error: (NSError **) error
{
    url = [NSURL URLWithString:courseServletUrl];
    
    request = [ASIFormDataRequest requestWithURL:url];
    request = [self.utils fillHeader:request];
    [request setRequestMethod:@"POST"];
    [request addRequestHeader:@"API" value:@"createAnnouncement"];
    [request setPostFormat: ASIMultipartFormDataPostFormat];
    [request setPostValue: announcement forKey:@"announcement"];
    [request setPostValue: className forKey:@"className"];
    [request setPostValue: courseID forKey:@"courseID"];
    [request setPostValue: date forKey:@"date"];
    
    [request startSynchronous];
    
    NSString *result = [[request responseHeaders] objectForKey:@"postResponseHeader"];
    
    if (error){
        *error = [request error];
    }
    
    [request clearDelegatesAndCancel];
    
    return result;
}

- (NSString *) getCourseHistory: (NSString *) date
                      className: (NSString *) className
                     courseName: (NSString *) courseName
                          error: (NSError **) error
{
    url = [NSURL URLWithString:courseServletUrl];
    
    request = [ASIFormDataRequest requestWithURL:url];
    request = [self.utils fillHeader:request];
    [request setRequestMethod:@"POST"];
    [request addRequestHeader:@"API" value:@"getCourseHistory"];
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
