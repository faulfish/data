//
//  SeatCVC.h
//  iClass
//
//  Created by Richard Ting on 9/17/13.
//  Copyright (c) 2013 alan.tu. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "SeatTable.h"

@interface SeatCVC : UICollectionViewController

#define MINMUM_SEAT_NUMBER 5

@property (strong, nonatomic) NSArray *seats;
@property (nonatomic) NSUInteger seatCount;

- (void)updateCell:(UICollectionViewCell *)cell usingSeat:(Seat *)seat; // abstract

@end
