//
//  MainViewController.m
//  iClass
//
//  Created by Richard Ting on 8/14/13.
//  Copyright (c) 2013 alan.tu. All rights reserved.
//

#import "MainViewController.h"
#import "NSCalendarCategories.h"
#import "PlannerViewController.h"
#import "DetailNavigationController.h"
#import "DatePickerView.h"
#import "LatestCourseTVC.h"
#import "ScheduleNavigationController.h"
#import "UIColor+HexString.h"

@interface MainViewController () <DatePickerViewDataSource, DatePickerViewDelegate>

@property (strong, nonatomic) UITabBarController *masterViewController;
@property (strong, nonatomic) DetailNavigationController *firstDetailViewController;
@property (strong, nonatomic) ScheduleNavigationController *secondDetailViewController;
@property (weak, nonatomic) IBOutlet UIView *firstDetailView;
@property (weak, nonatomic) IBOutlet UIView *secondDetailView;
@property (weak, nonatomic) IBOutlet UISegmentedControl *tabPicker;
@property (weak, nonatomic) IBOutlet UIToolbar *masterToolbar;
@property (weak, nonatomic) IBOutlet UIToolbar *buttomToolbar;
@property (weak, nonatomic) IBOutlet DatePickerView *datePickerView;
@property (strong, nonatomic) NSDate *pickedDate;

@end

@implementation MainViewController

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
    [self reloadDetailContainerView];
    [self setupToolbarWithRole:self.role];
    [self setTabPickerTitle];
    [self.masterViewController.tabBar setHidden:YES];
}

