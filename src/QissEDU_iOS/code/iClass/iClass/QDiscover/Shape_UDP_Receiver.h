//
//  Shape_UDP_Receiver.h
//  iClass
//
//  Created by alan.tu on 10/21/13.
//  Copyright (c) 2013 alan.tu. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "AsyncUdpSocket.h"
#import "GCDAsyncUdpSocket.h"

@protocol ShapeDelegate

@optional
- (void) eventChanegePage : (NSInteger) pageID ;

@end

@interface Shape_UDP_Receiver : NSObject<GCDAsyncUdpSocketDelegate> {
    NSMutableArray *delegates;
}

- (id) initWithSocket : (NSString*) ip port:(NSInteger) port;
- (void) registerDelegate : (id) delegate ;
- (void) unregisterDelegate : (id) delegate;

@end
