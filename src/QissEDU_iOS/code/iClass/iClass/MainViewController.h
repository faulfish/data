//
//  MainViewController.h
//  iClass
//
//  Created by Richard Ting on 8/14/13.
//  Copyright (c) 2013 alan.tu. All rights reserved.
//

#import <UIKit/UIKit.h>

#define QISSEDU_ROLE_TEACHER 0
#define QISSEDU_ROLE_STUDENT 1

@interface MainViewController : UIViewController

@property (strong, nonatomic)NSString *userID;
@property (strong, nonatomic)NSString *userName;
@property (nonatomic) NSUInteger role;

@end
