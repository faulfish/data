//
//  Course.h
//  iClass
//
//  Created by Richard Ting on 9/5/13.
//  Copyright (c) 2013 alan.tu. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface Course : NSObject

@property (strong, nonatomic) NSString *courseId;
@property (strong, nonatomic) NSString *courseName;
@property (strong, nonatomic) NSString *className;
@property (strong, nonatomic) NSDate *startDate;
@property (strong, nonatomic) NSDate *endDate;
@property (strong, nonatomic) NSDate *startTime;
@property (strong, nonatomic) NSDate *endTime;
@property (nonatomic) NSUInteger weekday;

- (void)setStartDateWithString:(NSString *)date;
- (void)setEndDateWithString:(NSString *)date;
- (void)setStartTimeWithString:(NSString *)time;
- (void)setEndTimeWithString:(NSString *)time;

@end
