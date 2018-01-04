//
//  ClassInfoAPI.m
//  implement iClassAPI of class info.
//
//  Created by admin on 13/8/14.
//  Copyright (c) 2013年 admin. All rights reserved.
//

#import "ClassInfoAPI.h"

static NSString * const classServletUrl = @"https://qissedufirst.appspot.com/testapp/class";

@implementation ClassInfoAPI

@synthesize utils = _utils;
@synthesize url;
@synthesize request;
@synthesize responseString;

- (HTTPUtils *)utils
{
    if (!_utils) _utils = [[HTTPUtils alloc] init];
    return _utils;
}

- (NSString *) getAllstudentByClass: (NSString *) className error: (NSError **) error
{
    url = [NSURL URLWithString:classServletUrl];
    request = [ASIFormDataRequest requestWithURL:url];
    request = [self.utils fillHeader:request];
    [request setRequestMethod:@"POST"];
    [request addRequestHeader:@"API" value:@"getAllstudentByClass"];
    [request setPostFormat: ASIMultipartFormDataPostFormat];
    [request setPostValue: className forKey:@"className"];
    [request startSynchronous];
    
    NSData *data = request.responseData;
    responseString = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];
    
    if(error){
        *error = [request error];
    }
    
    [request clearDelegatesAndCancel];
    
    return responseString;
}

- (NSString *) getClass: (NSString *) teacherID error: (NSError **) error
{
    url = [NSURL URLWithString:classServletUrl];
    request = [ASIFormDataRequest requestWithURL:url];
    request = [self.utils fillHeader:request];
    [request setRequestMethod:@"POST"];
    [request addRequestHeader:@"API" value:@"getClass"];
    [request setPostFormat: ASIMultipartFormDataPostFormat];
    [request setPostValue: teacherID forKey:@"teacher_id"];
    
    [request startSynchronous];
    
    NSData *data = request.responseData;
    responseString = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];
    
    if(error){
        *error = [request error];
    }
    
    [request clearDelegatesAndCancel];
    
    return responseString;
}

- (NSString *) getClassByStudentID: (NSString *) studentID error: (NSError **) error
{
    url = [NSURL URLWithString:classServletUrl];
    request = [ASIFormDataRequest requestWithURL:url];
    request = [self.utils fillHeader:request];
    [request setRequestMethod:@"POST"];
    [request addRequestHeader:@"API" value:@"getClassByStudentID"];
    [request setPostFormat: ASIMultipartFormDataPostFormat];
    [request setPostValue: studentID forKey:@"studentID"];
    
    [request startSynchronous];
    
    NSData *data = request.responseData;
    responseString = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];
    
    if(error){
        *error = [request error];
    }
    
    [request clearDelegatesAndCancel];
    
    return responseString;
}

- (NSString *) getClassOnly: (NSString *) teacherID error: (NSError **) error
{
    url = [NSURL URLWithString:classServletUrl];
    request = [ASIFormDataRequest requestWithURL:url];
    request = [self.utils fillHeader:request];
    [request setRequestMethod:@"POST"];
    [request addRequestHeader:@"API" value:@"getClassOnly"];
    [request setPostFormat: ASIMultipartFormDataPostFormat];
    [request setPostValue: teacherID forKey:@"teacher_id"];
    
    [request startSynchronous];
    
    NSData *data = request.responseData;
    responseString = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];
    
    if(error){
        *error = [request error];
    }
    
    [request clearDelegatesAndCancel];
    
    return responseString;
}


- (NSString *) getClassTable: (NSString *) userID error: (NSError **) error
{
    url = [NSURL URLWithString:classServletUrl];
    request = [ASIFormDataRequest requestWithURL:url];
    request = [self.utils fillHeader:request];
    [request setRequestMethod:@"POST"];
    [request addRequestHeader:@"API" value:@"getClassTable"];
    [request setPostFormat: ASIMultipartFormDataPostFormat];
    [request setPostValue: userID forKey:@"userID"];
    
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
