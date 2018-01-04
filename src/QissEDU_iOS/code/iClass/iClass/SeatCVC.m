//
//  SeatCVC.m
//  iClass
//
//  Created by Richard Ting on 9/17/13.
//  Copyright (c) 2013 alan.tu. All rights reserved.
//

#import "SeatCVC.h"
#import "SeatViewCell.h"
#import "StudentInfoViewController.h"

@interface SeatCVC () <UICollectionViewDataSource, UIPopoverControllerDelegate>
@property (strong, nonatomic) IBOutlet UICollectionView *seatCollectionView;
@property (strong, nonatomic) SeatTable *table;
@property (strong, nonatomic) UIPopoverController *popoverStudentInfo;
@end

@implementation SeatCVC

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

    UIImageView *backgroundView = [[UIImageView alloc] initWithImage:[UIImage imageNamed:@"bk_seating_chart.png"]];
    backgroundView.contentMode = UIViewContentModeTopLeft;
    [self.collectionView setBackgroundView:backgroundView];
    self.popoverStudentInfo.delegate = self;
    self.popoverStudentInfo.popoverContentSize = CGSizeMake(300, 300);
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (void)setSeats:(NSArray *)seats
{
    // seatCount should be set before seatTable
    [self.table updateSeats:seats];
    [self.collectionView reloadData];
}

- (void)setSeatCount:(NSUInteger)seatCount
{
    _seatCount = seatCount;
    self.table = [[SeatTable alloc] initWithSeatCount:seatCount];
}

- (void)updateCell:(UICollectionViewCell *)cell usingSeat:(Seat *)seat
{
    // abstract
}

- (UIPopoverController *)popoverStudentInfo
{
    if (!_popoverStudentInfo) {
        UIViewController *contentViewController = [[UIViewController alloc] init];
        UIView *contentView = [[UIView alloc] initWithFrame:CGRectMake(0, 0, 300, 300)];
        [contentView setBackgroundColor:[UIColor whiteColor]];
        contentViewController.view = contentView;
        _popoverStudentInfo = [[UIPopoverController alloc] initWithContentViewController:contentViewController];
    }
    return _popoverStudentInfo;
}

- (IBAction)showPopover:(UITapGestureRecognizer *)gesture {
    CGPoint tapLocation = [gesture locationInView:self.seatCollectionView];
    NSIndexPath *indexPath = [self.seatCollectionView indexPathForItemAtPoint:tapLocation];
    SeatViewCell *tapCell = (SeatViewCell *)[self.seatCollectionView cellForItemAtIndexPath:indexPath];
    UIStoryboard *storyboard = [UIStoryboard storyboardWithName:@"Class" bundle:nil];
    StudentInfoViewController *contentViewController = (StudentInfoViewController *)[storyboard instantiateViewControllerWithIdentifier:@"StudentInfo"];
    NSLog(@"alpha of tapCell: %f", tapCell.seatView.alpha);
    contentViewController.attendanceStatus = (tapCell.seatView.alpha == 1.0) ? YES : NO;
    contentViewController.attendanceNote = tapCell.seatView.attendanceNote;
    contentViewController.studentId = tapCell.seatView.studentId;
    contentViewController.studentName = tapCell.seatView.studentName;

    self.popoverStudentInfo = [[UIPopoverController alloc] initWithContentViewController:contentViewController];
    self.popoverStudentInfo.delegate = self;
    [self.popoverStudentInfo presentPopoverFromRect:tapCell.frame inView:self.seatCollectionView permittedArrowDirections:UIPopoverArrowDirectionAny animated:YES];
}

#pragma mark - UICollectionViewDataSource
- (NSInteger)numberOfSectionsInCollectionView:(UICollectionView *)collectionView
{
    return 1;
}

- (NSInteger)collectionView:(UICollectionView *)collectionView numberOfItemsInSection:(NSInteger)section
{
    return self.seatCount;
}

- (UICollectionViewCell *)collectionView:(UICollectionView *)collectionView cellForItemAtIndexPath:(NSIndexPath *)indexPath
{
    UICollectionViewCell *cell = [collectionView dequeueReusableCellWithReuseIdentifier:@"Seat" forIndexPath:indexPath];
    Seat *seat = [self.table seatAtIndex:indexPath.item];
    [self updateCell:cell usingSeat:seat];
    return cell;
}

@end
