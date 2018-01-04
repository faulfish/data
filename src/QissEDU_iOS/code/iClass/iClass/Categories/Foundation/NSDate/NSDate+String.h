//
//  NSDate+String.h
//  iClass
//
//  Created by Richard Ting on 9/5/13.
//  Copyright (c) 2013 alan.tu. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface NSDate (String)

+ (NSDate *)dateWithString:(NSString *)dateString;
+ (NSDate *)timeWithString:(NSString *)timeString;

@end
