//
//  ScheduleTVC.m
//  iClass
//
//  Created by Richard Ting on 10/9/13.
//  Copyright (c) 2013 alan.tu. All rights reserved.
//

#import "ScheduleTVC.h"
#import "CourseInfoTVC.h"
#import "ClientAgent.h"
#import "NSDate+Description.h"
#import "NSDate+String.h"

@interface ScheduleTVC ()
@property (strong, nonatomic) NSString *courseName;
@property (strong, nonatomic) NSString *className;
@property (strong, nonatomic) NSArray *courseHistory;
@end

@implementation ScheduleTVC

- (id)initWithStyle:(UITableViewStyle)style
{
    self = [super initWithStyle:style];
    if (self) {
        // Custom initialization
    }
    return self;
}

- (void)viewDidLoad
{
    [super viewDidLoad];

    // Uncomment the following line to preserve selection between presentations.
    // self.clearsSelectionOnViewWillAppear = NO;
 
    // Uncomment the following line to display an Edit button in the navigation bar for this view controller.
    // self.navigationItem.rightBarButtonItem = self.editButtonItem;

    // Setup background image
    UIImageView *backgroundView = [[UIImageView alloc] initWithImage:[UIImage imageNamed:@"bk_planner_right.png"]];
    backgroundView.contentMode = UIViewContentModeTopLeft;
    [self.tableView setBackgroundView:backgroundView];

    // Setup tableview selectable
    self.tableView.allowsSelection = (self.role == 0) ? YES : NO;

    [self.refreshControl addTarget:self
                            action:@selector(reloadCourseHistory)
                  forControlEvents:UIControlEventValueChanged];
//    [self reloadCourseHistory];
}

- (void)setCourseHistory:(NSArray *)courseHistory
{
    _courseHistory = courseHistory;
    [self.tableView reloadData];
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

#pragma mark - Table view data source

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView
{
    // Return the number of sections.
    return 1;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    // Return the number of rows in the section.
    return [self.courseHistory count];
}

- (NSString *)titleForRow:(NSUInteger)row
{
    return [NSString stringWithFormat:@"%@ - %@", [[NSDate date] description], @""];
}

// a helper method that looks in the Model for the course dictionary at the given row
//  and gets the owner of the photo out of it

- (NSString *)subtitleForRow:(NSUInteger)row
{
    return [NSString stringWithFormat:@"Attendance: %@, absent: %@", @"3", @"1"];
}

- (void)configureCell:(UITableViewCell *)cell withRole:(NSUInteger)role forRow:(NSUInteger)row
{
    if (role == 0) {
        cell.textLabel.text = [NSString stringWithFormat:@"%@ - %@", [self.courseHistory[row][QISSEDU_HISTORY_COURSE_DATE] description], [self.courseHistory[row][QISSEDU_HISTORY_COURSE_TIME] description]];
        cell.detailTextLabel.text = [NSString stringWithFormat:@"Attendance: %@, absent: %@", [self.courseHistory[row][QISSEDU_HISTORY_COURSE_ATTENDANCE] description], [self.courseHistory[row][QISSEDU_HISTORY_COURSE_ABSENT] description]];
    } else {
        NSString *title = [self.courseHistory[row][QISSEDU_HISTORY_MATERIAL_TITLE] description];
        if (!title) {
            title = [self.courseHistory[row][QISSEDU_HISTORY_EXAM_TITLE] description];
            if (!title) {
                title = [self.courseHistory[row][QISSEDU_HISTORY_COURSE_ANNOUNCEMENT] description];
            }
        }
        cell.textLabel.text = [NSString stringWithFormat:@"%@", title];
        cell.accessoryType = UITableViewCellAccessoryNone;
    }
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    NSString *CellIdentifier = (self.role == 0) ? @"HistoryCell1" : @"HistoryCell2";
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:CellIdentifier forIndexPath:indexPath];

    // Configure the cell...
    [self configureCell:cell withRole:self.role forRow:indexPath.row];
    
    return cell;
}

/*
// Override to support conditional editing of the table view.
- (BOOL)tableView:(UITableView *)tableView canEditRowAtIndexPath:(NSIndexPath *)indexPath
{
    // Return NO if you do not want the specified item to be editable.
    return YES;
}
*/

/*
// Override to support editing the table view.
- (void)tableView:(UITableView *)tableView commitEditingStyle:(UITableViewCellEditingStyle)editingStyle forRowAtIndexPath:(NSIndexPath *)indexPath
{
    if (editingStyle == UITableViewCellEditingStyleDelete) {
        // Delete the row from the data source
        [tableView deleteRowsAtIndexPaths:@[indexPath] withRowAnimation:UITableViewRowAnimationFade];
    }   
    else if (editingStyle == UITableViewCellEditingStyleInsert) {
        // Create a new instance of the appropriate class, insert it into the array, and add a new row to the table view
    }   
}
*/

