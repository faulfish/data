//
//  StudentSeatCVC.m
//  iClass
//
//  Created by Richard Ting on 9/17/13.
//  Copyright (c) 2013 alan.tu. All rights reserved.
//

#import "StudentSeatCVC.h"
#import "SeatViewCell.h"
#import "SeatView.h"
#import "ClientAgent.h"

@interface StudentSeatCVC ()<UISplitViewControllerDelegate>

@end

@implementation StudentSeatCVC

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Custom initialization
    }
    return self;
}

- (void)viewDidLoad
{
    [super viewDidLoad];
	// Do any additional setup after loading the view.

    self.splitViewController.delegate = self;
    [self loadSeatTable];
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (void)loadSeatTable
{
    dispatch_queue_t loaderQ = dispatch_queue_create("seat info loader", NULL);
    dispatch_async(loaderQ, ^{
        NSArray *seatArray = [self loadSeatTableWithCourseId:self.courseID date:self.courseDate];
        self.seatCount = [self getMaximumSeatNumberWithSeats:seatArray];
        dispatch_async(dispatch_get_main_queue(), ^{
            self.seats = seatArray;
        });
    });
}

- (NSArray *)loadSeatTableWithCourseId:(NSString *)courseId date:(NSString *)dateString
{
    ClientAgent *api = [[ClientAgent alloc] init];
    NSError *error = [[NSError alloc] init];
    NSString *dataSeatTable = [api getSeatingInfo:courseId date:dateString error:&error];
    dataSeatTable = [NSString stringWithFormat:@"{\"SeatInfo\":%@}", dataSeatTable];
    NSData *jsonData = [dataSeatTable dataUsingEncoding:NSUTF8StringEncoding];
    NSDictionary *results = jsonData ? [NSJSONSerialization JSONObjectWithData:jsonData options:0 error:nil] : nil;
    return [results valueForKey:@"SeatInfo"];
}

- (NSUInteger)getMaximumSeatNumberWithSeats:(NSArray *)seats
{
    int maximum = 0;
    float seatNumber;

    for (int i = 0; i < [seats count]; i++) {
        seatNumber = [[seats[i][QISSEDU_SEATINFO_SEAT_NUMBER] description] floatValue];
        if (ceilf((seatNumber+1) / 5.0) * 5 > maximum) {
            maximum = ceilf((seatNumber+1) / 5.0) * 5;
        }
    }

    return maximum;
}

- (void)updateCell:(UICollectionViewCell *)cell usingSeat:(Seat *)seat
{
    if ([cell isKindOfClass:[SeatViewCell class]]) {
        SeatView *seatView = ((SeatViewCell *)cell).seatView;
        if ([seat isKindOfClass:[Seat class]]) {
            seatView.attendanceNote = seat.attendanceNote;
            seatView.studentId = seat.studentId;
            seatView.studentName = seat.studentName;
            seatView.alpha = seat.attendanceStatus ? 1.0 : 0.2;
        }
    }
}

#pragma mark - UISplitViewControllerDelegate
- (BOOL)splitViewController:(UISplitViewController *)svc shouldHideViewController:(UIViewController *)vc inOrientation:(UIInterfaceOrientation)orientation {
    return YES;
}

@end
