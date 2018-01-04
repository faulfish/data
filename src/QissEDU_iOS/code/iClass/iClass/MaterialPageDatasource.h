//
//  MaterialPageDatasource.h
//  iClass
//
//  Created by alan.tu on 9/3/13.
//  Copyright (c) 2013 alan.tu. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "MaterialPageViewController.h"
#import "QDiscover/DSShapeManager.h"

@interface MaterialPageDatasource : NSObject<UIPageViewControllerDataSource>

@property NSString *materialID;
@property (nonatomic) DSShapeManager* dsm;

- (void) setDsm:(DSShapeManager *)dsm;
- (MaterialPageViewController *)viewControllerAtIndex:(NSUInteger)index storyboard:(UIStoryboard *)storyboard;
- (NSUInteger)indexOfViewController:(MaterialPageViewController *)viewController;

@end
