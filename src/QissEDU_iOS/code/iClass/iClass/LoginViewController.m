//
//  LoginViewController.m
//  iClass
//
//  Created by Richard Ting on 8/7/13.
//  Copyright (c) 2013 alan.tu. All rights reserved.
//

#import "LoginViewController.h"
#import "ClientAgent.h"
#import "MainViewController.h"
#import "GTMOAuth2ViewControllerTouch.h"

static NSString *const kKeychainItemName = @"iClass";

NSString *kMyClientID = @"1038111123701-d4nd6m56k406l4rtq5bp9nqh4vqhriqc.apps.googleusercontent.com";  // iClass native client id
NSString *kMyClientSecret = @"YtfVkZ5QWcSAR4lEJWAI1u98"; // iClass native client secret
NSString *scope = @"https://www.googleapis.com/auth/userinfo.profile";
bool isSignedIn = false;
GTMOAuth2Authentication *oAuthData;

@interface LoginViewController ()

@end

@implementation LoginViewController

@synthesize accountField =_accountField;
@synthesize passwordField = _passwordField;
@synthesize rememberMe = _rememberMe;
@synthesize loginButton = _loginButton;
@synthesize loginGoogleButton = _loginGoogleButton;
@synthesize spinner = _spinner;

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
    // Do any additional setup after loading the view.
    [super viewDidLoad];
    
    self.loginGoogleButton.contentHorizontalAlignment = UIControlContentHorizontalAlignmentLeft;
    self.loginGoogleButton.contentEdgeInsets = UIEdgeInsetsMake(0,55,0,0);
    [self.loginGoogleButton setImage:[UIImage imageNamed:@"btn_logingoogle_press.png"] forState:UIControlStateSelected | UIControlStateHighlighted];
    
    self.accountField.delegate = self;
    self.passwordField.delegate = self;
    
    ClientAgent *api = [[ClientAgent alloc] init];
    NSString *response = [api getExamAnswer:@"825047" error:nil];
    //NSString *response =[api submitExam:@"692001" examScheduleID:@"5665117697998848" studentAnswer:@"1,2-3,2" error: nil];
    //NSString *response =[api getCourseInfo:@"5287980914704384" date:@"20131014" error:nil];
    NSLog(@"response = %@", response);
    
    NSUserDefaults *userDefaults = [NSUserDefaults standardUserDefaults];
    if (userDefaults) {
        
        self.userAccount = [userDefaults objectForKey:USER_ACCOUNT];
        self.userName = [userDefaults objectForKey:USER_NAME];
        self.userID = [userDefaults objectForKey:USER_ID];
        self.role = [[userDefaults objectForKey:USER_ROLE] intValue] - 1;
        
        self.accountField.text = self.userAccount;
    }
    
    [self textFieldDidEndEditing:self.accountField];
    
}

