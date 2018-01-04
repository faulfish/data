//
//  Seat.h
//  iClass
//
//  Created by Richard Ting on 9/17/13.
//  Copyright (c) 2013 alan.tu. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface Seat : NSObject

@property (nonatomic) BOOL attendanceStatus;
@property (strong, nonatomic) NSString *attendanceNote;
@property (strong, nonatomic) NSString *studentId;
@property (strong, nonatomic) NSString *studentName;

+ (Seat *)emptySeat;

@end
