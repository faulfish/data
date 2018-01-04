
//  MaterialInfoAPI.h
//  iClassAPI
//
//  Created by admin on 13/8/19.
//  Copyright (c) 2013年 admin. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "HTTPUtils.h"

@interface MaterialInfoAPI : NSObject<ASIHTTPRequestDelegate>
{
    HTTPUtils *utils;
    NSURL *url;
    ASIFormDataRequest *request;
    NSString *responseString;
}

@property (strong, nonatomic) HTTPUtils *utils;
@property (strong, nonatomic) NSURL *url;
@property (strong, nonatomic) ASIFormDataRequest *request;
@property (strong, nonatomic) NSString *responseString;

- (NSString *) getAllMaterialToClassByTeacherID: (NSString *)teacherID error: (NSError **) error;
- (NSString *) getAllMaterialToClassByTeacherIDAndDateTime: (NSString *) teacherID
                                                      date:(NSString *) dateTime
                                                     error: (NSError **) error;
- (NSString *) getAllMaterialToClass: (NSString *)teacherID
                          materialID:(NSString *) materialID
                           className: (NSString *) className
                               error: (NSError **) error;
- (NSString *) getAllMaterialToClassByMaterialID: (NSString *) materialID
                                           error: (NSError **) error;
- (NSString *) getAllMaterialToClassByClassName: (NSString *) className
                                           date:(NSString *) dateTime
                                          error: (NSError **) error;

// For student to get a specific class timetable of the week
- (NSString *) getAllCourseToTeachingMaterialByDateAndClass: (NSString *) className
                                                       date: (NSString *)date
                                                      error: (NSError **) error;

- (NSString *) getCourseSchedule: (NSString *)userID
                           error: (NSError **) error;

- (NSString *) getCourseMaterial: (NSString *) courseID
                            date: (NSString *) date
                           error: (NSError **) error;
@end