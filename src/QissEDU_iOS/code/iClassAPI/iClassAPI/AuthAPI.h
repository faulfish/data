//
//  AuthAPI.h
//  iClassAPI
//
//  Created by admin on 13/10/14.
//  Copyright (c) 2013年 admin. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "HTTPUtils.h"

@interface AuthAPI : NSObject<ASIHTTPRequestDelegate>
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

- (NSString *) loginByUserAccount: (NSString *) account
                            error: (NSError **)error;

@end