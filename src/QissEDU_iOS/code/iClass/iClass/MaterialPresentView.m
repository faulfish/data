//
//  MaterialPresentView.m
//  iClass
//
//  Created by alan.tu on 10/23/13.
//  Copyright (c) 2013 alan.tu. All rights reserved.
//

#import "MaterialPresentView.h"
#import "UIColor+HexString.h"

@implementation MaterialPresentView

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
    [self drawGridLines];
}

- (void) touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event {
    NSLog(@"begin");
}

- (void) touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event {
    
}

- (void) touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event {
    
}

- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event {
    
}

- (void) motionBegan:(UIEventSubtype)motion withEvent:(UIEvent *)event {

    NSLog(@"start");
}

- (void) motionEnded:(UIEventSubtype)motion withEvent:(UIEvent *)event {
    NSLog(@"end");
}

- (void)drawGridLines
{
    CGContextRef context = UIGraphicsGetCurrentContext();
    
    UIColor *color = [UIColor colorWithHexString:@"#C3C3C2"];
    [color set];
    
    // Draw vertical grid lines
    CGContextSaveGState(context);
    
    CGFloat x = 0;
    CGFloat x_spacing = 5;
    CGFloat y = 5;
    CGFloat height = self.bounds.size.height - 5;
    
    x += x_spacing;
    for (int i = 1; i < (5+1); i++) {
        CGContextMoveToPoint(context, x, y);
        CGContextAddLineToPoint(context, x, y+height);
        CGContextStrokePath(context);
        
        x += x_spacing;
    }
	CGContextRestoreGState(context);
    
    // Draw horizontal grid lines - solid lines
    CGContextSaveGState(context);
    
    CGContextMoveToPoint(context, 0, 5 + 5);
    CGContextAddLineToPoint(context, self.bounds.size.width, 5 + 5);
    CGContextStrokePath(context);
}

@end
