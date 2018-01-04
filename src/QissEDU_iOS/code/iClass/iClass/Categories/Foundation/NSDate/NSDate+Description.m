//
//  NSDate+Description.m
//   MBCalendarKit
//
//  Created by Moshe Berman on 4/14/13.
//  Copyright (c) 2013 Moshe Berman. All rights reserved.
//

#import "NSDate+Description.h"
#import "NSCalendar+Juncture.h"

@implementation NSDate (Description)

- (NSString *)description
{
    NSDateFormatter *formatter = [NSDateFormatter new];
    [formatter setTimeStyle:NSDateFormatterNoStyle];
    [formatter setDateStyle:NSDateFormatterLongStyle];
    return [formatter stringFromDate:self];
}

- (NSString *)dayNameOnCalendar:(NSCalendar *)calendar
{
    NSDateFormatter *f = [NSDateFormatter new];
    [f setCalendar:calendar];
    [f setDateFormat:@"ccc"];
    return [f stringFromDate:self];
}

- (NSString *)localizedWeekdayName:(NSCalendar *)calendar
{
    NSDateComponents *components = [calendar components:NSWeekdayCalendarUnit fromDate:self];
    // The weekdays start from Sunday and from index 1
    // 1 - Sunday, 2 - Monday ...
    int weekdayNumber = [components weekday];

    // We need an NSDateFormatter to have access to the localized weekday strings
    NSDateFormatter *formatter = [[NSDateFormatter alloc] init];
    [formatter setLocale:calendar.locale];

    // weekdaySymbols returns and array of strings. Keep in mind that they start from Sunday
    // 0 - Sunday, 1 - Monday, 2 - Tuesday ...
    return [[formatter weekdaySymbols] objectAtIndex:weekdayNumber - 1];
}

- (NSString *)monthNameOnCalendar:(NSCalendar *)calendar
{
    NSDateFormatter *f = [NSDateFormatter new];
    [f setCalendar:calendar];
    [f setDateFormat:@"MMMM"];
    return [f stringFromDate:self];
}

- (NSString *)monthAndYearOnCalendar:(NSCalendar *)calendar
{
    NSDateFormatter *f = [NSDateFormatter new];
    [f setCalendar:calendar];
    [f setDateFormat:@"MMMM yyyy"];
    return [f stringFromDate:self];
}

- (NSString *)monthAbbreviationAndYearOnCalendar:(NSCalendar *)calendar
{
    NSDateFormatter *f = [NSDateFormatter new];
    [f setCalendar:calendar];
    [f setDateFormat:@"MMM yyyy"];
    return [f stringFromDate:self];
}


- (NSString *)monthAbbreviationOnCalendar:(NSCalendar *)calendar
{
    NSDateFormatter *f = [NSDateFormatter new];
    [f setCalendar:calendar];
    [f setDateFormat:@"MMM"];
    return [f stringFromDate:self];
}

- (NSString *)monthAndDayOnCalendar:(NSCalendar *)calendar
{
    NSDateFormatter *f = [NSDateFormatter new];
    [f setCalendar:calendar];
    [f setDateFormat:@"M/d"];
    return [f stringFromDate:self];
}

- (NSString *)dayOfMonthOnCalendar:(NSCalendar *)calendar
{
    NSDateFormatter *f = [NSDateFormatter new];
    [f setCalendar:calendar];
    [f setDateFormat:@"d"];
    return [f stringFromDate:self];
}

- (NSString *)monthAndDayAndYearOnCalendar:(NSCalendar *)calendar
{
    NSDateFormatter *f = [NSDateFormatter new];
    [f setCalendar:calendar];
    [f setDateFormat:@"MMM d yyyy"];
    return [f stringFromDate:self];
}

- (NSString *)yearAndMonthAndDayOnCalendar:(NSCalendar *)calendar
{
    NSDateFormatter *f = [NSDateFormatter new];
    [f setCalendar:calendar];
    [f setDateFormat:@"yyyyMMdd"];
    return [f stringFromDate:self];
}

- (NSString *)dayOfMonthAndYearOnCalendar:(NSCalendar *)calendar
{
    NSDateFormatter *f = [NSDateFormatter new];
    [f setCalendar:calendar];
    [f setDateFormat:@"d yyyy"];
    return [f stringFromDate:self];
}

- (NSString *)periodOfWeek
{
    return [self periodOfWeekUsingReferenceDate:self];
}

- (NSString *)periodOfWeekUsingReferenceDate:(NSDate *)date
{
    NSCalendar *calendar = [NSCalendar currentCalendar];
    NSDate *firstDayOfWeek = [calendar firstDayOfTheWeekUsingReferenceDate:date];
    NSDate *lastDayOfWeek = [calendar lastDayOfTheWeekUsingReferenceDate:date];

    NSString *firstMonth, *firstDay, *lastDay;
    NSDateFormatter *formatter = [[NSDateFormatter alloc] init];
    formatter.calendar = calendar;

    [formatter setDateFormat:@"MMM"];
    firstMonth = [formatter stringFromDate:firstDayOfWeek];

    [formatter setDateFormat:@"d"];
    firstDay = [formatter stringFromDate:firstDayOfWeek];
    lastDay = [formatter stringFromDate:lastDayOfWeek];

    return [NSString stringWithFormat:@"%@ %@ - %@", firstMonth, firstDay, lastDay];
}

@end
