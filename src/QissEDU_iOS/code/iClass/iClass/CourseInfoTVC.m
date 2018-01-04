//
//  CourseInfoTVC.m
//  iClass
//
//  Created by Richard Ting on 9/13/13.
//  Copyright (c) 2013 alan.tu. All rights reserved.
//

#import "CourseInfoTVC.h"
#import "ClientAgent.h"
#import "NSDate+Description.h"
#import "ClassViewController.h"
#import "ZipFile.h"
#import "FileInZipInfo.h"
#import "ZipReadStream.h"

@interface CourseInfoTVC ()
@property (weak, nonatomic) IBOutlet UIBarButtonItem *materialDownloadButton;
@property (strong, nonatomic) NSArray *courseInfo;
@end

@implementation CourseInfoTVC

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
    UIImageView *backgroundView = [[UIImageView alloc] initWithImage:[UIImage imageNamed:@"bk_planner_right.png"]];
    backgroundView.contentMode = UIViewContentModeTopLeft;
    [self.tableView setBackgroundView:backgroundView];

    [self.refreshControl addTarget:self
                            action:@selector(loadCourseInfo)
                  forControlEvents:UIControlEventValueChanged];
    [self loadCourseInfo];

    // Uncomment the following line to preserve selection between presentations.
    // self.clearsSelectionOnViewWillAppear = NO;
 
    // Uncomment the following line to display an Edit button in the navigation bar for this view controller.
    // self.navigationItem.rightBarButtonItem = self.editButtonItem;
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender
{
    if ([segue.identifier isEqualToString:@"showMaterial"]) {
        if ([segue.destinationViewController respondsToSelector:@selector(setRole:)]) {
            [segue.destinationViewController setRole:self.role];
        }
    }
}

- (void)setCourseInfo:(NSArray *)courseInfo
{
    _courseInfo = courseInfo;
    [self.tableView reloadData];
}

- (void)loadCourseInfo
{
    if ([self.courseId length] != 16) {
        NSLog(@"Invalid courseId: %@", self.courseId);
        [self.refreshControl endRefreshing];
        return;
    }

    [self.refreshControl beginRefreshing];
    dispatch_queue_t loaderQ = dispatch_queue_create("course information loader", NULL);
    dispatch_async(loaderQ, ^{
        self.courseInfo = [self loadCourseInfoWithId:self.courseId];
        dispatch_async(dispatch_get_main_queue(), ^{
            self.materialDownloadButton.enabled = ([self isCourseDataDownloaded]) ? NO : YES;
            [self.refreshControl endRefreshing];
        });
    });
}

- (NSArray *)loadCourseInfoWithId:(NSString *)courseId
{
    ClientAgent *api = [[ClientAgent alloc] init];
    NSError *error = [[NSError alloc] init];
    NSString *dateString = [self.courseDate yearAndMonthAndDayOnCalendar:[NSCalendar currentCalendar]];
    NSString *courseInfo = [api getCourseInfo:courseId date:dateString error:&error];
    NSLog(@"courseInfo: %@", courseInfo);
    courseInfo = [NSString stringWithFormat:@"{\"CourseInfo\":%@}", courseInfo];
    NSData *jsonData = [courseInfo dataUsingEncoding:NSUTF8StringEncoding];
    NSDictionary *results = jsonData ? [NSJSONSerialization JSONObjectWithData:jsonData options:0 error:nil] : nil;
    return [results valueForKey:@"CourseInfo"];
}

- (BOOL)isCourseDataDownloaded
{
    BOOL materialResult = NO, examResult = NO;

    if ([self.courseInfo count] > 0) {
        NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
        NSString *documentPath = ([paths count] > 0) ? [[paths objectAtIndex:0] description] : nil;
        NSLog(@"documentPath: %@", documentPath);

        // Check material data
        NSString *materialID = [self.courseInfo[0][QISSEDU_COURSEINFO_MATERIAL_ID] description];
        NSString *materialPath = [NSString stringWithFormat:@"%@/zip/%@", documentPath, materialID];
        NSLog(@"materialPath: %@", materialPath);
        NSString *examID = [self.courseInfo[0][QISSEDU_COURSEINFO_EXAM_ID] description];
        NSString *examPath = [NSString stringWithFormat:@"%@/zip/%@", documentPath, examID];
        NSLog(@"examPath: %@", examPath);

        NSFileManager *fileManager = [NSFileManager defaultManager];
        if (materialID) {
            if ([fileManager fileExistsAtPath:materialPath] == YES) {
                materialResult = YES;
            }
        } else {
            materialResult = YES;
        }
        if (examID) {
            if ([fileManager fileExistsAtPath:examPath] == YES) {
                examResult = YES;
            }
        } else {
            examResult = YES;
        }
    }

    return (materialResult && examResult);
}

