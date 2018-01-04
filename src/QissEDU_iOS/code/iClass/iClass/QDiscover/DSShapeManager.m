//
//  DSShapeManager.m
//  iClass
//
//  Created by alan.tu on 10/28/13.
//  Copyright (c) 2013 alan.tu. All rights reserved.
//

#import "DSShapeManager.h"
#import "DSShapePageList.h"
#import "Shape_UDP_Sender.h"
#import "Shape_UDP_Receiver.h"

NSString* MULTICAST_IP_SHAPE = @"230.0.0.1";
const NSInteger MULTICAST_PORT_SHAPE = 4446;
const NSUInteger MULTICAST_RECEIVER_SOCKET_PORT_SHAPE = 4445;


@interface DSShapeManager()
{
};

@property DSShapePageList *pagelist;
@property Shape_UDP_Receiver *shapereceiver;
@property Shape_UDP_Sender *shapesender;
@property NSMutableData* data;
@property (nonatomic) NSUInteger role;

@end

@implementation DSShapeManager

@synthesize shapereceiver = _shapereceiver;
@synthesize shapesender = _shapesender;
@synthesize pagelist = _pagelist;
@synthesize data = _data;
@synthesize role = _role;

- (id) initWithRole : (NSInteger) role {
    
    self = [super init];
    _role = role;
    
    if (_shapesender == nil) {
        _shapesender = [[Shape_UDP_Sender alloc] initWithSocket:MULTICAST_PORT_SHAPE targetip:MULTICAST_IP_SHAPE targetport:MULTICAST_RECEIVER_SOCKET_PORT_SHAPE];
    }
    if (_shapereceiver == nil) {
        _shapereceiver = [[Shape_UDP_Receiver alloc] initWithSocket:MULTICAST_IP_SHAPE port:MULTICAST_RECEIVER_SOCKET_PORT_SHAPE];
    }
    if(_data == nil) {
        _data = [[NSMutableData alloc] init];
        NSLog(@"DSShapeManager:Datainit");
    }
    return self;
}

- (void) registerDelegate:(id)delegate {
    if (_shapereceiver != nil) {
        [_shapereceiver registerDelegate:delegate];
    }
}

- (void) unregisterDelegate:(id)delegate {
    if (_shapereceiver != nil) {
        [_shapereceiver unregisterDelegate:delegate];
    }
}

- (void) setPageID : (short) pageid {
    if (_data != nil) {
        [_data appendBytes:&pageid length:sizeof(short)];
        NSLog(@"%@ %d",_data,sizeof(short));
        [self send];
    }
}

- (void) send {
    if (_shapesender != nil) {
        NSLog(@"%@",_data);
        [_shapesender send:_data];
        [_data setLength:0];
    }
}

@end
