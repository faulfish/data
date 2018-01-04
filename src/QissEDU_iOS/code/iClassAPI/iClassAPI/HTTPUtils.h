//
//  HTTPUtils.h
//  iClassAPI
//
//  Created by admin on 13/8/14.
//  Copyright (c) 2013年 admin. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "ASIFormDataRequest.h"
#import "ASIHTTPRequest.h"
//#import "ASIDownloadCache.h"

static NSString * const host = @"qissedufirst.appspot.com";
static NSString * const qauth_consumer_key = @"QissEdu_qauth_consumer_key";
static NSString * const qauth_nonce = @"QissEdu_qauth_nonce";
static NSString * const qauth_signature = @"QissEdu_qauth_signature";
static NSString * const qauth_signature_method = @"QissEdu_qauth_signature_method";
static NSString * const qauth_token = @"start";
static NSString * const qauth_token_secret = @"QissEdu_qauth_token_secret";
static NSString * const qauth_version = @"2.0";
static NSString * const content_type = @"multipart/form-data";
static NSString * const User_Agent = @"qissCloud1.0/com.QissEdu/1.0/iPad/zh-TW";

@interface HTTPUtils : NSObject

- (ASIFormDataRequest *) fillHeader:(ASIFormDataRequest *)request;

@end
