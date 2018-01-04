//
//  HTTPUtils.m
//  iClassAPI
//
//  Created by admin on 13/8/14.
//  Copyright (c) 2013年 admin. All rights reserved.
//

#import "HTTPUtils.h"

@implementation HTTPUtils

- (ASIFormDataRequest *) fillHeader:(ASIFormDataRequest *)request
{
    NSDateFormatter *formatter;
    NSString        *timestamp;
    formatter = [[NSDateFormatter alloc] init];
    [formatter setDateFormat:@"yyyyMMdd"];
    timestamp = [formatter stringFromDate:[NSDate date]];
    
    [request addRequestHeader:@"qauth_consumer_key" value:qauth_consumer_key];
    [request addRequestHeader:@"qauth_nonce" value:qauth_nonce];
    [request addRequestHeader:@"qauth_signature" value:qauth_signature];
    [request addRequestHeader:@"qauth_signature_method" value:qauth_signature_method];
    [request addRequestHeader:@"qauth_timestamp" value:timestamp];
    [request addRequestHeader:@"qauth_token" value:qauth_token];
    [request addRequestHeader:@"qauth_token_secret" value:qauth_token_secret];
    [request addRequestHeader:@"qauth_version" value:qauth_version];
    [request addRequestHeader:@"content-type" value:content_type];
    [request addRequestHeader:@"Host" value:host];
    [request addRequestHeader:@"User-Agent" value:User_Agent];
    
    /*
     [[ASIDownloadCache sharedCache] setShouldRespectCacheControlHeaders:NO];
     [request setDownloadCache:[ASIDownloadCache sharedCache]];
     [request setCachePolicy:ASIAskServerIfModifiedCachePolicy|ASIFallbackToCacheIfLoadFailsCachePolicy];
     [request setCacheStoragePolicy:ASICachePermanentlyCacheStoragePolicy];
     [request setSecondsToCache: 60*60*24*30];
     */
    
    return request;
}

@end
