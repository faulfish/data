//
//  LoginViewController.h
//  iClass
//
//  Created by Richard Ting on 8/7/13.
//  Copyright (c) 2013 alan.tu. All rights reserved.
//

#import <UIKit/UIKit.h>

#define  USER_ACCOUNT @"account"
#define  USER_ID @"userID"
#define  USER_NAME @"userName"
#define  USER_ROLE @"role"

@interface LoginViewController : UIViewController<UITextFieldDelegate>

@property (weak, nonatomic) IBOutlet UIButton *rememberMe;
@property (weak, nonatomic) IBOutlet UITextField *accountField;
@property (weak, nonatomic) IBOutlet UITextField *passwordField;
@property (weak, nonatomic) IBOutlet UIButton *loginButton;
@property (weak, nonatomic) IBOutlet UIButton *loginGoogleButton;
@property (weak, nonatomic) IBOutlet UIActivityIndicatorView *spinner;

@property (strong, nonatomic) NSString *userAccount;
@property (strong, nonatomic) NSString *password;
@property (strong, nonatomic) NSString *userID;
@property (strong, nonatomic) NSString *userName;
@property (strong, nonatomic) NSString *tmpRole;
@property (nonatomic) BOOL isRemember;
@property (nonatomic) NSUInteger role;

@end
