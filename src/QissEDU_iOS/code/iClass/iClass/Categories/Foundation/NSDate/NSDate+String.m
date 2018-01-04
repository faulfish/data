//
//  NSDate+String.m
//  iClass
//
//  Created by Richard Ting on 9/5/13.
//  Copyright (c) 2013 alan.tu. All rights reserved.
//

#import "NSDate+String.h"
#import "NSDate+Components.h"

@implementation NSDate (String)

// Date string format: 20130905
+ (NSDate *)dateWithString:(NSString *)dateString
{
    NSDate *date;
    NSUInteger year, month, day;

    if ([dateString length] == 8) {
        year = [self dateComponentFrom:dateString start:0 length:4];
        month = [self dateComponentFrom:dateString start:4 length:2];
        day = [self dateComponentFrom:dateString start:6 length:2];

        date = [NSDate dateWithDay:day month:month year:year];
    } else {
        date = nil;
    }

    return date;
}

// Time string format: 08:10
+ (NSDate *)timeWithString:(NSString *)timeString
{
    NSDate *date;
    NSUInteger hour, minute;

    if ([timeString length] == 5) {
        hour = [self dateComponentFrom:timeString start:0 length:2];
        minute = [self dateComponentFrom:timeString start:3 length:2];

        date = [NSDate dateWithHour:hour minute:minute];
    } else {
        date = nil;
    }

    return date;
}

+ (NSUInteger)dateComponentFrom:(NSString *)string start:(NSUInteger)start length:(NSUInteger)length
{
    NSString *substring = [string substringWithRange: NSMakeRange(start, length)];
    NSInteger result;
    [[NSScanner scannerWithString:substring] scanInteger:&result];
    return result;
}

@end
