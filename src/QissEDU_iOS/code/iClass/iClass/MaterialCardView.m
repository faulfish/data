//
//  MaterialCardView.m
//  iClass
//
//  Created by Richard Ting on 10/30/13.
//  Copyright (c) 2013 alan.tu. All rights reserved.
//

#import "MaterialCardView.h"

@interface MaterialCardView()

@end

@implementation MaterialCardView

- (id)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
        // Initialization code
    }
    return self;
}

- (void)setSelected:(BOOL)selected
{
    _selected = selected;
    if (_selected) {
        self.backgroundColor = [[UIColor cyanColor] colorWithAlphaComponent:0.3];
    } else {
        self.backgroundColor = [UIColor clearColor];
    }
}

// Only override drawRect: if you perform custom drawing.
// An empty implementation adversely affects performance during animation.
- (void)drawRect:(CGRect)rect
{
    // Drawing code
    NSArray *subviews = self.subviews;
    if ([subviews count] > 0) {
        for (UIView *view in subviews) {
            [view removeFromSuperview];
        }
    }

    UIImageView *imageView = [[UIImageView alloc] initWithFrame:CGRectMake(29, 6, 162, 121)];
    imageView.image = [self coverImageForContentID:self.contentID];
    [self addSubview:imageView];

    UILabel *nameLabel = [[UILabel alloc] initWithFrame:CGRectMake(29, 129, 162, 21)];
    nameLabel.textAlignment = NSTextAlignmentCenter;
    nameLabel.backgroundColor = [UIColor clearColor];
    nameLabel.font = [UIFont boldSystemFontOfSize:15];
    nameLabel.textColor = [UIColor whiteColor];
    nameLabel.text = self.contentTitle;
    [self addSubview:nameLabel];
}

- (UIImage *)coverImageForContentID:(NSString *)contentID
{
    UIImage *image;
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *documentPath = ([paths count] > 0) ? [[paths objectAtIndex:0] description]: nil;
    NSString *docsDir = [documentPath stringByAppendingPathComponent:[NSString stringWithFormat:@"zip/%@", self.contentID]];
    NSArray* files = [[NSFileManager defaultManager] contentsOfDirectoryAtPath:docsDir error:nil];
    if ([files count] > 0) {
        NSString *file = [files objectAtIndex:0];
        NSString *filepath = [docsDir stringByAppendingPathComponent:file];
        image = [[UIImage alloc] initWithContentsOfFile:filepath];
    }
    return image;
}

@end
