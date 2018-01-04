//
//  LatestCourseTVC.m
//  iClass
//
//  Created by Richard Ting on 8/22/13.
//  Copyright (c) 2013 alan.tu. All rights reserved.
//

#import "LatestCourseTVC.h"
#import "ClientAgent.h"

@interface LatestCourseTVC ()

@end

@implementation LatestCourseTVC

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

    UIImageView *backgroundView = [[UIImageView alloc] initWithImage:[UIImage imageNamed:@"bk_planner_left.png"]];
    backgroundView.contentMode = UIViewContentModeTopLeft;
    [self.tableView setBackgroundView:backgroundView];

    [self loadLatestCourses:YES];
    [self.refreshControl addTarget:self
                            action:@selector(loadLatestCourses:)
                  forControlEvents:UIControlEventValueChanged];
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (void)loadLatestCourses:(BOOL)useCache
{
    [self.refreshControl beginRefreshing];
    dispatch_queue_t loaderQ = dispatch_queue_create("latest course loader", NULL);
    dispatch_async(loaderQ, ^{
        NSArray *latestCourses = [self loadCourseListFromUserId:self.userID useCache:useCache];
        dispatch_async(dispatch_get_main_queue(), ^{
            self.courses = latestCourses;
            [self.refreshControl endRefreshing];
        });
    });
}

- (NSArray *)loadCourseListFromUserId:(NSString *)userId useCache:(BOOL)useCache
{
    ClientAgent *api = [[ClientAgent alloc] init];
    if (!useCache) {
        [api addCachePolicy:ASIDoNotReadFromCacheCachePolicy];
    }
    NSError *error = [[NSError alloc] init];
    NSString *classList = [api getCourseSchedule:userId error:&error];
    classList = [NSString stringWithFormat:@"{\"Courses\":%@}", classList];
    NSData *jsonData = [classList dataUsingEncoding:NSUTF8StringEncoding];
    NSDictionary *results = jsonData ? [NSJSONSerialization JSONObjectWithData:jsonData options:0 error:nil] : nil;
    return [results valueForKey:@"Courses"];
}

- (void)refresh
{
    self.courses = nil;
    [self loadLatestCourses:NO];
}

@end
