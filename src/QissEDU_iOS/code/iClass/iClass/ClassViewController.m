//
//  ClassViewController.m
//  iClass
//
//  Created by alan.tu on 8/20/13.
//  Copyright (c) 2013 alan.tu. All rights reserved.
//

#import "ClassViewController.h"
#import "ClassTabBarController.h"
#include <ifaddrs.h>
#include <arpa/inet.h>


@interface ClassViewController ()
@property (weak, nonatomic) IBOutlet UIView *ClassContainerView;
@property (weak, nonatomic) IBOutlet UISegmentedControl *FunctionSegmentedControl;
@property (weak, nonatomic) IBOutlet UISegmentedControl *classSegment;

@end

ClassTabBarController *childViewController; //keep from another view controller

@implementation ClassViewController

@synthesize dsm = _dsm;
@synthesize materialID = _materialID;
@synthesize role = _role;

@synthesize FunctionSegmentedControl;
@synthesize ClassContainerView;

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Custom initialization
    }
    return self;
}

- (NSString *)getIPAddress {
    
    NSString *address = @"error";
    struct ifaddrs *interfaces = NULL;
    struct ifaddrs *temp_addr = NULL;
    int success = 0;
    // retrieve the current interfaces - returns 0 on success
    success = getifaddrs(&interfaces);
    if (success == 0) {
        // Loop through linked list of interfaces
        temp_addr = interfaces;
        while(temp_addr != NULL) {
            if(temp_addr->ifa_addr->sa_family == AF_INET) {
                // Check if interface is en0 which is the wifi connection on the iPhone
                if([[NSString stringWithUTF8String:temp_addr->ifa_name] isEqualToString:@"en0"]) {
                    // Get NSString from C String
                    address = [NSString stringWithUTF8String:inet_ntoa(((struct sockaddr_in *)temp_addr->ifa_addr)->sin_addr)];
                    
                }
                
            }
            
            temp_addr = temp_addr->ifa_next;
        }
    }
    // Free memory
    freeifaddrs(interfaces);
    return address;
    
}

- (void)viewDidLoad
{
    [super viewDidLoad];
	// Do any additional setup after loading the view.
    //childViewController = nil;
    [childViewController setSelectedIndex:[FunctionSegmentedControl selectedSegmentIndex]];
    NSLog(@"Material: role = %d", self.role);
    
    if (discover == nil) {
        discover = [[QDiscover alloc]initWithRole:self.role];
    }
    
    if (_dsm == nil) {
        _dsm = [[DSShapeManager alloc]initWithRole:self.role];
    }
                
    if(self.role == 0) {
        
        QDiscoverServiceInfo *info = [[QDiscoverServiceInfo alloc]init];
        info.ip = [self getIPAddress];
        info.friendlyName = @"QDrawSyncHost-";
        info.serviceName = @"QisdaDrawSyncHost";
    
        CFUUIDRef UUID = CFUUIDCreate(kCFAllocatorDefault);
        CFStringRef UUIDString = CFUUIDCreateString(kCFAllocatorDefault,UUID);
        info.uuid = (__bridge NSString *)(UUIDString);
        NSMutableString *str = [NSMutableString new];
        [str appendFormat:@"QDrawSyncHost-%@",[info.uuid substringFromIndex:[info.uuid length]-4]];
        info.friendlyName = str;
    
        //NSError *error = nil;
        //[discover addService:info.uuid info:info error:&error];
    }
    if (self.role == 1) {
        
    }
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (void) writeShort:(int) val target:(NSMutableData*) data
{
    Byte buffer[2] = {0};
    buffer[0] = (Byte)(val >> 8);
    buffer[1] = (Byte)(val);
    NSData *shortData = [[NSData alloc]initWithBytes:buffer length:2];
    [data appendData:shortData];
    NSLog(@"%@", data);
}

- (short) readShort:(NSMutableData*) data
{
    NSData *data2 = [data subdataWithRange:NSMakeRange(0, 2)];
    short len = CFSwapInt16BigToHost(*(short*)([data2 bytes]));
    return len;
}

-(void) writeUTF:(NSString *)str target:(NSMutableData *)data
{
    unichar buffer[1024] = {0};
    [str getCharacters:buffer range:NSMakeRange(0,[str length])];
    int lengthbyte = [str length];
    [self writeShort:lengthbyte target:data];
    NSData* strdata = [str dataUsingEncoding:NSUTF8StringEncoding];
    [data appendData:strdata];
    NSLog(@"%@", data);
}

- (IBAction)SwitchView:(UISegmentedControl *)sender forEvent:(UIEvent *)event {
    if([sender selectedSegmentIndex] == 0){
        [self SwitchTo:ClassViewType_Public];
    }else if([sender selectedSegmentIndex] == 1){
        [self SwitchTo:ClassViewType_Student];
    }else{
        [self SwitchTo:ClassViewType_Material];
    }
}

- (void) prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
    UIViewController *dest = [segue destinationViewController];
    if ([dest isKindOfClass:[ClassTabBarController class]]) {
        childViewController = (ClassTabBarController *) [segue destinationViewController];
        if (_dsm == nil) {
            _dsm = [[DSShapeManager alloc]initWithRole:self.role];
        }
        childViewController.dsm = self.dsm;
        childViewController.materialID = self.materialID;
        childViewController.materialTitle = self.materialTitle;
        childViewController.courseID = self.courseID;
        childViewController.courseDate = self.courseDate;
        childViewController.examID = self.examID;
        childViewController.examTitle = self.examTitle;
        childViewController.examScheduleID = self.examScheduleID;
    }
}

- (void) SwitchTo:(ClassViewType) type {
    switch (type) {
        case ClassViewType_Public:
            _classSegment.selectedSegmentIndex = 0;
            break;
        case ClassViewType_Material:
            _classSegment.selectedSegmentIndex = 2;
            break;
        case ClassViewType_Student:
            _classSegment.selectedSegmentIndex = 1;
            break;
        case ClassViewType_Thumbnail:
            _classSegment.selectedSegmentIndex = 0;
            break;
            
        default:
            break;
    }
    [childViewController setSelectedIndex:type];
}

- (IBAction)unwindToClass:(UIStoryboardSegue *)segue {
    //
}

@end
