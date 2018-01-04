//
//  PlannerView.m
//  iClass
//
//  Created by Richard Ting on 8/27/13.
//  Copyright (c) 2013 alan.tu. All rights reserved.
//

#import <QuartzCore/QuartzCore.h>
#import "PlannerView.h"
#import "PlannerHeaderView.h"
#import "PlannerCourseCell.h"
#import "Course.h"
#import "NSCalendarCategories.h"
#import "UIColor+HexString.h"

@interface PlannerView()
@property (strong, nonatomic) PlannerHeaderView *headerView;
@property (strong, nonatomic) NSMutableArray *courseCells;
@property (nonatomic) NSInteger selectedCourseIndex;
@end

@implementation PlannerView

- (id)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
        // Initialization code
    }
    return self;
}

- (NSDate *)pickedDate
{
    return self.headerView.pickedDate;
}

- (void)setPickedDate:(NSDate *)pickedDate
{
    self.headerView.pickedDate = pickedDate;
    [self autoSelectOneCourse];
}

- (void)setCourses:(NSMutableArray *)courses
{
    _courses = courses;
    if (_courses) {
        [self drawCourseCells];
        [self autoSelectOneCourse];
    } else {
        [self initCourseCells];
    }
}

- (void)autoSelectOneCourse
{
    PlannerCourseCell *cell;
    NSCalendar *calendar = [NSCalendar currentCalendar];
    NSDate *today = [NSDate date];
    NSInteger weekday = [calendar weekdayInDate:today];
    NSInteger hour = [calendar hoursInDate:today];
    int startIndex;

    if ([calendar date:today isSameWeekAs:self.pickedDate]) {
        // Picked date is in the same week of today, should select the most closed course
        startIndex = (weekday - 2) * 8 + ((hour > 12) ? (hour - 9) : (hour - 8));
    } else {
        // Select the first course
        startIndex = 0;
    }
    for (int i = startIndex; i < [self.courseCells count]; i++) {
        cell = self.courseCells[i];

        if (![cell.text isEqualToString:@"--"]) {
            self.selectedCourseIndex = i;
            break;
        }
    }
}

- (void)setSelectedCourseIndex:(NSInteger)selectedCourseIndex
{
    _selectedCourseIndex = selectedCourseIndex;

    if (_selectedCourseIndex > -1) {
        PlannerCourseCell *cell;
        for (int i = 0; i < [self.courseCells count]; i++) {
            cell = self.courseCells[i];

            if (self.selectedCourseIndex == i) {
                [cell setBackgroundColor:self.courseCellColor];
                [cell setTextColor:[UIColor whiteColor]];

                if ([self.courseInfoDelegate respondsToSelector:@selector(setCourseId:courseName:className:date:period:)]) {
                    NSCalendar *calendar = [NSCalendar currentCalendar];

                    [self.courseInfoDelegate setCourseId:cell.courseId
                                              courseName:cell.courseName
                                               className:cell.className
                                                    date:[calendar dateByAddingDays:(i/8+1) toDate:self.pickedDate]
                                                  period:cell.period];
                }
            } else {
                [cell setBackgroundColor:[UIColor clearColor]];
                [cell setTextColor:[UIColor blackColor]];
            }
        }
    }
}

- (void)drawRect:(CGRect)rect
{
    // Drawing code
    [self setupRoundRectShadow];
    [self drawRoundRect];
    [self drawTitleRect];
    [self drawGridLines];
    [self drawTextLabels];
    [self initCourseCells];
}

- (void)setupRoundRectShadow
{
    // Round rect shadow
    self.layer.masksToBounds = NO;
    self.layer.cornerRadius = 8;
    //self.layer.shadowColor = [UIColor blueColor].CGColor;
    self.layer.shadowOffset = CGSizeMake(0, 1);
    self.layer.shadowRadius = 0.5;
    self.layer.shadowOpacity = 0.25;
    self.layer.shadowPath = [UIBezierPath bezierPathWithRoundedRect:self.bounds cornerRadius:PLANNER_VIEW_CORNER_RADIUS].CGPath;
}

- (void)drawRoundRect
{
    UIBezierPath *roundedRect = [UIBezierPath bezierPathWithRoundedRect:self.bounds cornerRadius:PLANNER_VIEW_CORNER_RADIUS];
    [roundedRect addClip];

    [[UIColor whiteColor] setFill];
    UIRectFill(self.bounds);
}

