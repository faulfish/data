//
//  MasterTabBarController.h
//  iClass
//
//  Created by Richard Ting on 9/18/13.
//  Copyright (c) 2013 alan.tu. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface MasterTabBarController : UITabBarController
@property (nonatomic) NSUInteger role;
@property (strong, nonatomic) NSString *userID;
@property (strong, nonatomic) NSString *userName;
@end
