//
//  PlannerHeaderView.m
//  MyTestApp
//
//  Created by Richard Ting on 8/8/13.
//  Copyright (c) 2013 Richard Ting. All rights reserved.
//

#import "PlannerHeaderView.h"
#import "NSCalendar+Juncture.h"
#import "NSCalendar+DateManipulation.h"
#import "NSDate+Description.h"

@interface PlannerHeaderView()

@property (nonatomic) BOOL isFirstColumnEmpty;
@property (nonatomic) NSUInteger columnCount;
@property (nonatomic) NSUInteger startDay; // 1 - Sunday, 2 - Monday, ..., 7 - Saturday

@end

@implementation PlannerHeaderView

@synthesize pickedDate = _pickedDate;

- (id)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
        // Initialization code
    }
    return self;
}

/*
// Only override drawRect: if you perform custom drawing.
// An empty implementation adversely affects performance during animation.
- (void)drawRect:(CGRect)rect
{
    // Drawing code
}
*/

- (NSDate *)pickedDate
{
    if (!_pickedDate) {
        NSCalendar *calendar = [NSCalendar currentCalendar];
        [calendar setLocale:[NSLocale currentLocale]];
        _pickedDate = [calendar firstDayOfTheWeekUsingReferenceDate:[NSDate date]];
    }
    return _pickedDate;
}

- (void)setPickedDate:(NSDate *)pickedDate
{
    NSCalendar *calendar = [NSCalendar currentCalendar];
    [calendar setLocale:[NSLocale currentLocale]];
    _pickedDate = [calendar firstDayOfTheWeekUsingReferenceDate:pickedDate];
    [self setNeedsLayout];
}

- (void)setColumnCount:(NSUInteger)count
              startDay:(NSUInteger)startDay
    firstColumnIsEmpty:(BOOL)firstColumnIsEmpty
{
    if (count > 0 && count <= 7)
        self.columnCount = count;

    if (startDay > 0 && count <= 7) {
        self.startDay = startDay;
    } else {
        self.startDay = 1;
    }

    self.isFirstColumnEmpty = firstColumnIsEmpty;
}

- (void)layoutSubviews
{
    [self setupWeekday1];
}

+ (NSArray *)weekdayTitleStrings
{
    //return @[@"", @"SUN", @"MON", @"TUE", @"WED", @"THU", @"FRI", @"SAT"];
    return @[@"", @"MON", @"TUE", @"WED", @"THU", @"FRI"];
}

- (void)setupWeekday1
{
    if (self.columnCount > 0) {
        NSArray *subviews = self.subviews;
        if ([subviews count] > 0) {
            for (UIView *view in subviews) {
                [view removeFromSuperview];
            }
        }

        CGFloat weekdayCellWidth = self.bounds.size.width / self.columnCount;
        CGFloat weekdayCellHeight = self.bounds.size.height;

        NSCalendar *calendar = [NSCalendar currentCalendar];
        [calendar setLocale:[NSLocale currentLocale]];
        NSDate *day = [calendar firstDayOfTheWeekUsingReferenceDate:self.pickedDate];
        day = [calendar dateByAddingDays:(self.startDay - 1) toDate:day];

        UILabel *label;
        NSRange range;
        for (int i = 0; i < self.columnCount; i++) {
            label = [[UILabel alloc] initWithFrame:CGRectMake(i * weekdayCellWidth, 0, weekdayCellWidth, weekdayCellHeight)];
            label.numberOfLines = 0;
            [label setTextAlignment:NSTextAlignmentCenter];
            [label setBackgroundColor:[UIColor clearColor]];

            NSString *dateString = [NSString stringWithFormat:@"%@", [day monthAndDayOnCalendar:calendar]];;
            NSString *weekdayName = [NSString stringWithFormat:@"%@", [day localizedWeekdayName:calendar]];
            label.text = [NSString stringWithFormat:@"%@\n%@", dateString, weekdayName];
            range = [[label.attributedText string] rangeOfString:dateString];
            [self addLabelAttributes:@{ NSFontAttributeName : [UIFont boldSystemFontOfSize:20]} range:range label:label];

            range = [[label.attributedText string] rangeOfString:weekdayName];
            [self addLabelAttributes:@{ NSFontAttributeName : [UIFont boldSystemFontOfSize:15]} range:range label:label];

            day = [calendar dateByAddingDays:1 toDate:day];

            if (i == 0 && self.isFirstColumnEmpty) continue;
            
            [self addSubview:label];
        }
    }
}

