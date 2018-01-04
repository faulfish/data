//
//  DSShapeManager.h
//  iClass
//
//  Created by alan.tu on 10/28/13.
//  Copyright (c) 2013 alan.tu. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "Shape_UDP_Receiver.h"
#import "Shape_UDP_Sender.h"

@interface DSShapeManager : NSObject

- (id) initWithRole : (NSInteger) role;
- (void) registerDelegate : (id) delegate;
- (void) unregisterDelegate : (id) delegate;
- (void) setPageID : (short) pageid;

@end
