//
//  DatePickerView.h
//  DatePicker
//
//  Created by Richard Ting on 10/2/13.
//  Copyright (c) 2013 Richard Ting. All rights reserved.
//

#import <UIKit/UIKit.h>

@class DatePickerView;

@protocol DatePickerViewDataSource <NSObject>

- (NSUInteger)numberOfColumnsInDatePickerView:(DatePickerView *)datePickerView;

@end

@protocol DatePickerViewDelegate <NSObject>

- (void)datePickerView:(DatePickerView *)datePickerView didSelectColumnAtIndex:(NSUInteger)index;

@end

@interface DatePickerView : UIView

@property (strong, nonatomic) NSDate *date;
@property (strong, nonatomic) UIColor *selectedColor;
@property (nonatomic, assign) id<DatePickerViewDataSource> dataSource;
@property (nonatomic, assign) id<DatePickerViewDelegate> delegate;

- (void)tapHandler:(CGPoint)point;

@end