- (void)drawTitleRect
{
    CGContextSaveGState(UIGraphicsGetCurrentContext());
    CGRect titleRect = CGRectMake(0, 0, self.bounds.size.width, PLANNER_VIEW_TITLE_HEIGHT);
    UIBezierPath *titleRectPath = [UIBezierPath bezierPathWithRoundedRect:titleRect
                                                      byRoundingCorners:UIRectCornerTopLeft|UIRectCornerTopRight
                                                            cornerRadii:CGSizeMake(PLANNER_VIEW_CORNER_RADIUS, PLANNER_VIEW_CORNER_RADIUS)];

    [titleRectPath addClip];

    [[UIColor colorWithHexString:@"#F1F3F4"] setFill];
    UIRectFill(titleRect);

    CGContextRestoreGState(UIGraphicsGetCurrentContext());
}

- (void)drawGridLines
{
    CGContextRef context = UIGraphicsGetCurrentContext();

    UIColor *color = [UIColor colorWithHexString:@"#C3C3C2"];
    [color set];

    // Draw vertical grid lines
    CGContextSaveGState(context);

    CGFloat x = 0;
    CGFloat x_spacing = self.bounds.size.width / (PLANNER_VIEW_COURSE_COLUMN+1);
    CGFloat y = PLANNER_VIEW_TITLE_HEIGHT;
    CGFloat height = self.bounds.size.height - PLANNER_VIEW_TITLE_HEIGHT;

    x += x_spacing;
    for (int i = 1; i < (PLANNER_VIEW_COURSE_COLUMN+1); i++) {
        CGContextMoveToPoint(context, x, y);
        CGContextAddLineToPoint(context, x, y+height);
        CGContextStrokePath(context);

        x += x_spacing;
    }
	CGContextRestoreGState(context);

    // Draw horizontal grid lines - solid lines
    CGContextSaveGState(context);

    CGContextMoveToPoint(context, 0, PLANNER_VIEW_TITLE_HEIGHT + PLANNER_VIEW_DATE_HEIGHT);
    CGContextAddLineToPoint(context, self.bounds.size.width, PLANNER_VIEW_TITLE_HEIGHT + PLANNER_VIEW_DATE_HEIGHT);
    CGContextStrokePath(context);

    CGFloat y_spacing = (self.bounds.size.height - PLANNER_VIEW_TITLE_HEIGHT - PLANNER_VIEW_DATE_HEIGHT - PLANNER_VIEW_NOON_HEIGHT) / PLANNER_VIEW_COURSE_ROW;
    y = PLANNER_VIEW_TITLE_HEIGHT + PLANNER_VIEW_DATE_HEIGHT + y_spacing;
    for (int i = 1; i < 4; i++) {
        CGContextMoveToPoint(context, 0, y);
        CGContextAddLineToPoint(context, x_spacing, y);
        CGContextStrokePath(context);
        y += y_spacing;
    }

    CGContextMoveToPoint(context, 0, y);
    CGContextAddLineToPoint(context, self.bounds.size.width, y);
    CGContextStrokePath(context);

    y += PLANNER_VIEW_NOON_HEIGHT;
    CGContextMoveToPoint(context, 0, y);
    CGContextAddLineToPoint(context, self.bounds.size.width, y);
    CGContextStrokePath(context);

    y += y_spacing;
    for (int i = 1; i < 4; i++) {
        CGContextMoveToPoint(context, 0, y);
        CGContextAddLineToPoint(context, x_spacing, y);
        CGContextStrokePath(context);
        y += y_spacing;
    }
    CGContextRestoreGState(context);

    // Draw horizontal grid lines - dashed lines
    CGContextSaveGState(context);

    CGFloat dashPattern[] = {2,6,4,2};
    y = PLANNER_VIEW_TITLE_HEIGHT + PLANNER_VIEW_DATE_HEIGHT + y_spacing;

    for (int i = 1; i < 8; i++) {
        CGContextMoveToPoint(context, x_spacing, y);
        CGContextAddLineToPoint(context, self.bounds.size.width, y);
        CGContextSetLineDash(context, 2, dashPattern, 1);
        CGContextStrokePath(context);
        if (i == 4) {
            y += (PLANNER_VIEW_NOON_HEIGHT + y_spacing);
        } else {
            y += y_spacing;
        }
    }
    CGContextRestoreGState(context);

    // Draw noon background
    CGContextSaveGState(UIGraphicsGetCurrentContext());
    CGRect noonRect = CGRectMake(0, PLANNER_VIEW_TITLE_HEIGHT + PLANNER_VIEW_DATE_HEIGHT + y_spacing*4 + 0.5, self.bounds.size.width, PLANNER_VIEW_NOON_HEIGHT-1);
    UIBezierPath *noonRectPath = [UIBezierPath bezierPathWithRect:noonRect];

    [noonRectPath addClip];

    [[UIColor colorWithHexString:@"#F1F3F4"] setFill];
    UIRectFill(noonRect);

    CGContextRestoreGState(UIGraphicsGetCurrentContext());
}