/*
// Override to support rearranging the table view.
- (void)tableView:(UITableView *)tableView moveRowAtIndexPath:(NSIndexPath *)fromIndexPath toIndexPath:(NSIndexPath *)toIndexPath
{
}
*/

/*
// Override to support conditional rearranging of the table view.
- (BOOL)tableView:(UITableView *)tableView canMoveRowAtIndexPath:(NSIndexPath *)indexPath
{
    // Return NO if you do not want the item to be re-orderable.
    return YES;
}
*/

#pragma mark - Table view delegate

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
}

#pragma mark - Navigation

// In a story board-based application, you will often want to do a little preparation before navigation
- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender
{
    if ([sender isKindOfClass:[UITableViewCell class]]) {
        NSIndexPath *indexPath = [self.tableView indexPathForCell:sender];
        if (indexPath) {
            // Get the new view controller using [segue destinationViewController].
            // Pass the selected object to the new view controller.
            if ([segue.identifier isEqualToString:@"showCourseInfo"]) {
                if ([segue.destinationViewController isKindOfClass:[CourseInfoTVC class]]) {
                    CourseInfoTVC *controller = (CourseInfoTVC *)segue.destinationViewController;
                    controller.courseId = [self.courseHistory[indexPath.row][QISSEDU_HISTORY_COURSE_ID] description];
                    controller.courseDate = [NSDate dateWithString:[self.courseHistory[indexPath.row][QISSEDU_HISTORY_COURSE_DATE] description]];
                    controller.coursePeriod = [self.courseHistory[indexPath.row][QISSEDU_HISTORY_COURSE_TIME] description];
                    controller.courseName = self.courseName;
                    controller.className = self.className;
                    controller.role = self.role;
                }
            }
        }
    }
}

- (void)setCourseName:(NSString *)courseName className:(NSString *)className
{
    self.courseName = courseName;
    self.className = className;
    [self reloadCourseHistory];
}

- (void)reloadCourseHistory
{
    if ([self.courseName isEqualToString:@""] || [self.className isEqualToString:@""]) {
        NSLog(@"courseName or className is empty! They are nessacery parameters");
        return;
    }

    [self.refreshControl beginRefreshing];
    dispatch_queue_t loaderQ = dispatch_queue_create("course history loader", NULL);
    dispatch_async(loaderQ, ^{
        if (self.role == 0) {
            self.courseHistory = [self loadAttendanceHistoryWithCourseName:self.courseName className:self.className];
        } else {
            self.courseHistory = [self loadCourseHistoryWithCourseName:self.courseName className:self.className];
        }
        dispatch_async(dispatch_get_main_queue(), ^{
            [self.refreshControl endRefreshing];
        });
    });
}

- (NSArray *)loadAttendanceHistoryWithCourseName:(NSString *)courseName className:(NSString *)className
{
    ClientAgent *api = [[ClientAgent alloc] init];
    NSError *error = [[NSError alloc] init];
    NSString *dateString = [[NSDate date] yearAndMonthAndDayOnCalendar:[NSCalendar currentCalendar]];
    NSString *attendanceHistory = [api getAttendanceHistory:dateString className:className courseName:courseName error:&error];
    attendanceHistory = [NSString stringWithFormat:@"{\"AttendanceHistory\":%@}", attendanceHistory];
    NSData *jsonData = [attendanceHistory dataUsingEncoding:NSUTF8StringEncoding];
    NSDictionary *results = jsonData ? [NSJSONSerialization JSONObjectWithData:jsonData options:0 error:nil] : nil;
    return [results valueForKey:@"AttendanceHistory"];
}

- (NSArray *)loadCourseHistoryWithCourseName:(NSString *)courseName className:(NSString *)className
{
    ClientAgent *api = [[ClientAgent alloc] init];
    NSError *error = [[NSError alloc] init];
    NSString *dateString = [[NSDate date] yearAndMonthAndDayOnCalendar:[NSCalendar currentCalendar]];
    NSString *courseHistory = [api getCourseHistory:dateString className:className courseName:courseName error:&error];
    courseHistory = [NSString stringWithFormat:@"{\"CourseHistory\":%@}", courseHistory];
    NSData *jsonData = [courseHistory dataUsingEncoding:NSUTF8StringEncoding];
    NSDictionary *results = jsonData ? [NSJSONSerialization JSONObjectWithData:jsonData options:0 error:nil] : nil;
    return [results valueForKey:@"CourseHistory"];
}

@end