- (void)viewDidAppear:(BOOL)animated
{
    [super viewDidAppear:animated];

    for (id controller in self.masterViewController.childViewControllers) {
        if ([controller isKindOfClass:[PlannerViewController class]]) {
            if ([self.firstDetailViewController.topViewController isKindOfClass:[CourseInfoTVC class]]) {
                ((PlannerViewController *)controller).courseInfoDelegate = self.firstDetailViewController.courseInfoTVCDelegate;
            }
        } else if ([controller isKindOfClass:[LatestCourseTVC class]]) {
            if ([self.secondDetailViewController.topViewController isKindOfClass:[ScheduleTVC class]]) {
                ((LatestCourseTVC *)controller).courseHistoryDelegate = self.secondDetailViewController.courseHistoryDelegate;
            }
        }
    }
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender
{
    if ([segue.identifier isEqualToString:@"masterContainerView"]) {
        if ([segue.destinationViewController isKindOfClass:[UITabBarController class]]) {
            self.masterViewController = segue.destinationViewController;
        }
    } else if ([segue.identifier isEqualToString:@"firstDetailContainerView"]) {
        if ([segue.destinationViewController isKindOfClass:[DetailNavigationController class]]) {
            self.firstDetailViewController = segue.destinationViewController;
        }
    } else if ([segue.identifier isEqualToString:@"secondDetailContainerView"]) {
        if ([segue.destinationViewController isKindOfClass:[ScheduleNavigationController class]]) {
            self.secondDetailViewController = segue.destinationViewController;
        }
    } else {
        NSLog(@"Unknown segue");
    }
    
    if ([segue.destinationViewController respondsToSelector:@selector(setRole:)]) {
        [segue.destinationViewController setRole:self.role];
    }
    if ([segue.destinationViewController respondsToSelector:@selector(setUserName:)]) {
        [segue.destinationViewController setUserName: (self.userName)];
    }
    if ([segue.destinationViewController respondsToSelector:@selector(setUserID:)]) {
        [segue.destinationViewController setUserID: (self.userID)];
    }
}

- (NSDate *)pickedDate
{
    if ([self.masterViewController.selectedViewController isKindOfClass:[PlannerViewController class]]) {
        PlannerViewController *controller = (PlannerViewController *)self.masterViewController.selectedViewController;
        return controller.pickedDate;
    } else {
        return nil;
    }
}

- (void)setPickedDate:(NSDate *)pickedDate
{
    if ([self.masterViewController.selectedViewController isKindOfClass:[PlannerViewController class]]) {
        PlannerViewController *controller = (PlannerViewController *)self.masterViewController.selectedViewController;
        controller.pickedDate = pickedDate;
    }
    self.datePickerView.date = pickedDate;
}

- (void)setupToolbarWithRole:(NSUInteger)role
{
    UIImage *image;
    UIColor *color;
    if (role == QISSEDU_ROLE_TEACHER) {
        image = [UIImage imageNamed:@"bk_planner_titlebar_left.png"];
        color = [UIColor colorWithHexString:@"#BA6DB3"];
    } else {
        image = [UIImage imageNamed:@"bk_planner_titlebar_left_b.png"];
        color = [UIColor colorWithHexString:@"#6FB3C1"];
    }
    [self.masterToolbar setTintColor:color];
    [self.masterToolbar setBackgroundImage:image forToolbarPosition:UIToolbarPositionTop barMetrics:UIBarMetricsDefault];
    [self.buttomToolbar setTintColor:color];
    [self.buttomToolbar setBackgroundImage:[UIImage imageNamed:@"bk_planner_bottom.png"] forToolbarPosition:UIToolbarPositionBottom barMetrics:UIBarMetricsDefault];

    self.datePickerView.dataSource = self;
    self.datePickerView.delegate = self;
    self.datePickerView.selectedColor = color;
}

- (void)setTabPickerTitle
{
    [self.tabPicker setTitle:@"課表" forSegmentAtIndex:0];
    [self.tabPicker setTitle:@"進度" forSegmentAtIndex:1];
}

- (IBAction)selectTab:(UISegmentedControl *)sender {
    [self.masterViewController setSelectedIndex:sender.selectedSegmentIndex];
    [self reloadDetailContainerView];
}

- (void)reloadDetailContainerView
{
    if (self.tabPicker.selectedSegmentIndex == 0) {
        self.firstDetailView.hidden = NO;
        self.secondDetailView.hidden = YES;
    } else {
        self.firstDetailView.hidden = YES;
        self.secondDetailView.hidden = NO;
    }
}

- (IBAction)previous:(id)sender {
    NSCalendar *calendar = [NSCalendar currentCalendar];
    [calendar setLocale:[NSLocale currentLocale]];
    NSDate *firstDayOfWeek = [calendar firstDayOfTheWeekUsingReferenceDate:self.pickedDate];
    self.pickedDate = [calendar dateBySubtractingWeeks:1 fromDate:firstDayOfWeek];
    [self.datePickerView layoutSubviews];
}

- (IBAction)next:(id)sender {
    NSCalendar *calendar = [NSCalendar currentCalendar];
    [calendar setLocale:[NSLocale currentLocale]];
    NSDate *firstDayOfWeek = [calendar firstDayOfTheWeekUsingReferenceDate:self.pickedDate];
    self.pickedDate = [calendar dateByAddingWeeks:1 toDate:firstDayOfWeek];
    [self.datePickerView layoutSubviews];
}

- (IBAction)today:(id)sender {
    self.pickedDate = [NSDate date];
    [self.datePickerView layoutSubviews];
}

- (NSUInteger)numberOfColumnsInDatePickerView:(DatePickerView *)datePickerView
{
    return 6;
}

- (IBAction)tapDatePickerView:(UITapGestureRecognizer *)gesture {
    [self.datePickerView tapHandler:[gesture locationInView:self.datePickerView]];
}

- (IBAction)refresh:(id)sender {
    if ([self.masterViewController.selectedViewController respondsToSelector:@selector(refresh)]) {
        [self.masterViewController.selectedViewController performSelector:@selector(refresh)];
    }
}

- (void)datePickerView:(DatePickerView *)datePickerView didSelectColumnAtIndex:(NSUInteger)index {
    self.pickedDate = [[NSCalendar currentCalendar] dateByAddingWeeks:index-2 toDate:self.pickedDate];
}

- (IBAction)unwindToMain:(UIStoryboardSegue *)segue
{
}

@end
