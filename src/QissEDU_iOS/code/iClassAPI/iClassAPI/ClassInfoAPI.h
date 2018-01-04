//
//  ClassInfoAPI.h
//  iClassAPI
//
//  Created by admin on 13/8/14.
//  Copyright (c) 2013年 admin. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "HTTPUtils.h"

@interface ClassInfoAPI : NSObject<ASIHTTPRequestDelegate>
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

- (NSString *) getAllstudentByClass: (NSString *) className error: (NSError **)error;
- (NSString *) getClass: (NSString *) teacherID error: (NSError **) error;
- (NSString *) getClassByStudentID: (NSString *) studentID error: (NSError **) error;
- (NSString *) getClassOnly: (NSString *) teacherID error: (NSError **) error;
- (NSString *) getClassTable: (NSString *) userID error: (NSError **) error;

@end