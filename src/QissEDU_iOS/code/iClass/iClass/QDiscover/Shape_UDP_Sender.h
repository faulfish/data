//
//  Shape_UDP_Sender.h
//  iClass
//
//  Created by alan.tu on 10/21/13.
//  Copyright (c) 2013 alan.tu. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "AsyncUdpSocket.h"
#import "GCDAsyncUdpSocket.h"

@interface Shape_UDP_Sender : NSObject<GCDAsyncUdpSocketDelegate>

- (id) initWithSocket : (NSInteger) port targetip:(NSString*) targetip targetport:(NSInteger) targetport ;
- (void) send:(NSData*) data;

@end
