//
//  MaterialInfoAPI.m
//  iClassAPI
//
//  Created by admin on 13/8/19.
//  Copyright (c) 2013年 admin. All rights reserved.
//

#import "MaterialInfoAPI.h"

static NSString * const materialServletUrl = @"https://qissedufirst.appspot.com/testapp/tm";

@implementation MaterialInfoAPI

@synthesize utils = _utils;
@synthesize url;
@synthesize request;
@synthesize responseString;

- (HTTPUtils *)utils
{
    if (!_utils) _utils = [[HTTPUtils alloc] init];
    return _utils;
}

- (NSString *) getAllMaterialToClassByTeacherID: (NSString *)teacherID error: (NSError **) error
{
    url = [NSURL URLWithString:materialServletUrl];
    request = [ASIFormDataRequest requestWithURL:url];
    request = [self.utils fillHeader:request];
    [request setRequestMethod:@"POST"];
    [request addRequestHeader:@"API" value:@"getAllMaterialToClassByTeacherID"];
    [request setPostFormat: ASIMultipartFormDataPostFormat];
    [request setPostValue: teacherID forKey:@"teacherID"];
    
    [request startSynchronous];
    
    NSData *data = request.responseData;
    responseString = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];
    
    if(error){
        *error = [request error];
    }
    
    [request clearDelegatesAndCancel];
    
    return responseString;
}

- (NSString *) getAllMaterialToClassByTeacherIDAndDateTime: (NSString *)teacherID
                                                      date:(NSString *)dateTime
                                                     error: (NSError **) error
{
    url = [NSURL URLWithString:materialServletUrl];
    request = [ASIFormDataRequest requestWithURL:url];
    request = [self.utils fillHeader:request];
    [request setRequestMethod:@"POST"];
    [request addRequestHeader:@"API" value:@"getAllMaterialToClassByTeacherIDAndDateTime"];
    [request setPostFormat: ASIMultipartFormDataPostFormat];
    [request setPostValue: teacherID forKey:@"teacherID"];
    [request setPostValue: dateTime forKey:@"dateTime"];
    
    [request startSynchronous];
    
    NSData *data = request.responseData;
    responseString = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];
    
    if(error){
        *error = [request error];
    }
    
    [request clearDelegatesAndCancel];
    
    return responseString;
}

- (NSString *) getAllMaterialToClass: (NSString *)teacherID
                          materialID:(NSString *)materialID
                           className: (NSString *) className
                               error: (NSError **) error
{
    url = [NSURL URLWithString:materialServletUrl];
    request = [ASIFormDataRequest requestWithURL:url];
    request = [self.utils fillHeader:request];
    [request setRequestMethod:@"POST"];
    [request addRequestHeader:@"API" value:@"getAllMaterialToClass"];
    [request setPostFormat: ASIMultipartFormDataPostFormat];
    [request setPostValue: teacherID forKey:@"teacherID"];
    [request setPostValue: materialID forKey:@"materialID"];
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

- (NSString *) getAllMaterialToClassByMaterialID: (NSString *) materialID error: (NSError **) error
{
    url = [NSURL URLWithString:materialServletUrl];
    request = [ASIFormDataRequest requestWithURL:url];
    request = [self.utils fillHeader:request];
    [request setRequestMethod:@"POST"];
    [request addRequestHeader:@"API" value:@"getAllMaterialToClassByMaterialID"];
    [request setPostFormat: ASIMultipartFormDataPostFormat];
    [request setPostValue: materialID forKey:@"materialID"];
    
    [request startSynchronous];
    
    NSData *data = request.responseData;
    responseString = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];
    
    if(error){
        *error = [request error];
    }
    
    [request clearDelegatesAndCancel];
    
    return responseString;
}

- (NSString *) getAllMaterialToClassByClassName: (NSString *) className
                                           date:(NSString *)dateTime
                                          error: (NSError **) error
{
    url = [NSURL URLWithString:materialServletUrl];
    request = [ASIFormDataRequest requestWithURL:url];
    request = [self.utils fillHeader:request];
    [request setRequestMethod:@"POST"];
    [request addRequestHeader:@"API" value:@"getAllMaterialToClassByClassName"];
    [request setPostFormat: ASIMultipartFormDataPostFormat];
    [request setPostValue: className forKey:@"className"];
    [request setPostValue: dateTime forKey:@"dateTime"];
    
    [request startSynchronous];
    
    NSData *data = request.responseData;
    responseString = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];
    
    if(error){
        *error = [request error];
    }
    
    [request clearDelegatesAndCancel];
    
    return responseString;
}


- (NSString *) getAllCourseToTeachingMaterialByDateAndClass: (NSString *) className
                                                       date:(NSString *)date
                                                      error: (NSError **) error
{
    url = [NSURL URLWithString:materialServletUrl];
    request = [ASIFormDataRequest requestWithURL:url];
    request = [self.utils fillHeader:request];
    [request setRequestMethod:@"POST"];
    [request addRequestHeader:@"API" value:@"getAllCourseToTeachingMaterialByDateAndClass"];
    [request setPostFormat: ASIMultipartFormDataPostFormat];
    [request setPostValue: date forKey:@"dateTime"];
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

- (NSString *) getCourseSchedule: (NSString *)userID
                           error: (NSError **) error
{
    url = [NSURL URLWithString:materialServletUrl];
    request = [ASIFormDataRequest requestWithURL:url];
    request = [self.utils fillHeader:request];
    [request setRequestMethod:@"POST"];
    [request addRequestHeader:@"API" value:@"getCourseSchedule"];
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

- (NSString *) getCourseMaterial: (NSString *) courseID
                            date: (NSString *) date
                           error: (NSError **) error
{
    url = [NSURL URLWithString:materialServletUrl];
    request = [ASIFormDataRequest requestWithURL:url];
    request = [self.utils fillHeader:request];
    [request setRequestMethod:@"POST"];
    [request addRequestHeader:@"API" value:@"getCourseMaterial"];
    [request setPostFormat: ASIMultipartFormDataPostFormat];
    [request setPostValue: courseID forKey:@"courseID"];
    [request setPostValue: date forKey:@"date"];
    
    [request startSynchronous];
    
    NSData *data = request.responseData;
    responseString = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];
    
    NSLog(@"responseString = %@", responseString);
    
    if(error){
        *error = [request error];
    }
    
    [request clearDelegatesAndCancel];
    
    return responseString;
}

@end
