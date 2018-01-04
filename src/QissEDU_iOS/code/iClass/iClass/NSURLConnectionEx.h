//
//  NSURLConnectionEx.h
//  iClass
//
//  Created by alan.tu on 8/7/13.
//  Copyright (c) 2013 alan.tu. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface NSURLConnectionEx : NSURLConnection

+ (NSData*) sendSynchronousRequest:(NSURLRequest *)request returningResponse:(NSURLResponse *__autoreleasing *)response error:(NSError *__autoreleasing *)error;

+ (void) flush;

+ (void) save;

+ (void) load;

+ (void) unittest;

@end
