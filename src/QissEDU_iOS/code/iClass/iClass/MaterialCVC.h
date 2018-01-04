//
//  MaterialCVC.h
//  iClass
//
//  Created by Richard Ting on 10/24/13.
//  Copyright (c) 2013 alan.tu. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "MaterialSplitViewController.h"

@interface MaterialCVC : UICollectionViewController

@property (strong, nonatomic) NSString *materialID;
@property (strong, nonatomic) NSString *materialTitle;
@property (strong, nonatomic) NSString *examID;
@property (strong, nonatomic) NSString *examTitle;
@property (strong, nonatomic) id<MaterialSplitViewDelegate> materialItemDelegate;

@end
