//
//  Seat.m
//  iClass
//
//  Created by Richard Ting on 9/17/13.
//  Copyright (c) 2013 alan.tu. All rights reserved.
//

#import "Seat.h"

@implementation Seat

+ (Seat *)emptySeat
{
    Seat *seat = [[Seat alloc] init];

    seat.attendanceStatus = NO;
    seat.attendanceNote = @"";
    seat.studentId = @"";
    seat.studentName = @"";

    return seat;
}

@end
