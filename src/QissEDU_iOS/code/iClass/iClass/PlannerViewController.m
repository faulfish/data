//
//  PlannerViewController.m
//  MyTestApp
//
//  Created by Richard Ting on 8/8/13.
//  Copyright (c) 2013 Richard Ting. All rights reserved.
//

#import "PlannerViewController.h"
#import "PlannerHeaderView.h"
#import "PlannerView.h"
#import "Planner.h"
#import "Course.h"
#import "NSCalendar+Components.h"
#import "NSCalendar+Juncture.h"
#import "NSCalendar+DateComparison.h"
#import "NSCalendar+DateManipulation.h"
#import "NSDate+Description.h"
#import "UIColor+HexString.h"
#import "ClientAgent.h"

@interface PlannerViewController ()

@property (weak, nonatomic) IBOutlet PlannerView *plannerView;
@property (weak, nonatomic) IBOutlet UIActivityIndicatorView *spinner;
@property (strong, nonatomic) Planner *planner;

@end

@implementation PlannerViewController

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

    UIImageView *backgroundView = [[UIImageView alloc] initWithImage:[UIImage imageNamed:@"bk_planner_left.png"]];
    backgroundView.contentMode = UIViewContentModeTopLeft;
    [self.view insertSubview:backgroundView belowSubview:self.plannerView];

    UIColor *spinnerColor, *courseCellColor;
    if (self.role == 0) {
        spinnerColor = [UIColor colorWithHexString:@"#BA6DB3"];
        courseCellColor = [UIColor colorWithHexString:@"#B546A4"];
    } else {
        spinnerColor = [UIColor colorWithHexString:@"#6FB3C1"];
        courseCellColor = [UIColor colorWithHexString:@"#478F9E"];
    }
    [self.spinner setColor:spinnerColor];
    [self.plannerView setCourseCellColor:courseCellColor];
    self.plannerView.role = self.role;
    self.plannerView.userID = self.userID;
    self.plannerView.userName = self.userName;

    // Planner View
    //[self showCurrentWeek];

    [self updateCoursesForPlanner:YES];
}

- (void)viewDidAppear:(BOOL)animated
{
    [super viewDidAppear:animated];
}

- (Planner *)planner
{
    if (!_planner) _planner = [[Planner alloc] init];
    return _planner;
}

- (void)updateCoursesForPlanner:(BOOL)useCache
{
    [self.spinner startAnimating];
    dispatch_queue_t loaderQ = dispatch_queue_create("latest course loader", NULL);
    dispatch_async(loaderQ, ^{
        NSArray *latestCourses = [self getCoursesWithId:self.userID useCache:useCache];
        self.planner = nil;
        [self.planner setupWithCourses:latestCourses];
        dispatch_async(dispatch_get_main_queue(), ^{
            [self.spinner stopAnimating];
            [self showPlannerCourse];
        });
    });
}

- (NSArray *)getCoursesWithId:(NSString *)userId useCache:(BOOL)useCache
{
    ClientAgent *api = [[ClientAgent alloc] init];
    if (!useCache) {
        [api addCachePolicy:ASIDoNotReadFromCacheCachePolicy];
    }
    NSString *data = [api getClassTable:userId error:nil];
    data = [NSString stringWithFormat:@"{\"Courses\":%@}", data];
    NSData *jsonData = [data dataUsingEncoding:NSUTF8StringEncoding];
    NSDictionary *results = jsonData ? [NSJSONSerialization JSONObjectWithData:jsonData options:0 error:nil] : nil;
    return [results valueForKey:@"Courses"];
}

- (void)showPlannerCourse
{
    self.plannerView.courses = self.planner.courses;
}

- (NSDate *)pickedDate
{
    return self.plannerView.pickedDate;
}

- (void)setPickedDate:(NSDate *)pickedDate
{
    self.plannerView.pickedDate = pickedDate;
}

- (id<CourseInfoTVCDelegate>)courseInfoDelegate
{
    if ([self.plannerView respondsToSelector:@selector(courseInfoDelegate:)]) {
        return self.plannerView.courseInfoDelegate;
    } else {
        return nil;
    }
}

- (void)setCourseInfoDelegate:(id<CourseInfoTVCDelegate>)courseInfoDelegate
{
    if ([self.plannerView respondsToSelector:@selector(setCourseInfoDelegate:)]) {
        [self.plannerView setCourseInfoDelegate:courseInfoDelegate];
    }
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (void)refresh
{
    self.plannerView.courses = nil;
    [self updateCoursesForPlanner:NO];
}

@end
