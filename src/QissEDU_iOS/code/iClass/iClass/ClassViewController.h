//
//  ClassViewController.h
//  iClass
//
//  Created by alan.tu on 8/20/13.
//  Copyright (c) 2013 alan.tu. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "QDiscover/QDiscover.h"
#import "QDiscover/DSShapeManager.h"

typedef enum _ClassViewType {
    ClassViewType_Public = 0,
    ClassViewType_Material = 1,
    ClassViewType_Student = 2,
    ClassViewType_Thumbnail = 3
} ClassViewType;

@interface ClassViewController : UIViewController {
    QDiscover* discover;
}

@property (nonatomic) DSShapeManager *dsm;
@property (nonatomic) NSUInteger role;
@property (strong, nonatomic) NSString *materialID;
@property (strong, nonatomic) NSString *materialTitle;
@property (strong, nonatomic) NSString *courseID;
@property (strong, nonatomic) NSString *courseDate;
@property (strong, nonatomic) NSString *examID;
@property (strong, nonatomic) NSString *examTitle;
@property (strong, nonatomic) NSString *examScheduleID;

- (void) SwitchTo:(ClassViewType) type;

@end
