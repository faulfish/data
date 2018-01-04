//
//  DatePickerView.m
//  DatePicker
//
//  Created by Richard Ting on 10/2/13.
//  Copyright (c) 2013 Richard Ting. All rights reserved.
//

#import "DatePickerView.h"
#import "NSCalendar+DateManipulation.h"
#import "NSDate+Description.h"

@interface DatePickerView()

@property (nonatomic) NSUInteger dateCellCount;
@property (strong, nonatomic) NSMutableArray *dateCells;

@end

@implementation DatePickerView

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

- (NSDate *)date
{
    if (!_date) _date = [NSDate date];
    return _date;
}

- (NSUInteger)dateCellCount
{
    if ([self.dataSource respondsToSelector:@selector(numberOfColumnsInDatePickerView:)]) {
        _dateCellCount = [self.dataSource numberOfColumnsInDatePickerView:self];
    }
    return _dateCellCount;
}

- (NSMutableArray *)dateCells
{
    if (!_dateCells) {
        _dateCells = [[NSMutableArray alloc] init];
        for (int i = 0; i < self.dateCellCount; i ++) {
            UILabel *label = [[UILabel alloc] init];
            [_dateCells addObject:label];
        }
    }
    return _dateCells;
}

- (void)layoutSubviews
{
    NSArray *subviews = self.subviews;
    if ([subviews count] > 0) {
        for (UIView *view in subviews) {
            [view removeFromSuperview];
        }
    }

    // Show date labels
    CGFloat xOffset = self.frame.size.width / self.dateCellCount;
    UILabel *dateCell;
    NSDate *date = [[NSCalendar currentCalendar] dateBySubtractingWeeks:2 fromDate:self.date];

    for (int i = 0; i < [self.dateCells count]; i++) {
        dateCell = self.dateCells[i];
        dateCell.frame = CGRectMake(i * xOffset, 0, xOffset, self.frame.size.height);
        dateCell.text = [NSString stringWithFormat:@"%@", [date periodOfWeek]];
        dateCell.textColor = [UIColor whiteColor];
        dateCell.font = [UIFont boldSystemFontOfSize:13.0];
        dateCell.textAlignment = NSTextAlignmentCenter;
        if (i == 2) {
            dateCell.backgroundColor = self.selectedColor;
        } else {
            dateCell.backgroundColor = [UIColor clearColor];
        }
        [self addSubview:dateCell];
        date = [[NSCalendar currentCalendar] dateByAddingWeeks:1 toDate:date];
    }
}

- (void)tapHandler:(CGPoint)point
{
    // Update cell highlight
    UILabel *cell;

    for (int i = 0; i < [self.dateCells count]; i++) {
        cell = self.dateCells[i];
        if (CGRectContainsPoint(cell.frame, point)) {
            cell.backgroundColor = self.selectedColor;
            self.date = [[NSCalendar currentCalendar] dateByAddingWeeks:i-2 toDate:self.date];
            if ([self.delegate respondsToSelector:@selector(datePickerView:didSelectColumnAtIndex:)]) {
                [self.delegate datePickerView:self didSelectColumnAtIndex:i];
            }
        } else {
            cell.backgroundColor = [UIColor clearColor];
        }
    }
    [self layoutSubviews];
}

@end
