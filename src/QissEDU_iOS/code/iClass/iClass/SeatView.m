//
//  SeatView.m
//  iClass
//
//  Created by Richard Ting on 9/17/13.
//  Copyright (c) 2013 alan.tu. All rights reserved.
//

#import "SeatView.h"

@interface SeatView()

@end

@implementation SeatView

- (id)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
        // Initialization code
    }
    return self;
}

// Only override drawRect: if you perform custom drawing.
// An empty implementation adversely affects performance during animation.
- (void)drawRect:(CGRect)rect
{
    // Drawing code
    [[UIImage imageNamed:@"bk_photo_frame_white.png"] drawInRect:self.bounds];
    [[UIImage imageNamed:@"photo.png"] drawAtPoint:CGPointMake(8, 8)];
    UILabel *nameLabel = [[UILabel alloc] initWithFrame:CGRectMake(8, 96, 128, 28)];
    [nameLabel setTextAlignment:NSTextAlignmentCenter];
    [nameLabel setBackgroundColor:[UIColor clearColor]];
    nameLabel.text = self.studentName;
    [self addSubview:nameLabel];
}

@end
