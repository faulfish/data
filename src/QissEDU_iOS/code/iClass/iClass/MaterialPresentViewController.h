//
//  MaterialPresentViewController.h
//  iClass
//
//  Created by alan.tu on 8/21/13.
//  Copyright (c) 2013 alan.tu. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "MaterialSplitViewController.h"
#import "QDiscover/DSShapeManager.h"

@interface MaterialPresentViewController : UIViewController<UISplitViewControllerDelegate,UIPageViewControllerDelegate, ShapeDelegate>

@property (strong, nonatomic) DSShapeManager *dsm;
@property (strong, nonatomic) NSString *materialID;
@property (strong, nonatomic) UIPageViewController *pageViewController;
@property (strong, nonatomic) id<MaterialSplitViewDelegate> materialSplitViewDelegate;
@property (weak, nonatomic) IBOutlet UIView *presentView;

- (void)showContentWithContentID:(NSString *)contentID;

//shapedelegate
- (void) eventChanegePage:(NSInteger)pageID;

@end
