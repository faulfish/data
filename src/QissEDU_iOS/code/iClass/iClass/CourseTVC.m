//
//  CourseTVC.m
//  iClass
//
//  Created by Richard Ting on 8/22/13.
//  Copyright (c) 2013 alan.tu. All rights reserved.
//

#import "CourseTVC.h"

@interface CourseTVC ()

@end

@implementation CourseTVC

- (id)initWithStyle:(UITableViewStyle)style
{
    self = [super initWithStyle:style];
    if (self) {
        // Custom initialization
    }
    return self;
}

- (void)setCourses:(NSArray *)courses
{
    _courses = courses;
    [self.tableView reloadData];
}

- (void)viewDidLoad
{
    [super viewDidLoad];

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

#pragma mark - Table view data source

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView
{
    // Return the number of sections.
    return 1;
}

- (NSString *)tableView:(UITableView *)tableView titleForHeaderInSection:(NSInteger)section
{
    NSString *header;
    if ([self.courses count] > 0) {
        if (self.role == 0) {
            header = [NSString stringWithFormat:@"%@老師 課表", self.userName];
        } else {
            header = [NSString stringWithFormat:@"%@ 課表", self.userName];
        }
    }
    return header;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    // Return the number of rows in the section.
    return [self.courses count];
}

// a helper method that looks in the Model for the course dictionary at the given row
//  and gets the title out of it

- (NSString *)titleForRow:(NSUInteger)row
{
    NSString *title;
    if (self.role == 0) {
        title = [NSString stringWithFormat:@"%@ - %@", [self.courses[row][QISSEDU_CLASS_NAME] description], [self.courses[row][QISSEDU_COURSE_NAME] description]];
    } else {
        title = [self.courses[row][QISSEDU_COURSE_NAME] description];
    }
    return title;
}

// a helper method that looks in the Model for the course dictionary at the given row
//  and gets the owner of the photo out of it

- (NSString *)subtitleForRow:(NSUInteger)row
{
    return [NSString stringWithFormat:@"最新進度： %@", [self.courses[row][QISSEDU_MATERIAL_TITLE] description]];
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    static NSString *CellIdentifier = @"Course";
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:CellIdentifier forIndexPath:indexPath];

    // Configure the cell...
    cell.textLabel.text = [self titleForRow:indexPath.row];
    cell.detailTextLabel.text = [self subtitleForRow:indexPath.row];

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
    // Navigation logic may go here. Create and push another view controller.
    /*
     <#DetailViewController#> *detailViewController = [[<#DetailViewController#> alloc] initWithNibName:@"<#Nib name#>" bundle:nil];
     // ...
     // Pass the selected object to the new view controller.
     [self.navigationController pushViewController:detailViewController animated:YES];
     */
    NSString *courseName = [self.courses[indexPath.row][QISSEDU_COURSE_NAME] description];
    NSString *className = [self.courses[indexPath.row][QISSEDU_CLASS_NAME] description];

    if ([self.courseHistoryDelegate respondsToSelector:@selector(setCourseName:className:)]) {
        [self.courseHistoryDelegate setCourseName:courseName className:className];
    }
}

@end
