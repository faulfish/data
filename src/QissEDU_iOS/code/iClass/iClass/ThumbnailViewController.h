//
//  ThumbnailViewController.h
//  iClass
//
//  Created by alan.tu on 8/21/13.
//  Copyright (c) 2013 alan.tu. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "ThumbnailView.h"

@interface ThumbnailViewController : UIViewController<UICollectionViewDataSource,UICollectionViewDelegate,UICollectionViewDelegateFlowLayout>
@property (weak, nonatomic) IBOutlet ThumbnailView *collectionView;
@property (strong, nonatomic) NSString *contentID;

@end
