//
//  MaterialSplitViewController.h
//  iClass
//
//  Created by Richard Ting on 10/24/13.
//  Copyright (c) 2013 alan.tu. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "QDiscover/DSShapeManager.h"

@protocol MaterialSplitViewDelegate <NSObject>

- (void)setMasterViewHidden:(BOOL)hidden;
- (void)toThumbnail;
- (void)showContentWithContentID:(NSString *)contentID;

@end

@interface MaterialSplitViewController : UIViewController

@property (strong, nonatomic) DSShapeManager *dsm;
@property (strong, nonatomic) NSString *materialID;
@property (strong, nonatomic) NSString *materialTitle;
@property (strong, nonatomic) NSString *examID;
@property (strong, nonatomic) NSString *examTitle;
@property (strong, nonatomic) NSString *examScheduleID;

@end
