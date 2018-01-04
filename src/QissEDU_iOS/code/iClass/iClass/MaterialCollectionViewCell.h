//
//  MaterialCollectionViewCell.h
//  iClass
//
//  Created by Richard Ting on 10/30/13.
//  Copyright (c) 2013 alan.tu. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "MaterialCardView.h"

@interface MaterialCollectionViewCell : UICollectionViewCell

@property (weak, nonatomic) IBOutlet MaterialCardView *materialCardView;

@end
