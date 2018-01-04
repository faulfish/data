//
//  iClassAPI.m
//  iClassAPI
//
//  Created by admin on 13/8/14.
//  Copyright (c) 2013年 admin. All rights reserved.
//

#import "ClientAgent.h"

static NSMutableDictionary *_cache = nil;
static NSString *backedName = @"NSUserDefaultsBackup.plist";
static NSString *cachePolicyName = @"NSCachePolicy.plist";

@interface ClientAgent() {
    ASICachePolicy cachePolicy;
};

@end

@implementation ClientAgent

@synthesize classAPI = _classAPI;
@synthesize materialAPI = _materialAPI;
@synthesize courseAPI = _courseAPI;
@synthesize attendanceAPI = _attendanceAPI;
@synthesize examAPI = _examAPI;
@synthesize authAPI = _authAPI;

- (AuthAPI *) authAPI
{
    if (!_authAPI) _authAPI = [[AuthAPI alloc] init];
    return _authAPI;
}

- (ClassInfoAPI *)classAPI
{
    if (!_classAPI) _classAPI = [[ClassInfoAPI alloc] init];
    return _classAPI;
}

- (MaterialInfoAPI *)materialAPI
{
    if (!_materialAPI) _materialAPI = [[MaterialInfoAPI alloc] init];
    return _materialAPI;
}

- (CourseInfoAPI *)courseAPI
{
    if (!_courseAPI) _courseAPI = [[CourseInfoAPI alloc] init];
    return _courseAPI;
}

- (AttendanceInfoAPI *)attendanceAPI
{
    if (!_attendanceAPI) _attendanceAPI = [[AttendanceInfoAPI alloc] init];
    return _attendanceAPI;
}

- (ExamInfoAPI *)examAPI
{
    if (!_examAPI)_examAPI = [[ExamInfoAPI alloc] init];
    return _examAPI;
}

- (NSString *) loginByUserAccount: (NSString *) account
                            error: (NSError **)error
{
    NSString *loginInfo = [self.authAPI loginByUserAccount:account error:error];
    return loginInfo;
}

- (NSString *) getAllstudentByClass: (NSString *) className error: (NSError **)error
{
    NSString *studentInfo = [self.classAPI getAllstudentByClass:className error: error];
    return studentInfo;
}

- (NSString *) getClass: (NSString *) teacherID error: (NSError **) error
{
    NSString *classInfo = [self.classAPI getClass:teacherID error: error];
    return classInfo;
}

- (NSString *) getClassByStudentID: (NSString *) studentID error: (NSError **) error
{
    NSString *className = [self.classAPI getClassByStudentID:studentID error: error];
    return className;
}

- (NSString *) getClassOnly: (NSString *) teacherID error: (NSError **) error
{
    NSString *className = [self.classAPI getClassOnly:teacherID error: error];
    return className;
}

- (NSString *) getClassTable: (NSString *) userID error: (NSError **) error
{
    NSString *classData = [self.classAPI getClassTable:userID error: error];
    return classData;
}

- (NSString *) getAllCourseToTeachingMaterialByDateAndClass: (NSString *) className
                                                       date:(NSString *)date
                                                      error: (NSError **) error
{
    NSString *allCourse = [self.materialAPI getAllCourseToTeachingMaterialByDateAndClass:className
                                                                                    date:date
                                                                                   error: error];
    return allCourse;
}

- (NSString *) getAllMaterialToClassByTeacherID: (NSString *)teacherID error: (NSError **) error
{
    NSString *material = [self.materialAPI getAllMaterialToClassByTeacherID:teacherID error: error];
    return material;
}

- (NSString *) getAllMaterialToClassByTeacherIDAndDateTime: (NSString *)teacherID
                                                      date:(NSString *)dateTime
                                                     error: (NSError **) error
{
    NSString *material = [self.materialAPI getAllMaterialToClassByTeacherIDAndDateTime:teacherID
                                                                                  date:dateTime
                                                                                 error: error];
    return material;
}

- (NSString *) getAllMaterialToClass: (NSString *)teacherID
                          materialID:(NSString *)materialID
                           className: (NSString *) className
                               error: (NSError **) error
{
    NSString *material = [self.materialAPI getAllMaterialToClass:teacherID
                                                      materialID:materialID
                                                       className:className
                                                           error: error];
    return material;
}

