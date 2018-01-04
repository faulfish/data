//
//  QDiscover.h
//  iClass
//
//  Created by alan.tu on 9/12/13.
//  Copyright (c) 2013 alan.tu. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "AsyncUdpSocket.h"
#import "GCDAsyncUdpSocket.h"

/*
 public class QDiscoveryServiceInfo {
 public String uuid;
 public String serviceName;
 public String friendlyName;
 
 public String ip;
 public long lastHelloTime = 0;
 }
 */
@interface QDiscoverServiceInfo : NSObject<GCDAsyncUdpSocketDelegate>

@property NSString* uuid;
@property NSString* serviceName;
@property NSString* friendlyName;
@property NSString* ip;
@property NSDate  * lastHelloTime;

@end

@interface QDiscover : NSObject

@property NSMutableDictionary* services;
@property NSTimer*  timer;
@property NSInteger role;

- (id) initWithRole: (NSInteger)rolePara;

//addService(String uuid, QDiscoveryServiceInfo info)
- (void) addService: (NSString *) uuid info:(QDiscoverServiceInfo*) info  error: (NSError **) error;

//public void removeService(String uuid)
- (void) removeService: (NSString*) uuid error: (NSError**) error;

//check Server
- (void) checkServer;

@end
