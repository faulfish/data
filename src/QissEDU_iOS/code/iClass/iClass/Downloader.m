//
//  Downloader.m
//  iClass
//
//  Created by alan.tu on 8/28/13.
//  Copyright (c) 2013 alan.tu. All rights reserved.
//

#import "Downloader.h"
#import "NSURLConnectionEx.h"

@interface Downloader ()

@property (nonatomic, strong, readwrite) NSData *data;
@property (nonatomic, strong, readwrite) NSMutableData *receiverData;
@property (nonatomic, strong) DownloaderCompletionBlock completion;

@end

@implementation Downloader

@synthesize data = _data;
@synthesize receiverData = _receiverData;
@synthesize completion = _completion;

- (void) download:(NSURL *)url complection:(DownloaderCompletionBlock)completion {
    if(url) {
        [self setCompletion:completion];
        [self setReceiverData:[[NSMutableData alloc]init]];
         NSURLRequest *request = [NSURLRequest requestWithURL:url];
         NSURLConnectionEx *connection = [[NSURLConnectionEx alloc] initWithRequest:request delegate:self startImmediately:NO];
         [connection scheduleInRunLoop:[NSRunLoop currentRunLoop] forMode:NSRunLoopCommonModes];
         [connection start];
    }
}
       
#pragma mark - NSURLConnection delegate method
         
         - (void) connection:(NSURLConnection*) connection didReceiveData:(NSData *)data {
             [[self receiverData] appendData:data];
         }
         
         - (void) connection:(NSURLConnection*) connection didReceiveResponse:(NSURLResponse *)response {
             [[self receiverData] setLength:0];
         }
         
         - (void) connectionDidFinishLoading: (NSURLConnection* )connection {
             [self setData:[self receiverData]];
             [self setReceiverData:nil];
             
             DownloaderCompletionBlock complection = [self completion];
             complection(_data,nil);
         }
         
         - (void) connection: (NSURLConnection *) connection didFailWithError:(NSError *)error {
             [self setReceiverData:nil];
             
             DownloaderCompletionBlock complection = [self completion];
             complection(nil,error);
         }

@end
