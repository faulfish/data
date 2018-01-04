//
//  ThumbnailView.m
//  iClass
//
//  Created by alan.tu on 8/27/13.
//  Copyright (c) 2013 alan.tu. All rights reserved.
//

#import "ThumbnailView.h"
#import "ThumbnailCell.h"

@implementation ThumbnailView

- (id)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
        // Initialization code
    }
    return self;
}

/*
// Only override drawRect: if you perform custom drawing.
// An empty implementation adversely affects performance during animation.
- (void)drawRect:(CGRect)rect
{
    // Drawing code
}
*/

- (UICollectionViewCell *)collectionView:(UICollectionView *)cv cellForItemAtIndexPath:(NSIndexPath *)indexPath {
    ThumbnailCell *cell = [cv dequeueReusableCellWithReuseIdentifier:@"ThumbnailCell" forIndexPath:indexPath];
    //NSString *searchTerm = self.searches[indexPath.section];
    //cell.photo = self.searchResults[searchTerm]
    //[indexPath.row];
    return cell;
}

@end