+ (NSArray *)timeLabelTitle
{
    return @[@"1", @"2", @"3", @"4", @"5", @"6", @"7", @"8"];
}

+ (NSArray *)timeLabelSubtitle
{
    return @[@"08:10-09:00", @"09:10-10:00", @"10:10-11:00", @"11:10-12:00", @"13:10-14:00", @"14:10-15:00", @"15:10-16:00", @"16:10-17:00"];
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

- (NSArray *)courseCells
{
    if (!_courseCells) _courseCells = [[NSMutableArray alloc] init];
    return _courseCells;
}

- (void)drawTextLabels
{
    // Title view
    UILabel *nameLabel = [[UILabel alloc] initWithFrame:CGRectMake(20, 0, self.frame.size.width-40, PLANNER_VIEW_TITLE_HEIGHT)];
    [nameLabel setBackgroundColor:[UIColor clearColor]];
    nameLabel.text = [NSString stringWithFormat:@"%@%@", self.userName, ((self.role == 0) ? @" 老師" : @"")];
    [self addSubview:nameLabel];

    // Day row view
    self.headerView = [[PlannerHeaderView alloc] initWithFrame:CGRectMake(0, PLANNER_VIEW_TITLE_HEIGHT, self.frame.size.width, PLANNER_VIEW_DATE_HEIGHT)];
    [self.headerView setColumnCount:PLANNER_VIEW_COURSE_COLUMN+1 startDay:1 firstColumnIsEmpty:YES];
    [self addSubview:self.headerView];

    // Time column view
    CGFloat x_spacing = self.bounds.size.width / (PLANNER_VIEW_COURSE_COLUMN+1);
    CGFloat y_spacing = (self.bounds.size.height - PLANNER_VIEW_TITLE_HEIGHT - PLANNER_VIEW_DATE_HEIGHT - PLANNER_VIEW_NOON_HEIGHT) / PLANNER_VIEW_COURSE_ROW;
    CGFloat y = PLANNER_VIEW_TITLE_HEIGHT + PLANNER_VIEW_DATE_HEIGHT;

    UILabel *timeLabel;
    NSRange range;

    for (int i = 0; i < PLANNER_VIEW_COURSE_ROW; i++) {
        timeLabel = [[UILabel alloc] initWithFrame:CGRectMake(0, y, x_spacing, y_spacing)];
        timeLabel.numberOfLines = 0;
        [timeLabel setTextAlignment:NSTextAlignmentCenter];
        [timeLabel setBackgroundColor:[UIColor clearColor]];

        timeLabel.text = [NSString stringWithFormat:@"%@\n%@", [PlannerView timeLabelTitle][i], [PlannerView timeLabelSubtitle][i]];;

        range = [[timeLabel.attributedText string] rangeOfString:[PlannerView timeLabelTitle][i]];
        [self addLabelAttributes:@{ NSFontAttributeName : [UIFont boldSystemFontOfSize:24]} range:range label:timeLabel];

        range = [[timeLabel.attributedText string] rangeOfString:[PlannerView timeLabelSubtitle][i]];
        [self addLabelAttributes:@{ NSFontAttributeName : [UIFont boldSystemFontOfSize:12]} range:range label:timeLabel];

        [self addSubview:timeLabel];

        if (i == 3) {
            y += (PLANNER_VIEW_NOON_HEIGHT + y_spacing);
        } else {
            y += y_spacing;
        }
    }
}

- (void)initCourseCells
{
    CGFloat x_spacing = self.bounds.size.width / (PLANNER_VIEW_COURSE_COLUMN+1);
    CGFloat y_spacing = (self.bounds.size.height - PLANNER_VIEW_TITLE_HEIGHT - PLANNER_VIEW_DATE_HEIGHT - PLANNER_VIEW_NOON_HEIGHT) / PLANNER_VIEW_COURSE_ROW;
    CGFloat y;

    // Clear previous cells
    if ([self.courseCells count] > 0) {
        for (UIView *view in self.courseCells) {
            [view removeFromSuperview];
        }
        self.courseCells = nil;
    }

    // Course table
    self.selectedCourseIndex = -1;
    PlannerCourseCell *courseLabel;

    for (int i = 0; i < PLANNER_VIEW_COURSE_COLUMN; i++) {
        y = PLANNER_VIEW_TITLE_HEIGHT + PLANNER_VIEW_DATE_HEIGHT;
        for (int j = 0; j < PLANNER_VIEW_COURSE_ROW; j++) {
            courseLabel = [[PlannerCourseCell alloc] initWithFrame:CGRectMake((i+1)*x_spacing, y, x_spacing, y_spacing)];
            courseLabel.numberOfLines = 0;
            [courseLabel setTextAlignment:NSTextAlignmentCenter];
            [courseLabel setFont:[UIFont boldSystemFontOfSize:15]];
            [courseLabel setBackgroundColor:[UIColor clearColor]];
            courseLabel.text = @"--";

            [self.courseCells addObject:courseLabel];
            [self addSubview:courseLabel];
            if (j == 3) {
                y += (PLANNER_VIEW_NOON_HEIGHT + y_spacing);
            } else {
                y += y_spacing;
            }
        }
    }
}

- (void)drawCourseCells
{
    for (int i = 0; i < [self.courses count] && i < [self.courseCells count]; i++) {
        Course *course = self.courses[i];
        [self drawCourseCellWithCourse:course role:self.role];
    }
}

- (void)drawCourseCellWithCourse:(Course *)course role:(NSUInteger)role
{
    NSCalendar *calendar = [NSCalendar currentCalendar];
    NSUInteger weekday = course.weekday;
    NSUInteger hour = [calendar hoursInDate:course.startTime];
    NSUInteger duration = 1;

    // Weekday: Sunday = 1, Monday = 2
    if (course.weekday < 2 && course.weekday > 6 && hour < 8 && hour > 16) {
        return;
    }

    int index = (weekday - 2) * 8 + ((hour > 12) ? (hour - 9) : (hour - 8));
    for (int i = 0; i < duration; i++) {
        if ([self.courseCells count] <= index) {
            NSLog(@"The couse '%@' is out of planner table", course.courseName);
            continue;
        }
        PlannerCourseCell *cell = self.courseCells[index];
        cell.courseId = course.courseId;
        cell.courseName = course.courseName;
        cell.className = course.className;
        cell.period = [PlannerView timeLabelSubtitle][index%8];
        if (role == 0) {
            cell.text = [NSString stringWithFormat:@"%@\n%@", course.courseName, course.className];
        } else {
            cell.text = course.courseName;
        }
        index++;
    }
}

- (void)drawCourseWithName:(NSString *)name weekday:(NSUInteger)weekday startHour:(NSUInteger)hour duration:(NSUInteger)duration
{
    // Weekday: Sunday = 1, Monday = 2
    if (weekday < 2 && weekday > 6 && hour < 8 && hour > 16) {
        return;
    }

    int index = (weekday - 2) * 8 + ((hour > 12) ? (hour - 9) : (hour - 8));

    for (int i = 0; i < duration; i++) {
        if ([self.courseCells count] <= index) {
            NSLog(@"The couse '%@' is out of planner table", name);
            continue;
        }
        PlannerCourseCell *cell = self.courseCells[index];
        cell.text = name;
        index++;
    }
}

#pragma mark - Touch Handling

- (BOOL)pointInside:(CGPoint)point withEvent:(UIEvent *)event
{
    NSLog(@"%@, event_type:%d, event_subtype: %d", NSStringFromCGPoint(point), event.type, event.subtype);

    PlannerCourseCell *cell;

    for (int i = 0; i < [self.courseCells count]; i++) {
        cell = self.courseCells[i];

        if ([cell.text isEqualToString:@"--"]) {
            if (CGRectContainsPoint(cell.frame, point)) {
                NSLog(@"select empty course");
                return [super pointInside:point withEvent:event];
            }
            continue;
        }
        if (CGRectContainsPoint(cell.frame, point)) {
            if (self.selectedCourseIndex == i) {
                NSLog(@"select the same course");
                return [super pointInside:point withEvent:event];
            }
            self.selectedCourseIndex = i;
            break;
        }
    }

    return [super pointInside:point withEvent:event];
}

@end
