//
//  EdgeSwipeGestureRecognizer.m
//  iClass
//
//  Created by alan.tu on 8/8/13.
//  Copyright (c) 2013 alan.tu. All rights reserved.
//

#import "EdgeSwipeGestureRecognizer.h"
#import <UIKit/UIGestureRecognizerSubclass.h>

@interface EdgeSwipeGestureRecognizer() {
    id  _target;
    SEL _actionFromInit;
    CGPoint _startPoint;
    BOOL _enable;
    int _edge;
    int _swipeLength;
}

@end

@implementation EdgeSwipeGestureRecognizer

@synthesize direction = _direction;

- (id) initWithTarget1:(id)target action:(SEL)action {
    
    self = [super initWithTarget:self action:@selector(_action:)];
    _target = target;
    _actionFromInit = action;
    return self;
}

- (void) _action:(EdgeSwipeGestureRecognizer*)recognizer {
    
    if ([_target respondsToSelector:_actionFromInit]) {
        NSLog (@"Calling '%@'", NSStringFromSelector( _actionFromInit ));
        [_target performSelector:_actionFromInit withObject:recognizer];
    }
}

- (void) touchesBegan:(NSSet *)touchs withEvent:(UIEvent *)event {
    
    if([[event touchesForGestureRecognizer:self] count] > 1) {
        [self setState:UIGestureRecognizerStateFailed];
    }
    
    _edge = 10;
    _swipeLength = 100;
    _enable = NO;
    UITouch *touch = [touchs anyObject];
    UIView *view = [self view];
    CGPoint currentPoint = [touch locationInView:view];
    CGFloat width = CGRectGetWidth([view bounds]);
    
    NSLog(@"start %f %f",currentPoint.x,currentPoint.y);
    if((currentPoint.x > _edge) && (currentPoint.x < (width - _edge))){
        [self setState:UIGestureRecognizerStateFailed];
    }
    _startPoint = currentPoint;
}

- (void) touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event {
    
    //NSLog(@"end %f %f",currentPoint,currentPoint.y);
    if(_enable == YES) {
        [self setState:UIGestureRecognizerStateEnded];
    }
}

- (void) touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event {
    
    UITouch *touch = [touches anyObject];
    UIView *view = [self view];
    CGPoint currentPoint = [touch locationInView:view];
    CGFloat width = CGRectGetWidth([view bounds]);
    
    //NSLog(@"Moved %f %f",currentPoint,currentPoint.y);
    if(_enable == YES) {
        [self setState:UIGestureRecognizerStateChanged];
    }
    else if(_startPoint.x < _edge) {
        _direction = EdgeSwipeGestureRecognizerLeft;
        if(_enable == NO && (currentPoint.x - _startPoint.x) > _swipeLength) {
            [self setState:UIGestureRecognizerStateChanged];
            _enable = YES;
        }
    }
    else if(_startPoint.x > (width - _edge)) {
        _direction = EdgeSwipeGestureRecognizerRight;
        if(_enable == NO && (_startPoint.x - currentPoint.x) > _swipeLength) {
            [self setState:UIGestureRecognizerStateChanged];
            _enable = YES;
        }
    }

}

- (void) touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event {
    
    NSLog(@"%s",__PRETTY_FUNCTION__);
    [self setState:UIGestureRecognizerStateCancelled];
}

@end
