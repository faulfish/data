//
//  MaterialPageDatasource.m
//  iClass
//
//  Created by alan.tu on 9/3/13.
//  Copyright (c) 2013 alan.tu. All rights reserved.
//

#import "MaterialPageDatasource.h"

@interface MaterialPageDatasource()
@property (readonly, strong, nonatomic) NSArray *pageData;
@end

@implementation MaterialPageDatasource

@synthesize materialID = _materialID;

- (id) init {
    self = [super init];
    return self;
}

-(NSString *) materialID {
    return _materialID;
}

- (void) setDsm:(DSShapeManager *)dsm {
    _dsm = dsm;
}

-(void)setMaterialID:(NSString *)materialID1 {
    _materialID = materialID1;
    if (self) {
        // Create the data model.
        //NSDateFormatter *dateFormatter = [[NSDateFormatter alloc] init];
        //_pageData = [[dateFormatter monthSymbols] copy];
        
        NSArray *searchPaths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
        NSString *documentPath = [searchPaths objectAtIndex:0];
        NSString *zippathTemp = [documentPath stringByAppendingPathComponent:@"zip"];
        NSString *docsDir = [zippathTemp stringByAppendingPathComponent:self.materialID];
        NSError *error;
        _pageData = [[NSFileManager defaultManager] contentsOfDirectoryAtPath:docsDir error:&error];
    }
}

- (MaterialPageViewController *)viewControllerAtIndex:(NSUInteger)index storyboard:(UIStoryboard *)storyboard
{
    // Return the data view controller for the given index.
    if (([self.pageData count] == 0) || (index >= [self.pageData count])) {
        return nil;
    }
    
    // Create a new view controller and pass suitable data.
    MaterialPageViewController *dataViewController = [storyboard instantiateViewControllerWithIdentifier:@"MaterialPageViewController"];
    
    NSArray *searchPaths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *documentPath = [searchPaths objectAtIndex:0];
    NSString *zippathTemp = [documentPath stringByAppendingPathComponent:@"zip"];
    NSString *docsDir = [zippathTemp stringByAppendingPathComponent:self.materialID];
    NSString *filepath = [docsDir stringByAppendingPathComponent:self.pageData[index]];
    
    dataViewController.dataObject = filepath;
    return dataViewController;
}

- (NSUInteger)indexOfViewController:(MaterialPageViewController *)viewController
{
    // Return the index of the given data view controller.
    // For simplicity, this implementation uses a static array of model objects and the view controller stores the model object; you can therefore use the model object to identify the index.
    
    NSArray* array = [[NSFileManager defaultManager] componentsToDisplayForPath:viewController.dataObject];
    
    return [self.pageData indexOfObject:[array lastObject]];
}

#pragma mark - Page View Controller Data Source

- (UIViewController *)pageViewController:(UIPageViewController *)pageViewController viewControllerBeforeViewController:(UIViewController *)viewController
{
    NSUInteger index = [self indexOfViewController:(MaterialPageViewController *)viewController];
    if ((index == 0) || (index == NSNotFound)) {
        return nil;
    }
    
    index--;
    if (_dsm) {
        [_dsm setPageID:index];
    }
    return [self viewControllerAtIndex:index storyboard:viewController.storyboard];
}

- (UIViewController *)pageViewController:(UIPageViewController *)pageViewController viewControllerAfterViewController:(UIViewController *)viewController
{
    NSUInteger index = [self indexOfViewController:(MaterialPageViewController *)viewController];
    if (index == NSNotFound) {
        return nil;
    }
    
    index++;
    if (index == [self.pageData count]) {
        return nil;
    }
    if (_dsm) {
        [_dsm setPageID:index];
    }
    return [self viewControllerAtIndex:index storyboard:viewController.storyboard];
}

@end
