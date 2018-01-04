//
//  StudentInfoViewController.m
//  iClass
//
//  Created by Richard Ting on 10/1/13.
//  Copyright (c) 2013 alan.tu. All rights reserved.
//

#import "StudentInfoViewController.h"

@interface StudentInfoViewController () <UITableViewDataSource, UITableViewDelegate>

@end

@implementation StudentInfoViewController

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
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    NSUInteger section = indexPath.section;

    UITableViewCell *cell;

    if (section == 0) {
        cell = [tableView dequeueReusableCellWithIdentifier:@"attendanceCell" forIndexPath:indexPath];
        cell.textLabel.text = @"出席記錄";
        cell.detailTextLabel.text = self.attendanceStatus ? @"出席" : @"缺席";
    } else if (section == 1) {
        cell = [tableView dequeueReusableCellWithIdentifier:@"noteCell" forIndexPath:indexPath];
        cell.textLabel.text = self.attendanceNote;
    }

    return cell;
}

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView
{
    return 2;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    return 1;
}

@end