- (NSString *) getAllMaterialToClassByMaterialID: (NSString *) materialID error: (NSError **) error
{
    NSString *material = [self.materialAPI getAllMaterialToClassByMaterialID:materialID error: error];
    return material;
}

- (NSString *) getAllMaterialToClassByClassName: (NSString *) className
                                           date: (NSString *) dateTime
                                          error: (NSError **) error
{
    NSString *material = [self.materialAPI getAllMaterialToClassByClassName:className date:dateTime error: error];
    return material;
}

- (NSString *) getCourseSchedule: (NSString *)userID error: (NSError **) error
{
    NSString *schedule = [self.materialAPI getCourseSchedule:userID error: error];
    return schedule;
}

- (NSString *) getCourseMaterial: (NSString *) courseID
                            date: (NSString *) date
                           error: (NSError **) error
{
    NSString *material = [self.materialAPI getCourseMaterial: courseID date: date error: error];
    return material;
}

- (NSString *) getCourseAllInfo: (NSString *)classID error: (NSError **) error
{
    NSString *courseInfo = [self.courseAPI getCourseAllInfo:classID error: error];
    return courseInfo;
}

- (NSString *) createSemesterData: (NSString *) year
                     semesterType: (NSString *) type
                semesterStartTime: (NSString *) startTime
                  semesterEndTime: (NSString *) endTime
                            error: (NSError **) error
{
    NSString *result = [self.courseAPI createSemesterData: year
                                             semesterType: type
                                        semesterStartTime: startTime
                                          semesterEndTime: endTime
                                                    error: error];
    return result;
}

- (NSString *)getCourseInfo: (NSString *) courseID date: (NSString *) date error: (NSError **) error
{
    NSString *courseInfo = [self.courseAPI getCourseInfo: courseID date: date error: error];
    return courseInfo;
}

- (NSString *) createAnnouncement: (NSString *) announcement
                        className: (NSString *) className
                         courseID: (NSString *) courseID
                             date: (NSString *) date
                            error: (NSError **) error
{
    NSString *result = [self.courseAPI createAnnouncement: announcement
                                                className: className
                                                 courseID: courseID
                                                     date: date
                                                    error: error];
    return result;
}

- (NSString *) getCourseHistory: (NSString *) date
                      className: (NSString *) className
                     courseName: (NSString *) courseName
                          error: (NSError **) error
{
    NSString *history = [self.courseAPI getCourseHistory: date
                                               className: className
                                              courseName: courseName
                                                   error:error];
    return history;
}

- (NSString *) createAttendanceData: (NSString *) courseID
                          studentID: (NSString *) studentID
                             status: (BOOL) status
                               note: (NSString *) note
                               date: (NSString *) date
                              error: (NSError **) error
{
    NSString *result = [self.attendanceAPI createAttendanceData: courseID
                                                      studentID: studentID
                                                         status: status
                                                           note: note
                                                           date: date
                                                          error: error];
    return result;
}

- (NSString *) getSeatingInfo: (NSString *) courseID
                         date: (NSString *) date
                        error: (NSError **) error
{
    NSString *seatingInfo = [self.attendanceAPI getSeatingInfo: courseID
                                                          date: date
                                                         error: error];
    return seatingInfo;
}

- (NSString *) editAttendanceData: (NSString *) attendanceID
                           status: (BOOL) status
                             note: (NSString *) note
                            error: (NSError **) error
{
    NSString *result = [self.attendanceAPI editAttendanceData: attendanceID
                                                       status: status
                                                         note: note
                                                        error: error];
    return result;
}

- (NSString *) getAttendanceByStudent: (NSString *) courseID
                            studentID: (NSString *) studentID
                                 date: (NSString *) date
                                error: (NSError **) error
{
    NSString *attendanceData = [self.attendanceAPI getAttendanceByStudent: courseID
                                                                studentID: studentID
                                                                     date: date
                                                                    error: error];
    return attendanceData;
}

