//
//  AuthAPI.m
//  iClassAPI
//
//  Created by admin on 13/10/14.
//  Copyright (c) 2013年 admin. All rights reserved.
//

#import "AuthAPI.h"

static NSString * const authServletUrl = @"https://qissedufirst.appspot.com/testapp/auth";

@implementation AuthAPI

@synthesize utils = _utils;
@synthesize url;
@synthesize request;
@synthesize responseString;

- (HTTPUtils *)utils
{
    if (!_utils) _utils = [[HTTPUtils alloc] init];
    return _utils;
}

- (NSString *) loginByUserAccount: (NSString *) account
                            error: (NSError **)error
{
    url = [NSURL URLWithString:authServletUrl];
    request = [ASIFormDataRequest requestWithURL:url];
    request = [self.utils fillHeader:request];
    [request setRequestMethod:@"POST"];
    [request addRequestHeader:@"API" value:@"loginByUserAccount"];
    [request setPostFormat: ASIMultipartFormDataPostFormat];
    [request setPostValue: account forKey:@"account"];
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

