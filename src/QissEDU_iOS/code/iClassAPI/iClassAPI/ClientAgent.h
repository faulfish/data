//
//  iClassAPI.h
//  iClassAPI
//
//  Created by admin on 13/8/14.
//  Copyright (c) 2013年 admin. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "ClassInfoAPI.h"
#import "MaterialInfoAPI.h"
#import "CourseInfoAPI.h"
#import "AttendanceInfoAPI.h"
#import "ExamInfoAPI.h"
#import "AuthAPI.h"

@interface ClientAgent : NSObject
{
    ClassInfoAPI *classAPI;
    MaterialInfoAPI *materialAPI;
    CourseInfoAPI *courseAPI;
    AttendanceInfoAPI *attendanceAPI;
    ExamInfoAPI *examAPI;
    AuthAPI *authAPI;
}

@property (strong, nonatomic) ClassInfoAPI *classAPI;
@property (strong, nonatomic) MaterialInfoAPI *materialAPI;
@property (strong, nonatomic) CourseInfoAPI *courseAPI;
@property (strong, nonatomic) AttendanceInfoAPI *attendanceAPI;
@property (strong, nonatomic) ExamInfoAPI *examAPI;
@property (strong, nonatomic) AuthAPI *authAPI;

// [API: login]
- (NSString *) loginByUserAccount: (NSString *) account
                            error: (NSError **)error;
// For SPEC-R1 for user login.

// [API: get class info.]
- (NSString *) getAllstudentByClass: (NSString *) className error: (NSError **)error;
- (NSString *) getClass: (NSString *) teacherID error: (NSError **) error;
- (NSString *) getClassByStudentID: (NSString *) studentID error: (NSError **) error;
- (NSString *) getClassOnly: (NSString *) teacherID error: (NSError **) error;
- (NSString *) getClassTable: (NSString *) userID error: (NSError **) error; // For SPEC-R3 to get class table.

// [API: get material info.]
- (NSString *) getAllMaterialToClassByTeacherID: (NSString *) teacherID error: (NSError **) error;
- (NSString *) getAllMaterialToClassByTeacherIDAndDateTime: (NSString *) teacherID
                                                      date:(NSString *)dateTime
                                                     error: (NSError **) error;
- (NSString *) getAllMaterialToClassByMaterialID: (NSString *) materialID
                                           error: (NSError **) error;
- (NSString *) getAllMaterialToClass: (NSString *)teacherID
                          materialID:(NSString *)materialID
                           className: (NSString *) className
                               error: (NSError **) error;
- (NSString *) getAllMaterialToClassByClassName: (NSString *) className
                                           date:(NSString *)dateTime
                                          error: (NSError **) error;
- (NSString *) getAllCourseToTeachingMaterialByDateAndClass: (NSString *) className
                                                       date:(NSString *)date
                                                      error: (NSError **) error;
- (NSString *) getCourseSchedule: (NSString *)userID error: (NSError **) error; // For SPEC-R3 to get student or teacher course schedule.
- (NSString *) getCourseMaterial: (NSString *) courseID
                            date: (NSString *) date
                           error: (NSError **) error;
// For SPEC-R6, which teacher can use it  to get teaching material.

// [API: get course info.]
- (NSString *) getCourseAllInfo: (NSString *)classID error: (NSError **) error;
- (NSString *) createSemesterData: (NSString *) year
                     semesterType: (NSString *) type
                semesterStartTime: (NSString *) startTime
                  semesterEndTime: (NSString *) endTime
                            error: (NSError **) error;
- (NSString *)getCourseInfo: (NSString *) courseID date: (NSString *) date error: (NSError **) error;
// For SPEC-R1 to get course info, including attendance, announcement, teaching material and exam schedule.

- (NSString *) createAnnouncement: (NSString *) announcement
                        className: (NSString *) className
                         courseID: (NSString *) courseID
                             date: (NSString *) date
                            error: (NSError **) error;
- (NSString *) getCourseHistory: (NSString *) date
                      className: (NSString *) className
                     courseName: (NSString *) courseName
                          error: (NSError **) error;
//For SPEC-R4 to get course history, including announcement, material title and exam title.

// [API: attendance info.]
- (NSString *) createAttendanceData: (NSString *) courseID
                          studentID: (NSString *) studentID
                             status: (BOOL) status
                               note: (NSString *) note
                               date: (NSString *) date
                              error: (NSError **) error;
// For SPEC-R5 to create attendence data for the specific student.

- (NSString *) getSeatingInfo: (NSString *) courseID
                         date: (NSString *) date
                        error: (NSError **) error;
// For SPEC-R5 to get all student seating info for the class.

- (NSString *) editAttendanceData: (NSString *) attendanceID
                           status: (BOOL) status
                             note: (NSString *) note
                            error: (NSError **) error;
// For SPEC-R5 to edit attendence data for the specific student.

- (NSString *) getAttendanceByStudent: (NSString *) courseID
                            studentID: (NSString *) studentID
                                 date: (NSString *) date
                                error: (NSError **) error;
// For SPEC-R5,R11 to get specific student attendence data.

- (NSString *) getAttendanceHistory: (NSString *) date
                          className: (NSString *) className
                         courseName: (NSString *) courseName
                              error: (NSError **) error;
// For SPEC-R3 to get student's attendance history.

// [API: exam API.]
- (NSString *) editExamComment: (NSString *) examResultID
                       comment: (NSString *) comment
                         error: (NSError **) error;
// For SPEC-R10 to edit exam comment for specific student.

- (NSString *) getExamScoreList: (NSString *) examScheduleID
                          error: (NSError **) error;
// For SPEC-R10 to get all student exam result for the class.

- (NSString *) submitExam: (NSString *) studentID
           examScheduleID: (NSString *) scheduleID
            studentAnswer: (NSString *) answer
                    error: (NSError **) error;
- (NSString *) getStudentExamResult: (NSString *) studentID
                           courseID: (NSString *) courseID
                               date: (NSString *) date
                              error: (NSError **) error;
// For SPEC-R11 to get all exam result for the specific student.

- (NSString *) getExamAnswer: (NSString *) examID
                       error: (NSError **) error;

//for cache policy
- (void) flush; //remove all cache
- (void) addCachePolicy: (ASICachePolicy) policy;
- (void) removeCachePolicy: (ASICachePolicy) policy;

@end