#pragma mark - Table view data source

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView
{
    // Return the number of sections.
    return self.courseInfo ? 4 : 0;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    // Return the number of rows in the section.
    NSInteger rowCount = 0;

    switch (section) {
        case 0:
            rowCount = 2;
            break;

        case 1:
            rowCount = 1;
            break;

        case 2:
            rowCount = 1;
            break;

        case 3:
            rowCount = 1;
            break;

        default:
            break;
    }

    return rowCount;
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    NSUInteger row = indexPath.row;
    NSUInteger section = indexPath.section;

    UITableViewCell *cell;

    if (section == 0) {
        if (row == 0) {
            cell = [tableView dequeueReusableCellWithIdentifier:@"CourseInfoCell1" forIndexPath:indexPath];
        } else {
            cell = [tableView dequeueReusableCellWithIdentifier:@"CourseInfoCell2" forIndexPath:indexPath];
        }
    } else if (section == 1) {
        cell = [tableView dequeueReusableCellWithIdentifier:@"CourseInfoCell4" forIndexPath:indexPath];
    } else {
        cell = [tableView dequeueReusableCellWithIdentifier:@"CourseInfoCell3" forIndexPath:indexPath];
    }

    // Configure the cell...
    switch (section) {
        case 0:
            switch (row) {
                case 0:
                    cell.textLabel.text = [NSString stringWithFormat:@"%@ %@", [self.courseDate description], [self.courseDate localizedWeekdayName:[NSCalendar currentCalendar]]];
                    cell.detailTextLabel.text = self.coursePeriod;
                    break;

                case 1:
                    if (self.role == 0) {
                        NSString *attendanceString = [self.courseInfo[0][QISSEDU_COURSEINFO_ATTENDANCE] description];
                        NSString *absentString = [self.courseInfo[0][QISSEDU_COURSEINFO_ABSENT] description];
                        if (attendanceString && absentString) {
                            cell.textLabel.text = [NSString stringWithFormat:@"出席: %@, 缺席: %@", [self.courseInfo[0][QISSEDU_COURSEINFO_ATTENDANCE] description], [self.courseInfo[0][QISSEDU_COURSEINFO_ABSENT] description]];
                        } else {
                            cell.textLabel.text = @"未點名";
                        }
                    } else {
                        cell.textLabel.text = @"上課中";
                    }

                    break;
                    
                default:
                    break;
            }

            break;

        case 1:
            cell.textLabel.text = [self.courseInfo[0][QISSEDU_COURSEINFO_ANNOUNCEMENT] description];
            break;

        case 2:
            cell.textLabel.text = [self.courseInfo[0][QISSEDU_COURSEINFO_MATERIAL_TITLE] description];
            [self configureDownloadState:[self.courseInfo[0][QISSEDU_COURSEINFO_MATERIAL_ID] description] forCell:cell];
            break;

        case 3:
            cell.textLabel.text = [self.courseInfo[0][QISSEDU_COURSEINFO_EXAM_TITLE] description];
            [self configureDownloadState:[self.courseInfo[0][QISSEDU_COURSEINFO_EXAM_ID] description] forCell:cell];
            break;

        default:
            break;
    }

    return cell;
}

- (void)configureDownloadState:(NSString *)folder forCell:(UITableViewCell *)cell
{
    if (cell.textLabel.text != NULL) {
        NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
        NSString *documentPath = ([paths count] > 0) ? [[paths objectAtIndex:0] description] : nil;
        NSString *materialPath =  [NSString stringWithFormat:@"%@/zip/%@", documentPath, folder];
        NSLog(@"dataPath: %@", materialPath);
        if ([[NSFileManager defaultManager] fileExistsAtPath:materialPath] == NO) {
            cell.detailTextLabel.text = @"未下載";
            cell.userInteractionEnabled = NO;
            cell.accessoryType = UITableViewCellAccessoryNone;
            cell.textLabel.alpha = 0.3;
            cell.detailTextLabel.alpha = 0.3;
        } else {
            cell.detailTextLabel.text = @"已下載";
            cell.userInteractionEnabled = YES;
            cell.accessoryType = UITableViewCellAccessoryDisclosureIndicator;
            cell.textLabel.alpha = 1.0;
            cell.detailTextLabel.alpha = 1.0;
        }
    } else {
        cell.detailTextLabel.text = @"";
        cell.accessoryType = UITableViewCellAccessoryNone;
    }
}