- (void)setupWeekday
{
    NSArray *subviews = self.subviews;
    if ([subviews count] > 0) {
        for (UIView *view in subviews) {
            [view removeFromSuperview];
        }
    }
    if ([[PlannerHeaderView weekdayTitleStrings] count] > 0) {
        CGFloat weekdayCellWidth = self.bounds.size.width / [[PlannerHeaderView weekdayTitleStrings] count];
        CGFloat weekdayNameLabelHeight = PLANNER_HEADER_NAME_LABEL_HEIGHT;
        CGFloat weekdayDateLabelHeight = PLANNER_HEADER_DATE_LABEL_HEIGHT;

        NSCalendar *calendar = [NSCalendar currentCalendar];
        [calendar setLocale:[NSLocale currentLocale]];
        NSDate *day = [calendar firstDayOfTheWeekUsingReferenceDate:self.pickedDate];
        day = [calendar dateByAddingDays:1 toDate:day];

        for (NSUInteger weekday = 0; weekday < [[PlannerHeaderView weekdayTitleStrings] count]; weekday++) {
            UILabel *nameLabel = [[UILabel alloc] initWithFrame:CGRectMake(weekday * weekdayCellWidth, self.bounds.size.height - weekdayNameLabelHeight, weekdayCellWidth, weekdayNameLabelHeight)];
            [nameLabel setTextAlignment:NSTextAlignmentCenter];
            [nameLabel setFont:[UIFont systemFontOfSize:10]];
            [nameLabel setText:[PlannerHeaderView weekdayTitleStrings][weekday]];
            [nameLabel setBackgroundColor:[UIColor clearColor]];
            [self addSubview:nameLabel];

            UILabel *dateLabel = [[UILabel alloc] initWithFrame:CGRectMake(weekday * weekdayCellWidth, self.bounds.size.height - weekdayNameLabelHeight - weekdayDateLabelHeight, weekdayCellWidth, weekdayDateLabelHeight)];
            [dateLabel setTextAlignment:NSTextAlignmentCenter];
            [dateLabel setFont:[UIFont systemFontOfSize:20]];
            if (weekday != 0) {
                dateLabel.text = [NSString stringWithFormat:@"%@", [day dayOfMonthOnCalendar:calendar]];
                day = [calendar dateByAddingDays:1 toDate:day];
            }
            [dateLabel setBackgroundColor:[UIColor clearColor]];
            [self addSubview:dateLabel];
        }
    }
}

- (NSUInteger)startDayOfWeekFromDate:(NSDate *)date
{
    NSDate *startDate;
    NSCalendar *gregorian = [[NSCalendar alloc] initWithCalendarIdentifier:NSGregorianCalendar];

    NSDateComponents *components = [gregorian components:NSYearCalendarUnit | NSMonthCalendarUnit | NSWeekCalendarUnit fromDate:date];
    [components setWeekday:1]; // Sunday == 1
    startDate = [gregorian dateFromComponents:components];

    NSDateComponents *startDateComponents = [gregorian components:NSYearCalendarUnit | NSMonthCalendarUnit | NSDayCalendarUnit fromDate:startDate];

    return [startDateComponents day];
}

// add attributes to the given range in self.label
- (void)addLabelAttributes:(NSDictionary *)attributes range:(NSRange)range label:(UILabel *)label
{
    if (range.location != NSNotFound) {
        NSMutableAttributedString *mat = [label.attributedText mutableCopy];
        [mat addAttributes:attributes
                     range:range];
        label.attributedText = mat;
    }
}

@end
