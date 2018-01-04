//
//  NSURLConnectionEx.m
//  iClass
//
//  Created by alan.tu on 8/7/13.
//  Copyright (c) 2013 alan.tu. All rights reserved.
//

#import "NSURLConnectionEx.h"

static NSMutableDictionary *_cache;
static NSString *backedName = @"NSUserDefaultsBackup.plist";

@interface NSURLConnectionEx()
{
}

@end

@implementation NSURLConnectionEx

+ (NSData*) sendSynchronousRequest:(NSURLRequest *)request returningResponse:(NSURLResponse *__autoreleasing *)response error:(NSError *__autoreleasing *)error {
    
    if(_cache == nil) {
        _cache = [NSMutableDictionary new];
    }
    
    NSData *_data = [_cache objectForKey:[[request URL]absoluteString]];
    if(_data == nil) {
        _data = [super sendSynchronousRequest:request returningResponse:response error:error];
        [_cache setObject:_data forKey:[[request URL]absoluteString]];
    }

    return _data;
}

+ (void) flush {
    if(_cache != nil) {
        [_cache removeAllObjects];
    }
}

+ (void) save {
    
    if( _cache != nil) {
        //Get the user documents directory
        NSString *documentsDirectory = [NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES) lastObject];
        //Create a path to save the details
        NSString *backedUpUserDefaultsPath = [documentsDirectory stringByAppendingPathComponent:backedName];
        //The easiest thing to do here is just write it to a file
        [_cache writeToFile:backedUpUserDefaultsPath atomically:YES];
        
        //Create and open a stream
        NSOutputStream *outputStream = [[NSOutputStream alloc] initToFileAtPath:backedUpUserDefaultsPath append:NO];
        [outputStream scheduleInRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
        //outputStream.delegate = self; //you'll want to close, and potentially dealloc your stream in the delegate callback
        [outputStream open];
        
        //write that to the stream!
        [NSPropertyListSerialization writePropertyList:_cache
                                              toStream:outputStream
                                                format:NSPropertyListXMLFormat_v1_0
                                               options:NSPropertyListImmutable
                                                 error:nil];
        
        [outputStream close];
    }
}

+ (void) load {
    
    if(_cache == nil) {
        _cache = [NSMutableDictionary new];
    }
    
    if (_cache != nil) {
        
        //Get the user documents directory
        NSString *documentsDirectory = [NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES) lastObject];
        //Create a path to save the details
        NSString *backedUpUserDefaultsPath = [documentsDirectory stringByAppendingPathComponent:backedName];
        
        BOOL fileExists = [[NSFileManager defaultManager] fileExistsAtPath:backedUpUserDefaultsPath];
        if(fileExists) {
            _cache = [NSDictionary dictionaryWithContentsOfFile:backedUpUserDefaultsPath];
        }
        
    }
}

+ (void) unittest {
    
    if(_cache == nil) {
        _cache = [NSMutableDictionary new];
    }
    
    for ( id key in _cache )
    {
        NSLog( @"%@: %@", key, [_cache objectForKey:key] );
    }
}

@end
