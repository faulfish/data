//
//  Shape_UDP_Receiver.m
//  iClass
//
//  Created by alan.tu on 10/21/13.
//  Copyright (c) 2013 alan.tu. All rights reserved.
//

#import "Shape_UDP_Receiver.h"

@interface Shape_UDP_Receiver() {
    //NSString* groupIP;// = "230.0.0.1";
    //NSInteger groupPort;// = 4445;
    //NSInteger socketPort;// = 4446;
    GCDAsyncUdpSocket *udpSocket;
}

@end

@implementation Shape_UDP_Receiver

- (id) initWithSocket : (NSString*) ip port:(NSInteger) port {
    self = [super init];
    
    NSError *error = nil;
    //m_receiver = new UDPMulticastReceiver(groupIP, groupPort);
    
    udpSocket = [[GCDAsyncUdpSocket alloc]initWithDelegate:self delegateQueue:dispatch_get_main_queue()];
    if (![udpSocket bindToPort:port error:&error])
    {
        NSLog(@"Error bindToPort: %@", [error description]);
    }
    if (![udpSocket joinMulticastGroup:ip error:&error])
    {
        NSLog(@"Error joinMulticastGroup: %@", [error description]);
    }
    if (![udpSocket beginReceiving:&error])
    {
        NSLog(@"Error receiving: %@", [error description]);
    }
    [udpSocket enableBroadcast:YES error:&error];
    if (error != nil)
    {
        NSLog(@"Error enableing broadcast: %@", [error description]);
    }
    
    NSLog(@"Socket Created");
    
    if (delegates == nil) {
        delegates = [[NSMutableArray alloc] init];
    }
    
    return self;
}

- (void) registerDelegate:(id)delegate {
    if (delegates == nil) {
        delegates = [[NSMutableArray alloc] init];
    }
    [delegates removeObject:delegates];
    [delegates addObject:delegate];
}

- (void) unregisterDelegate:(id)delegate {
    [delegates removeObject:delegates];
}

- (void) writeShort:(int) val target:(NSMutableData*) data
{
    Byte buffer[2] = {0};
    buffer[0] = (Byte)(val >> 8);
    buffer[1] = (Byte)(val);
    NSData *shortData = [[NSData alloc]initWithBytes:buffer length:2];
    [data appendData:shortData];
    NSLog(@"%@", data);
}

- (short) readShort:(NSMutableData*) data
{
    NSData *data2 = [data subdataWithRange:NSMakeRange(0, 2)];
    short len = CFSwapInt16BigToHost(*(short*)([data2 bytes]));
    return len;
}

-(void) writeUTF:(NSString *)str target:(NSMutableData *)data
{
    unichar buffer[1024] = {0};
    [str getCharacters:buffer range:NSMakeRange(0,[str length])];
    int lengthbyte = [str length];
    [self writeShort:lengthbyte target:data];
    NSData* strdata = [str dataUsingEncoding:NSUTF8StringEncoding];
    [data appendData:strdata];
    NSLog(@"%@", data);
}

-(NSString *) readUTF:(NSMutableData **)data
{
    NSString* str = nil;
    if ([*data length] > 2) {
        NSData *data2 = [*data subdataWithRange:NSMakeRange(0, 2)];
        short len = CFSwapInt16BigToHost(*(short*)([data2 bytes]));
        NSData *dataStr = [*data subdataWithRange:NSMakeRange(2, len)];
        str = [NSString stringWithUTF8String:[dataStr bytes]];
        NSData *dataLeave = [*data subdataWithRange:NSMakeRange(2+len, [*data length] - (2+len))];
        *data = nil;
        *data = [[NSMutableData alloc] initWithData:dataLeave];
    }
    
    return str;
}


/**
 * Called when the datagram with the given tag has been sent.
 **/
- (void)onUdpSocket:(AsyncUdpSocket *)sock didSendDataWithTag:(long)tag {
    NSLog(@"%s",__PRETTY_FUNCTION__);
}

/**
 * Called if an error occurs while trying to send a datagram.
 * This could be due to a timeout, or something more serious such as the data being too large to fit in a sigle packet.
 **/
- (void)onUdpSocket:(AsyncUdpSocket *)sock didNotSendDataWithTag:(long)tag dueToError:(NSError *)error {
    NSLog(@"%s",__PRETTY_FUNCTION__);
}

