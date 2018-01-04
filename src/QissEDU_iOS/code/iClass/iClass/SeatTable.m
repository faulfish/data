//
//  SeatTable.m
//  iClass
//
//  Created by Richard Ting on 9/17/13.
//  Copyright (c) 2013 alan.tu. All rights reserved.
//

#import "SeatTable.h"

@interface SeatTable()

@property (strong, nonatomic) NSMutableArray *seats;

@end

@implementation SeatTable

- (NSMutableArray *)seats
{
    if (!_seats) _seats = [[NSMutableArray alloc] init];
    return _seats;
}

- (id)initWithSeatCount:(NSUInteger)count
{
    self = [super init];

    if (self) {
        for (int i = 0; i < count; i++) {
            [self.seats addObject:[Seat emptySeat]];
        }
    }

    return self;
}

- (void)updateSeats:(NSArray *)seats
{
    int seatNumber;
    Seat *seat;

    for (int i = 0; i < [seats count]; i++) {

        seatNumber = [[seats[i][QISSEDU_SEATINFO_SEAT_NUMBER] description] intValue];

        if (seatNumber < [self.seats count]) {
            seat = (Seat *)self.seats[seatNumber];
            seat.attendanceStatus = [[seats[i][QISSEDU_SEATINFO_ATTENDANCE_STATUS] description] boolValue];
            seat.attendanceNote = [seats[i][QISSEDU_SEATINFO_ATTENDANCE_NOTE] description];
            seat.studentId = [seats[i][QISSEDU_SEATINFO_STUDENT_ID] description];
            seat.studentName = [seats[i][QISSEDU_SEATINFO_STUDENT_NAME] description];
        }
    }
}

- (Seat *)seatAtIndex:(NSUInteger)index
{
    return (index < [self.seats count]) ? self.seats[index] : nil;
}

@end
