//
//  SeatTable.h
//  iClass
//
//  Created by Richard Ting on 9/17/13.
//  Copyright (c) 2013 alan.tu. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "Seat.h"

@interface SeatTable : NSObject

#define QISSEDU_SEATINFO_SEAT_NUMBER @"seat"
#define QISSEDU_SEATINFO_ATTENDANCE_STATUS @"status"
#define QISSEDU_SEATINFO_ATTENDANCE_NOTE @"attendanceNote"
#define QISSEDU_SEATINFO_STUDENT_ID @"studentID"
#define QISSEDU_SEATINFO_STUDENT_NAME @"studentName"

- (id)initWithSeatCount:(NSUInteger)count;
- (void)updateSeats:(NSArray *)seats;
- (Seat *)seatAtIndex:(NSUInteger)index;

@end
