//
//  EdgeSwipeGestureRecognizer.h
//  iClass
//
//  Created by alan.tu on 8/8/13.
//  Copyright (c) 2013 alan.tu. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface EdgeSwipeGestureRecognizer : UIGestureRecognizer

enum {
    EdgeSwipeGestureRecognizerLeft,
    EdgeSwipeGestureRecognizerRight
};

@property int direction;

- (id) initWithTarget1:(id)target action:(SEL)action;
- (void) _action:(EdgeSwipeGestureRecognizer*)recognizer ;

@end
