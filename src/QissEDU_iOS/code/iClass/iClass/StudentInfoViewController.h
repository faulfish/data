//
//  StudentInfoViewController.h
//  iClass
//
//  Created by Richard Ting on 10/1/13.
//  Copyright (c) 2013 alan.tu. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface StudentInfoViewController : UIViewController

@property (nonatomic) BOOL attendanceStatus;
@property (strong, nonatomic) NSString *attendanceNote;
@property (strong, nonatomic) NSString *studentId;
@property (strong, nonatomic) NSString *studentName;

@end