- (NSString *)tableView:(UITableView *)tableView titleForHeaderInSection:(NSInteger)section
{
    NSString *title = @"";

    switch (section) {
        case 1:
            title = @"公告";
            break;

        case 2:
            title = @"上課教材";
            break;

        case 3:
            title = @"小考";
            break;

        default:
            break;
    }

    return title;
}

- (void)setCourseId:(NSString *)courseId
         courseName:(NSString *)courseName
          className:(NSString *)className
               date:(NSDate *)date
             period:(NSString *)period
{
    self.courseId = courseId;
    self.courseName = courseName;
    self.className = className;
    self.courseDate = date;
    self.coursePeriod = period;

    if (courseId == nil) {
        self.courseId = @"5717546363781120";
        self.courseDate = [NSDate date];
        self.coursePeriod = @"08:10-10:00";
        self.courseName = @"英文";
        self.className = @"一年乙班";
    }
    [self.refreshControl beginRefreshing];
    if (self.role == 0) {
        [self setTitle:[NSString stringWithFormat:@"%@ - %@", self.className, self.courseName]];
    } else {
        [self setTitle:self.courseName];
    }
    self.courseInfo = nil;
    [self loadCourseInfo];
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
    // Navigation logic may go here. Create and push another view controller.
    /*
     <#DetailViewController#> *detailViewController = [[<#DetailViewController#> alloc] initWithNibName:@"<#Nib name#>" bundle:nil];
     // ...
     // Pass the selected object to the new view controller.
     [self.navigationController pushViewController:detailViewController animated:YES];
     */
    UIStoryboard *storyboard = [UIStoryboard storyboardWithName:@"Class" bundle:nil];
    if ([[storyboard instantiateInitialViewController] isKindOfClass:[ClassViewController class]]) {
        ClassViewController *contentViewController = (ClassViewController *)[storyboard instantiateInitialViewController];
        contentViewController.role = self.role;
        contentViewController.materialID = [self.courseInfo[0][QISSEDU_COURSEINFO_MATERIAL_ID] description];
        contentViewController.materialTitle = [self.courseInfo[0][QISSEDU_COURSEINFO_MATERIAL_TITLE] description];
        contentViewController.examID = [self.courseInfo[0][QISSEDU_COURSEINFO_EXAM_ID] description];
        contentViewController.examTitle = [self.courseInfo[0][QISSEDU_COURSEINFO_EXAM_TITLE] description];
        contentViewController.examScheduleID = [self.courseInfo[0][QISSEDU_COURSEINFO_EXAM_SCHEDULE_ID] description];
        contentViewController.courseID = self.courseId;
        contentViewController.courseDate = [self.courseDate yearAndMonthAndDayOnCalendar:[NSCalendar currentCalendar]];
        [self presentViewController:contentViewController animated:YES completion:NULL];
        if (indexPath.section == 0) {
            [contentViewController SwitchTo:ClassViewType_Student];
        } else if (indexPath.section == 2) {
            [contentViewController SwitchTo:ClassViewType_Material];
        }
    }
}

