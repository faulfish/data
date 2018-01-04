//
//  QDiscover.m
//  iClass
//
//  Created by alan.tu on 9/12/13.
//  Copyright (c) 2013 alan.tu. All rights reserved.
//

#import "QDiscover.h"
#import <ifaddrs.h>
#import <arpa/inet.h>

@implementation QDiscoverServiceInfo

@synthesize uuid = _uuid;
@synthesize serviceName = _serviceName;
@synthesize friendlyName = _friendlyName;
@synthesize ip = _ip;
@synthesize lastHelloTime = _lastHelloTime;

@end

@interface QDiscover() {
    NSString* groupIP;// = "230.0.0.1";
    NSInteger groupPort;// = 8888;
    NSInteger socketPort;// = 8887;
    NSInteger MULTICAST_REPEAT_COUNT;// = 4;
    NSInteger FREQUENCY_HELLO;// = 1000 * 10;
    NSInteger TIMEOUT_HELLO;// = 1000 * 30;
    //AsyncUdpSocket* socket;
    GCDAsyncUdpSocket *udpSocket;
}

@end

@implementation QDiscover

@synthesize services = _services;
@synthesize timer = _timer;
@synthesize role = _role;

- (id) initWithRole:(NSInteger)rolePara {
    self = [super init];
    groupIP = @"230.0.0.1";
    groupPort = 8888;
    socketPort = 8887;
    MULTICAST_REPEAT_COUNT = 4;
    FREQUENCY_HELLO = 10;//seconds
    TIMEOUT_HELLO = 30;//seconds
    _role = rolePara;
    
    NSError* error = nil;
    
    udpSocket = [[GCDAsyncUdpSocket alloc]initWithDelegate:self delegateQueue:dispatch_get_main_queue()];
    
    if(_role == 0)
    {//server
        if (![udpSocket bindToPort:socketPort error:&error])
        {
            NSLog(@"Error bindToPort: %@", [error description]);
        }
        NSTimeInterval interval = FREQUENCY_HELLO;
        _timer = [NSTimer scheduledTimerWithTimeInterval:interval target:self selector:@selector(timerFunc) userInfo:nil repeats:YES];
    }
    else if(_role == 1)
    {//client
        if (![udpSocket bindToPort:groupPort error:&error])
        {
            NSLog(@"Error bindToPort: %@", [error description]);
        }
        if (![udpSocket joinMulticastGroup:groupIP error:&error])
        {
            NSLog(@"Error joinMulticastGroup: %@", [error description]);
        }
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
    
    return self;
}

- (void) addService:(NSString *)uuid info:(QDiscoverServiceInfo *)info error:(NSError *__autoreleasing *)error {
    
    if(_services == nil) {
        _services = [NSMutableDictionary new];
    }
    
    QDiscoverServiceInfo *_info = [_services objectForKey:uuid];
    if(_info == nil) {
        [_services setObject:info forKey:uuid];
    }
}

- (void) removeService:(NSString *)uuid error:(NSError *__autoreleasing *)error {
    
    if(_services == nil) {
        _services = [NSMutableDictionary new];
    }
    
    QDiscoverServiceInfo *_info = [_services objectForKey:uuid];
    if(_info != nil) {
        [_services removeObjectForKey:uuid];
    }
}

- (void) checkServer {
    //[socket receiveWithTimeout:10000 tag:0];
}

- (void) timerFunc {
    
    if(_services == nil) {
        _services = [NSMutableDictionary new];
    }
    
    [self checkServer];
    
    for ( id key in _services ) {
        [self hello:key info:[_services objectForKey:key]];
    }
}

- (void) send: (GCDAsyncUdpSocket*) udpSocket1 target: (NSData*) data {
    NSLog(@"%s",__PRETTY_FUNCTION__);
    
    //NSString *str1 = @"M-SEARCH * HTTP/1.1\r\nHOST: 230.0.0.1:8888\r\nMAN: \"ssdp discover\"\r\nMX: 3\r\nST: ssdp:all\r\nUSER-AGENT: iOS UPnP/1.1 TestApp/1.0\r\n\r\n";
    //NSData *data = [str1 dataUsingEncoding:NSUTF8StringEncoding];
    //[udpSocket1 sendData:data toHost:groupIP port:groupPort withTimeout:-1 tag:0];
    
    NSLog(@"Sent Data");
    
    //NSData* data = [str dataUsingEncoding:NSUTF8StringEncoding];
    [udpSocket1 sendData:data toHost:groupIP port:groupPort withTimeout:-1 tag:0];
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
    Byte buffer[2] = {0};
    [data getBytes:buffer length:2];
    //data = [data ]
    //m_in.Read(m_tmpbuf, 0, 2);
    //return (short)((m_tmpbuf[1] & 0xFF) |
      //             (m_tmpbuf[0] << 8));
    
    return 0;
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

- (void) hello:(NSString*) uuid info:(QDiscoverServiceInfo*) info {
    
    //const Byte zerobyte = 0x0;
    NSMutableData *data = [NSMutableData new];
    [self writeUTF:@"ssdp:alive" target:data];
    [self writeUTF:info.uuid target:data];
    [self writeUTF:info.serviceName target:data];
    [self writeUTF:info.friendlyName target:data];
    
    for (NSInteger i =0; i < MULTICAST_REPEAT_COUNT;i++) {
        [self send:udpSocket target:data];
    }
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
    
    /*
     oos.writeUTF("ssdp:alive");
     oos.writeUTF(uuid);
     oos.writeUTF(info.serviceName);
     oos.writeUTF(info.friendlyName);
     */
    /*
     oos.writeUTF("ssdp:byebye");
     oos.writeUTF(uuid);
     oos.writeUTF(info.serviceName);
     */
    /*
     \x0A....ssdp:alive
     \x24....4ab50e50-4276-11e2-a25f-0017cab147c8
     \x11....QisdaDrawSyncHost
     \x12....QDrawSyncHost-47c8
     */
    NSLog(@"%@",data);
    
    QDiscoverServiceInfo *newInfo = [QDiscoverServiceInfo new];
    
    NSMutableData *newData = [[NSMutableData alloc] initWithData:data];
    //parser UFT format
    //{[len][data*len]}*
    
    NSString *NTS = [self readUTF:&newData];
    newInfo.uuid = [self readUTF:&newData];
    NSString *serviceName = [self readUTF:&newData];
    newInfo.serviceName = serviceName;
    
    if ([NTS compare:@"ssdp:alive"] == NSOrderedSame) {
        QDiscoverServiceInfo *_info = [_services objectForKey:newInfo.uuid];
        if (_info == nil) {
            NSString *friendlyName = [self readUTF:&newData];
            newInfo.friendlyName = friendlyName;
            NSError *error = nil;
            
            struct sockaddr_in sockaddr4;
            socklen_t sockaddr4len = sizeof(sockaddr4);
            char buffer[1024] = {0};
            memset(buffer,0,1024);
            [data getBytes:buffer];
            memcpy(&sockaddr4, buffer, sockaddr4len);
            NSLog(@"%zi bytes read from %s:%d...", [data length],
                  inet_ntoa(((struct sockaddr_in*)&sockaddr4)->sin_addr),
                  ((struct sockaddr_in*)&sockaddr4)->sin_port);
            strcpy(buffer, inet_ntoa(((struct sockaddr_in*)&sockaddr4)->sin_addr));
            newInfo.ip = [[NSString alloc]initWithBytes:buffer length:strlen(buffer) encoding:NSStringEncodingConversionAllowLossy];
            
            newInfo.lastHelloTime = [NSDate date];
            [self addService:newInfo.uuid info:newInfo error:&error];
        }
    }
    else if ([NTS compare:@"ssdp:byebye"] == NSOrderedSame) {
        NSError *error = nil;
        [self removeService:newInfo.uuid error:&error];
    }
}

/**
 * Called when the socket is closed.
 **/
- (void)udpSocketDidClose:(GCDAsyncUdpSocket *)sock withError:(NSError *)error{
    NSLog(@"%s",__PRETTY_FUNCTION__);
}

@end
