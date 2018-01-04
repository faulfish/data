//
//  Course.m
//  iClass
//
//  Created by Richard Ting on 9/5/13.
//  Copyright (c) 2013 alan.tu. All rights reserved.
//

#import "Course.h"
#import "NSDate+String.h"

@implementation Course

- (void)setStartDateWithString:(NSString *)date
{
    self.startDate = [NSDate dateWithString:date];
}

- (void)setEndDateWithString:(NSString *)date
{
    self.endDate = [NSDate dateWithString:date];
}

- (void)setStartTimeWithString:(NSString *)time
{
    self.startTime = [NSDate timeWithString:time];
}

- (void)setEndTimeWithString:(NSString *)time
{
    self.endTime = [NSDate timeWithString:time];
}

@end