- (void)viewDidAppear:(BOOL)animated
{
    [super viewDidAppear:animated];
    
    [UIView animateWithDuration:0.5 animations:^{
        [super viewDidAppear:animated];
        [self.navigationController setNavigationBarHidden:YES];
    }];
    
    NSUserDefaults *userDefaults = [NSUserDefaults standardUserDefaults];
    
    if (userDefaults) {
        
        NSString *tmpID = [userDefaults objectForKey:USER_ID];
        NSString *tmpName = [userDefaults objectForKey:USER_NAME];
        NSString *tmprole = [userDefaults objectForKey:USER_ROLE];
        
        if (tmpID && tmpName && tmprole) {
            [self performSegueWithIdentifier:@"ShowPlaner" sender:nil];
        }
    }
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender
{
    if ([segue.identifier isEqualToString:@"ShowPlaner"]) {
        if ([segue.destinationViewController respondsToSelector:@selector(setRole:)]) {
            [segue.destinationViewController setRole: (self.role)];
        }
        if ([segue.destinationViewController respondsToSelector:@selector(setUserName:)]) {
            [segue.destinationViewController setUserName: (self.userName)];
        }
        if ([segue.destinationViewController respondsToSelector:@selector(setUserID:)]) {
            [segue.destinationViewController setUserID: (self.userID)];
        }
    }
}

- (IBAction)selectRememberMe:(id)sender {
    
    self.rememberMe.selected = !self.rememberMe.selected;
    self.isRemember = self.rememberMe.selected;
}

- (IBAction)unwindToLogin:(UIStoryboardSegue *)segue
{
    
    isSignedIn = false;
    [GTMOAuth2ViewControllerTouch removeAuthFromKeychainForName:kKeychainItemName];
    [GTMOAuth2ViewControllerTouch revokeTokenForGoogleAuthentication:oAuthData];
    
    NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
    
    if (defaults) {
        [defaults setObject:nil forKey:USER_NAME];
        [defaults setObject:nil forKey:USER_ID];
        [defaults setObject:nil forKey:USER_ROLE];
        
        [defaults synchronize];
    }
    
    self.accountField.text = self.userAccount;
    
}


- (void)textFieldDidEndEditing:(UITextField *)textField {
    
    if (textField == self.accountField) {
        // Store the first name in a property, or array, or something.
    } else if (textField == self.passwordField) {
    }
}

- (IBAction)backgroundTap:(id)sender
{
    [self.accountField resignFirstResponder];
    [self.passwordField resignFirstResponder];
}

- (BOOL)textFieldShouldReturn:(UITextField *)textField {
    [textField resignFirstResponder];
    return NO;
}

- (IBAction)userName:(id)sender {
    [sender resignFirstResponder];
}

- (IBAction)password:(id)sender {
    [sender resignFirstResponder];
}

- (BOOL) shouldPerformSegueWithIdentifier:(NSString *)identifier sender:(id)sender
{
    if([identifier isEqualToString:@"ShowPlaner"]) {
        return NO;
    }
    
    return YES;
}

- (IBAction)login:(id)sender {
    
    self.userAccount = self.accountField.text;
    //self.password = self.passwordField.text;
    
    [self.spinner startAnimating];
    dispatch_queue_t loaderQ = dispatch_queue_create("login info loader", NULL);
    dispatch_async(loaderQ, ^{
        
        ClientAgent *api = [[ClientAgent alloc] init];
        NSString *response =[api loginByUserAccount:self.userAccount error: nil];
        
        dispatch_async(dispatch_get_main_queue(), ^{
            
            [self.spinner stopAnimating];
            
            if ([[self.accountField text] isEqualToString:@""] || response.length <= 0)  {
                
                UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Warning" message:@"Please login again!" delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil];
                [alert show];
                
            } else {
                
                if (response.length > 0) {
                    
                    NSData *jsonData = [response dataUsingEncoding:NSUTF8StringEncoding];
                    NSArray *json = [NSJSONSerialization JSONObjectWithData:jsonData options:0 error:nil];
                    NSDictionary *userDic = [json objectAtIndex:0];
                    
                    self.userID = [userDic objectForKey:@"userID"];
                    self.tmpRole = [userDic objectForKey:@"role"];
                    self.role = [self.tmpRole intValue] - 1;
                    self.userName = [userDic objectForKey:@"userName"];
                    
                    
                    if (self.isRemember) {
                        NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
                        
                        if (defaults) {
                            [defaults setObject:self.userAccount forKey: USER_ACCOUNT];
                            [defaults setObject:self.userName forKey:USER_NAME];
                            [defaults setObject:self.userID forKey:USER_ID];
                            [defaults setObject:self.tmpRole forKey:USER_ROLE];
                            
                            [defaults synchronize];
                        }
                    }
                    
                    [self performSegueWithIdentifier:@"ShowPlaner" sender:nil];
                    
                }
            }
        });
    });
}

- (IBAction)signInToGoogle:(id)sender
{
    if (!isSignedIn) {
        
        GTMOAuth2ViewControllerTouch *viewController;
        viewController = [[GTMOAuth2ViewControllerTouch alloc] initWithScope:scope
                                                                    clientID:kMyClientID
                                                                clientSecret:kMyClientSecret
                                                            keychainItemName:kKeychainItemName
                                                                    delegate:self
                                                            finishedSelector:@selector(viewController:finishedWithAuth:error:)];
        
        [self.navigationController pushViewController:viewController animated:YES];
        [self.navigationController setNavigationBarHidden:NO];
        
    } else {
        isSignedIn = false;
    }
}

- (void)viewController:(GTMOAuth2ViewControllerTouch *)viewController
      finishedWithAuth:(GTMOAuth2Authentication *)auth
                 error:(NSError *)error {
    
    oAuthData = auth;
    
    [self dismissViewControllerAnimated:YES completion:nil];
    
    if (error != nil) {
        // Authentication failed
        NSLog(@"error = %@",[error description]);
        
    } else {
        // Authentication succeeded
        isSignedIn = true;
        
        auth.authorizationTokenKey = @"id_token";
        NSLog(@"auth.userEmail = %@", auth.userEmail);
        
        [self.spinner startAnimating];
        dispatch_queue_t loaderQ = dispatch_queue_create("google login loader", NULL);
        dispatch_async(loaderQ, ^{
            
            ClientAgent *api = [[ClientAgent alloc] init];
            NSString *response =[api loginByUserAccount:auth.userEmail error: nil];
            
            dispatch_async(dispatch_get_main_queue(), ^{
                
                [self.spinner stopAnimating];
                
                if (response.length <= 0)  {
                    UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Warning" message:@"There is no user data in our server, please login again!" delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil];
                    [alert show];
                    
                } else {
                    
                    if (response.length > 0) {
                        
                        NSData *jsonData = [response dataUsingEncoding:NSUTF8StringEncoding];
                        NSArray *json = [NSJSONSerialization JSONObjectWithData:jsonData options:0 error:nil];
                        NSDictionary *userDic = [json objectAtIndex:0];
                        
                        self.userID = [userDic objectForKey:@"userID"];
                        self.tmpRole = [userDic objectForKey:@"role"];
                        self.role = [self.tmpRole intValue] - 1;
                        self.userName = [userDic objectForKey:@"userName"];
                        
                        NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
                        
                        if (defaults) {
                            [defaults setObject:self.userAccount forKey: USER_ACCOUNT];
                            [defaults setObject:self.userName forKey:USER_NAME];
                            [defaults setObject:self.userID forKey:USER_ID];
                            [defaults setObject:self.tmpRole forKey:USER_ROLE];
                            
                            [defaults synchronize];
                        }
                        
                        [self performSegueWithIdentifier:@"ShowPlaner" sender:nil];
                        
                    }
                }
            });
        });
    }
}

@end
