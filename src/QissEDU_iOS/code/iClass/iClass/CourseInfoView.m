//
//  CourseInfoView.m
//  iClass
//
//  Created by alan.tu on 8/8/13.
//  Copyright (c) 2013 alan.tu. All rights reserved.
//

#import "CourseInfoView.h"

@implementation CourseInfoView

- (id) init {
    
    NSLog(@"%s",__PRETTY_FUNCTION__);
    self = [super init];
    return self;
}

- (id)initWithFrame:(CGRect)frame
{
    NSLog(@"%s",__PRETTY_FUNCTION__);
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

- (void) didMoveToSuperview {
    NSLog(@"%s",__PRETTY_FUNCTION__);
}

- (void) didAddSubview:(UIView *)subview {
    NSLog(@"%s",__PRETTY_FUNCTION__);
}

- (void) didChange:(NSKeyValueChange)changeKind valuesAtIndexes:(NSIndexSet *)indexes forKey:(NSString *)key {
    NSLog(@"%s",__PRETTY_FUNCTION__);
}

- (void) didMoveToWindow {
    NSLog(@"%s",__PRETTY_FUNCTION__);
    
    if ([self respondsToSelector: @selector(edgeswipe:)]) {
        EdgeSwipeGestureRecognizer *gesture = [[EdgeSwipeGestureRecognizer alloc] initWithTarget:self action:@selector(edgeswipe:)];
        [self addGestureRecognizer:gesture];
    }
}

- (void) edgeswipe:(EdgeSwipeGestureRecognizer *) recognizer {
    NSLog(@"...%s",__PRETTY_FUNCTION__);
}

@end
