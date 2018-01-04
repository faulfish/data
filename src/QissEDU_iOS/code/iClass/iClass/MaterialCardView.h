//
//  MaterialCardView.h
//  iClass
//
//  Created by Richard Ting on 10/30/13.
//  Copyright (c) 2013 alan.tu. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface MaterialCardView : UIView

@property (strong, nonatomic) NSString *contentID;
@property (strong, nonatomic) NSString *contentTitle;
@property (nonatomic, getter = isSelected) BOOL selected;

@end