- (NSString *) getAttendanceHistory: (NSString *) date
                          className: (NSString *) className
                         courseName: (NSString *) courseName
                              error: (NSError **) error
{
    NSString *history = [self.attendanceAPI getAttendanceHistory: date
                                                       className: className
                                                      courseName: courseName
                                                           error: error];
    return history;
}

- (NSString *) editExamComment: (NSString *) examResultID
                       comment: (NSString *) comment
                         error: (NSError **) error
{
    NSString *result = [self.examAPI editExamComment: examResultID
                                             comment: comment
                                               error: error];
    return result;
}

- (NSString *) getExamScoreList: (NSString *) examScheduleID
                          error: (NSError **) error
{
    NSString *scoreList = [self.examAPI getExamScoreList:examScheduleID error: error];
    return scoreList;
}

- (NSString *) submitExam: (NSString *) studentID
           examScheduleID: (NSString *) scheduleID
            studentAnswer: (NSString *) answer
                    error: (NSError **) error
{
    NSString *scoreList = [self.examAPI submitExam:studentID examScheduleID:scheduleID studentAnswer:answer error: error];
    return scoreList;
}

- (NSString *) getStudentExamResult: (NSString *) studentID
                           courseID: (NSString *) courseID
                               date: (NSString *) date
                              error: (NSError **) error
{
    NSString *scoreList = [self.examAPI getStudentExamResult:studentID courseID:courseID date:date error: error];
    return scoreList;
}

- (NSString *) getExamAnswer: (NSString *) examID
                       error: (NSError **) error
{
    NSString *examAnswerList = [self.examAPI getExamAnswer:examID error: error];
    return examAnswerList;
}


- (void) flush {
    _cache = nil;//clear the previous
    
    if(_cache == nil) {
        _cache = [NSMutableDictionary new];
    }
    
    [self save];
}

- (void) save {
    
    if( _cache != nil) {
        [_cache setObject:@"withoutempty" forKey:@"withoutempty"];//without empty when save
        
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

- (void) save:(ASICachePolicy) policy {
    
    NSMutableDictionary *_cachePolicy = [NSMutableDictionary new];
    if( _cachePolicy != nil) {
        NSData* data = [[NSData alloc] initWithBytes:&policy length:sizeof(ASICachePolicy)];
        [_cachePolicy setObject:data forKey:@"_cachePolicy"];
        
        //Get the user documents directory
        NSString *documentsDirectory = [NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES) lastObject];
        //Create a path to save the details
        NSString *backedUpUserDefaultsPath = [documentsDirectory stringByAppendingPathComponent:cachePolicyName];
        //The easiest thing to do here is just write it to a file
        [_cache writeToFile:backedUpUserDefaultsPath atomically:YES];
        
        //Create and open a stream
        NSOutputStream *outputStream = [[NSOutputStream alloc] initToFileAtPath:backedUpUserDefaultsPath append:NO];
        [outputStream scheduleInRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
        //outputStream.delegate = self; //you'll want to close, and potentially dealloc your stream in the delegate callback
        [outputStream open];
        
        //write that to the stream!
        [NSPropertyListSerialization writePropertyList:_cachePolicy
                                              toStream:outputStream
                                                format:NSPropertyListXMLFormat_v1_0
                                               options:NSPropertyListImmutable
                                                 error:nil];
        
        [outputStream close];
    }
}

- (void) load {
    
    //Get the user documents directory
    NSString *documentsDirectory = [NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES) lastObject];
    //Create a path to save the details
    NSString *backedUpUserDefaultsPath = [documentsDirectory stringByAppendingPathComponent:backedName];
    
    BOOL fileExists = [[NSFileManager defaultManager] fileExistsAtPath:backedUpUserDefaultsPath];
    if(fileExists) {
        [self flush];
        _cache = nil;
        _cache = [NSDictionary dictionaryWithContentsOfFile:backedUpUserDefaultsPath];
    }
    
    if(_cache == nil) {
        _cache = [NSMutableDictionary new];
    }
}

- (void) addCachePolicy: (ASICachePolicy) policy {
    cachePolicy |= policy;
    [self save:policy];
}

- (void) removeCachePolicy: (ASICachePolicy) policy {
    cachePolicy &= ~policy;
    [self save:policy];
}

@end

