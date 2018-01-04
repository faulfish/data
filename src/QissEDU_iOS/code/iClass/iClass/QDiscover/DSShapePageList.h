//
//  DSShapePageList.h
//  iClass
//
//  Created by alan.tu on 10/28/13.
//  Copyright (c) 2013 alan.tu. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface DSShapePageList : NSObject {
    short pageid;
}

- (void) setPageID : (short) _pageid;

//protocol
- (void) readExternal : (NSMutableArray*) data;
- (void) writeExternal : (NSMutableArray*) data;

@end