- (IBAction)materialDownload:(id)sender {
    UIAlertView *alertView = [[UIAlertView alloc]initWithTitle:@"課程資料下載中" message:@"請稍後..." delegate:self cancelButtonTitle:nil otherButtonTitles:nil];
    [alertView show];

    UIActivityIndicatorView *indicator = [[UIActivityIndicatorView alloc] initWithActivityIndicatorStyle:UIActivityIndicatorViewStyleWhiteLarge];
    // Adjust the indicator to place at the bottom of the dialog window.
    indicator.center = CGPointMake(alertView.bounds.size.width / 2, alertView.bounds.size.height - 50);
    [indicator startAnimating];
    [alertView addSubview:indicator];

    dispatch_queue_t downloaderQ = dispatch_queue_create("download material", NULL);

    dispatch_async(downloaderQ, ^{
        // Download material
        NSData *materialData;
        NSString *materialID = [self.courseInfo[0][QISSEDU_COURSEINFO_MATERIAL_ID] description];
        NSString *materialURL = [self.courseInfo[0][QISSEDU_COURSEINFO_MATERIAL_URL] description];
        if (materialURL) {
            materialData = [[NSData alloc] initWithContentsOfURL:[[NSURL alloc] initWithString:materialURL]];
        }

        // Download exam
        NSData *examData;
        NSString *examID = [self.courseInfo[0][QISSEDU_COURSEINFO_EXAM_ID] description];
        NSString *examURL = [self.courseInfo[0][QISSEDU_COURSEINFO_EXAM_URL] description];
        if (examURL) {
            examData = [[NSData alloc] initWithContentsOfURL:[[NSURL alloc] initWithString:examURL]];
        }

        dispatch_async(dispatch_get_main_queue(), ^{
            NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
            NSString *documentPath = ([paths count] > 0) ? [[paths objectAtIndex:0] description]: nil;
            NSString *zipPath;

            UIAlertView *downloadAlertView;
            BOOL materialResult = NO, examResult = NO;

            // Extract material data
            if (materialID) {
                zipPath = [documentPath stringByAppendingPathComponent:materialID];
                if ([materialData writeToFile:zipPath atomically:YES]) {
                    NSLog(@"Write to '%@' success.", zipPath);
                    [self extractZipFile:zipPath toDestination:[NSString stringWithFormat:@"%@/zip/%@", documentPath, materialID]];
                    materialResult = YES;
                } else {
                    NSLog(@"Write to '%@' fail", zipPath);
                }
            } else {
                materialResult = YES;
            }

            // Extract exam data
            if (examID) {
                zipPath = [documentPath stringByAppendingPathComponent:examID];
                if ([examData writeToFile:zipPath atomically:YES]) {
                    NSLog(@"Write to '%@' success.", zipPath);
                    [self extractZipFile:zipPath toDestination:[NSString stringWithFormat:@"%@/zip/%@", documentPath, examID]];
                    examResult = YES;
                } else {
                    NSLog(@"Write to '%@' fail", zipPath);
                }
            } else {
                examResult = YES;
            }

            // Show download result
            if (materialResult && examResult) {
                downloadAlertView = [[UIAlertView alloc]initWithTitle:@"下載成功" message:nil delegate:self cancelButtonTitle:@"確認" otherButtonTitles:nil];
            } else {
                downloadAlertView = [[UIAlertView alloc]initWithTitle:@"下載失敗" message:@"請重新下載" delegate:self cancelButtonTitle:@"確認" otherButtonTitles:nil];
            }

            [alertView dismissWithClickedButtonIndex:0 animated:YES];
            [downloadAlertView show];
            [self.tableView reloadData];
        });
    });
}

- (void)extractZipFile:(NSString *)zipFilePath toDestination:(NSString *)destination
{
    if ([[NSFileManager defaultManager] createDirectoryAtPath:destination
                                   withIntermediateDirectories:YES
                                                    attributes:nil
                                                         error:nil]) {
        NSLog(@"Success to create destination folder, '%@'", destination);

        ZipFile *zipFile;
        @try {
            zipFile = [[ZipFile alloc] initWithFileName:zipFilePath mode:ZipFileModeUnzip];
        }
        @catch (NSException *exception) {
            NSLog(@"Exception: %@", exception);
            [[NSFileManager defaultManager] removeItemAtPath:destination error:nil];
            zipFile = nil;
        }
        @finally {
            // perform tasks necessary whether exception occurred or not
            // Delete the material zip file
            [[NSFileManager defaultManager] removeItemAtPath:zipFilePath error:nil];
        }

        if (zipFile) {
            NSLog(@"Open zip file");

            NSArray *infos = [zipFile listFileInZipInfos];
            for (FileInZipInfo *info in infos) {
                [zipFile locateFileInZip:info.name];
                ZipReadStream *readStream = [zipFile readCurrentFileInZip];
                NSMutableData *data = [[NSMutableData alloc] initWithLength:info.length];
                int bytesRead = [readStream readDataWithBuffer:data];
                while (bytesRead > 0) {
                    bytesRead = [readStream readDataWithBuffer:data];
                }
                [readStream finishedReading];

                NSString *unzipFile = [destination stringByAppendingPathComponent:info.name];
                [data writeToFile:unzipFile atomically:YES];
            }
        }
    }
}

@end
