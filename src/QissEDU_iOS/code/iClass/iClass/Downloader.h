//
//  Downloader.h
//  iClass
//
//  Created by alan.tu on 8/28/13.
//  Copyright (c) 2013 alan.tu. All rights reserved.
//

#import <Foundation/Foundation.h>

typedef void(^DownloaderCompletionBlock) (NSData* data,NSError *error);

@interface Downloader : NSObject

@property (nonatomic, strong, readonly) NSData *data;

- (void) download:(NSURL*) url complection:(DownloaderCompletionBlock)completion;

@end
