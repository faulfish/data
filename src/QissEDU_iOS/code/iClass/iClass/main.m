//
//  main.m
//  iClass
//
//  Created by alan.tu on 7/24/13.
//  Copyright (c) 2013 alan.tu. All rights reserved.
//

#import <UIKit/UIKit.h>

#import "qisseduAppDelegate.h"
#import "NSURLConnectionEx.h"

@interface Car : NSObject  {
    NSString *name;
    NSMutableArray *tires;
    
    NSString *make;
    NSString *model;
    int modelYear;
    int numberOfDoors;
    float mileage;
}
@end


int main(int argc, char *argv[])
{
    @autoreleasepool {
        
        /*
        //test url
        
        [NSURLConnectionEx load];
        [NSURLConnectionEx unittest];
        
        NSURL *url = [[NSURL alloc]initWithString:@"http://udn.com"];
        NSURLRequest *request = [NSURLRequest requestWithURL:url];
        NSURLResponse *response = nil;
        NSError *error = nil;
        NSData *_data = [NSURLConnectionEx sendSynchronousRequest:request returningResponse:&response error:&error];
        if(_data == nil) {
            NSLog(@"%s: Error: %@", __PRETTY_FUNCTION__, [error localizedDescription]);
        }
        else {
            NSLog(@"data: %@, length: %d", _data, [_data length]);
            NSLog(@"%@", [[request URL]absoluteString]);
        }
        NSData *_data2 = [NSURLConnectionEx sendSynchronousRequest:request returningResponse:&response error:&error];
        if(_data2 == nil) {
            NSLog(@"%s: Error: %@", __PRETTY_FUNCTION__, [error localizedDescription]);
        }
        else {
            NSLog(@"data: %@, length: %d", _data2, [_data2 length]);
            NSLog(@"%@", [[request URL]absoluteString]);
        }
        
        [NSURLConnectionEx save];
        [NSURLConnectionEx unittest];
         */
        
        return UIApplicationMain(argc, argv, nil, NSStringFromClass([qisseduAppDelegate class]));
    }
}