/**
 * Called when the socket has received the requested datagram.
 *
 * Due to the nature of UDP, you may occasionally receive undesired packets.
 * These may be rogue UDP packets from unknown hosts,
 * or they may be delayed packets arriving after retransmissions have already occurred.
 * It's important these packets are properly ignored, while not interfering with the flow of your implementation.
 * As an aid, this delegate method has a boolean return value.
 * If you ever need to ignore a received packet, simply return NO,
 * and AsyncUdpSocket will continue as if the packet never arrived.
 * That is, the original receive request will still be queued, and will still timeout as usual if a timeout was set.
 * For example, say you requested to receive data, and you set a timeout of 500 milliseconds, using a tag of 15.
 * If rogue data arrives after 250 milliseconds, this delegate method would be invoked, and you could simply return NO.
 * If the expected data then arrives within the next 250 milliseconds,
 * this delegate method will be invoked, with a tag of 15, just as if the rogue data never appeared.
 *
 * Under normal circumstances, you simply return YES from this method.
 **/
- (BOOL)onUdpSocket:(AsyncUdpSocket *)sock
     didReceiveData:(NSData *)data
            withTag:(long)tag
           fromHost:(NSString *)host
               port:(UInt16)port {
    NSLog(@"%s",__PRETTY_FUNCTION__);
    
    return NO;
}

/**
 * Called if an error occurs while trying to receive a requested datagram.
 * This is generally due to a timeout, but could potentially be something else if some kind of OS error occurred.
 **/
- (void)onUdpSocket:(AsyncUdpSocket *)sock didNotReceiveDataWithTag:(long)tag dueToError:(NSError *)error {
    NSLog(@"%s",__PRETTY_FUNCTION__);
}

/**
 * Called when the socket is closed.
 * A socket is only closed if you explicitly call one of the close methods.
 **/
- (void)onUdpSocketDidClose:(AsyncUdpSocket *)sock {
    NSLog(@"%s",__PRETTY_FUNCTION__);
}

/////////////
/**
 * By design, UDP is a connectionless protocol, and connecting is not needed.
 * However, you may optionally choose to connect to a particular host for reasons
 * outlined in the documentation for the various connect methods listed above.
 *
 * This method is called if one of the connect methods are invoked, and the connection is successful.
 **/
- (void)udpSocket:(GCDAsyncUdpSocket *)sock didConnectToAddress:(NSData *)address{
    NSLog(@"%s",__PRETTY_FUNCTION__);
}

/**
 * By design, UDP is a connectionless protocol, and connecting is not needed.
 * However, you may optionally choose to connect to a particular host for reasons
 * outlined in the documentation for the various connect methods listed above.
 *
 * This method is called if one of the connect methods are invoked, and the connection fails.
 * This may happen, for example, if a domain name is given for the host and the domain name is unable to be resolved.
 **/
- (void)udpSocket:(GCDAsyncUdpSocket *)sock didNotConnect:(NSError *)error{
    NSLog(@"%s",__PRETTY_FUNCTION__);
}

/**
 * Called when the datagram with the given tag has been sent.
 **/
- (void)udpSocket:(GCDAsyncUdpSocket *)sock didSendDataWithTag:(long)tag{
    NSLog(@"%s",__PRETTY_FUNCTION__);
}

/**
 * Called if an error occurs while trying to send a datagram.
 * This could be due to a timeout, or something more serious such as the data being too large to fit in a sigle packet.
 **/
- (void)udpSocket:(GCDAsyncUdpSocket *)sock didNotSendDataWithTag:(long)tag dueToError:(NSError *)error{
    NSLog(@"%s",__PRETTY_FUNCTION__);
}

/**
 * Called when the socket has received the requested datagram.
 **/
- (void)udpSocket:(GCDAsyncUdpSocket *)sock didReceiveData:(NSData *)data fromAddress:(NSData *)address withFilterContext:(id)filterContext{
    NSLog(@"%s",__PRETTY_FUNCTION__);
    
    NSLog(@"%@",data);
    
    NSData *data2 = [data subdataWithRange:NSMakeRange(0, 2)];
    short pageid = *(short*)([data2 bytes]);

    for (id delegate in delegates) {
        [delegate eventChanegePage:pageid];
    }
}

/**
 * Called when the socket is closed.
 **/
- (void)udpSocketDidClose:(GCDAsyncUdpSocket *)sock withError:(NSError *)error{
    NSLog(@"%s",__PRETTY_FUNCTION__);
}

@end
