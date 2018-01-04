//
//  ClassTabBarController.h
//  iClass
//
//  Created by alan.tu on 8/20/13.
//  Copyright (c) 2013 alan.tu. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "QDiscover/DSShapeManager.h"

@interface ClassTabBarController : UITabBarController

@property (strong, nonatomic) DSShapeManager* dsm;
@property (strong, nonatomic) NSString *contentID;
@property (strong, nonatomic) NSString *materialID;
@property (strong, nonatomic) NSString *materialTitle;
@property (strong, nonatomic) NSString *courseID;
@property (strong, nonatomic) NSString *courseDate;
@property (strong, nonatomic) NSString *examID;
@property (strong, nonatomic) NSString *examTitle;
@property (strong, nonatomic) NSString *examScheduleID;

@end
