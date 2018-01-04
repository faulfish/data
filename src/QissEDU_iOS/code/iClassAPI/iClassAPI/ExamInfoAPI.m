//
//  ExamInfoAPI.m
//  iClassAPI
//
//  Created by admin on 13/9/5.
//  Copyright (c) 2013年 admin. All rights reserved.
//

#import "ExamInfoAPI.h"

static NSString * const examServletUrl = @"https://qissedufirst.appspot.com/testapp/exam";

@implementation ExamInfoAPI

@synthesize utils = _utils;
@synthesize url;
@synthesize request;
@synthesize responseString;

- (HTTPUtils *)utils
{
    if (!_utils) _utils = [[HTTPUtils alloc] init];
    return _utils;
}

- (NSString *) editExamComment: (NSString *) examResultID
                       comment: (NSString *) comment
                         error: (NSError **) error
{
    url = [NSURL URLWithString:examServletUrl];
    
    request = [ASIFormDataRequest requestWithURL:url];
    request = [self.utils fillHeader:request];
    [request setRequestMethod:@"POST"];
    [request addRequestHeader:@"API" value:@"createExamComment"];
    [request setPostFormat: ASIMultipartFormDataPostFormat];
    [request setPostValue: examResultID forKey:@"examResultID"];
    [request setPostValue: comment forKey:@"comment"];
    
    [request startSynchronous];
    
    NSString *result = [[request responseHeaders] objectForKey:@"postResponseHeader"];
    
    if (error){
        *error = [request error];
    }
    
    [request clearDelegatesAndCancel];
    
    return result;
}

- (NSString *) getExamScoreList: (NSString *) examScheduleID
                          error: (NSError **) error
{
    url = [NSURL URLWithString:examServletUrl];
    request = [ASIFormDataRequest requestWithURL:url];
    request = [self.utils fillHeader:request];
    [request setRequestMethod:@"POST"];
    [request addRequestHeader:@"API" value:@"getExamScoreList"];
    [request setPostFormat: ASIMultipartFormDataPostFormat];
    [request setPostValue: examScheduleID forKey:@"examScheduleID"];
    
    [request startSynchronous];
    
    NSData *data = request.responseData;
    responseString = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];
    
    if(error){
        *error = [request error];
    }
    
    [request clearDelegatesAndCancel];
    
    return responseString;
}

- (NSString *) submitExam: (NSString *) studentID
           examScheduleID: (NSString *) scheduleID
            studentAnswer: (NSString *) answer
                    error: (NSError **) error
{
    url = [NSURL URLWithString:examServletUrl];
    
    request = [ASIFormDataRequest requestWithURL:url];
    request = [self.utils fillHeader:request];
    [request setRequestMethod:@"POST"];
    [request addRequestHeader:@"API" value:@"submitExam"];
    [request setPostFormat: ASIMultipartFormDataPostFormat];
    [request setPostValue: studentID forKey:@"studentID"];
    [request setPostValue: scheduleID forKey:@"examScheduleID"];
    [request setPostValue: answer forKey:@"myAnswer"];
    
    [request startSynchronous];
    
    NSString *result = [[request responseHeaders] objectForKey:@"postResponseHeader"];
    
    if (error){
        *error = [request error];
    }
    
    [request clearDelegatesAndCancel];
    
    return result;
}

- (NSString *) getStudentExamResult: (NSString *) studentID
                           courseID: (NSString *) courseID
                               date: (NSString *) date
                              error: (NSError **) error
{
    url = [NSURL URLWithString:examServletUrl];
    request = [ASIFormDataRequest requestWithURL:url];
    request = [self.utils fillHeader:request];
    [request setRequestMethod:@"POST"];
    [request addRequestHeader:@"API" value:@"getStudentExamResult"];
    [request setPostFormat: ASIMultipartFormDataPostFormat];
    [request setPostValue: studentID forKey:@"studentID"];
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

- (NSString *) getExamAnswer: (NSString *) examID
                       error: (NSError **) error
{
    url = [NSURL URLWithString:examServletUrl];
    request = [ASIFormDataRequest requestWithURL:url];
    request = [self.utils fillHeader:request];
    [request setRequestMethod:@"POST"];
    [request addRequestHeader:@"API" value:@"getExamAnswer"];
    [request setPostFormat: ASIMultipartFormDataPostFormat];
    [request setPostValue: examID forKey:@"examID"];
    
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
