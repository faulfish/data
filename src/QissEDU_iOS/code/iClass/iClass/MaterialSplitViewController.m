//
//  MaterialSplitViewController.m
//  iClass
//
//  Created by Richard Ting on 10/24/13.
//  Copyright (c) 2013 alan.tu. All rights reserved.
//

#import "MaterialSplitViewController.h"
#import "ClassViewController.h"
#import "ClassTabBarController.h"
#import "MaterialPresentViewController.h"

@interface MaterialSplitViewController () <MaterialSplitViewDelegate>

@property (weak, nonatomic) IBOutlet UIView *masterView;
@property (weak, nonatomic) IBOutlet UIView *detailView;
@property (strong, nonatomic) MaterialPresentViewController *detailController;

@end

@implementation MaterialSplitViewController

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Custom initialization
    }
    return self;
}

- (void)viewDidLoad
{
    [super viewDidLoad];
	// Do any additional setup after loading the view.
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (void) setDsm:(DSShapeManager *)dsm {
    _dsm = dsm;
}

- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender
{
    if ([segue.identifier isEqualToString:@"showMaterialContent"]) {
        if ([segue.destinationViewController respondsToSelector:@selector(setMaterialID:)]) {
            [segue.destinationViewController performSelector:@selector(setMaterialID:) withObject:self.materialID];
        }
        if ([segue.destinationViewController respondsToSelector:@selector(setMaterialSplitViewDelegate:)]) {
            [segue.destinationViewController performSelector:@selector(setMaterialSplitViewDelegate:) withObject:self];
        }
        if ([segue.destinationViewController respondsToSelector:@selector(setDsm:)]) {
            [segue.destinationViewController performSelector:@selector(setDsm:) withObject:self.dsm];
        }
        if ([segue.destinationViewController isKindOfClass:[MaterialPresentViewController class]]) {
            self.detailController = (MaterialPresentViewController *)segue.destinationViewController;
        }
    } else if ([segue.identifier isEqualToString:@"showMaterialList"]) {
        if ([segue.destinationViewController respondsToSelector:@selector(setMaterialID:)]) {
            [segue.destinationViewController performSelector:@selector(setMaterialID:) withObject:self.materialID];
        }
        if ([segue.destinationViewController respondsToSelector:@selector(setMaterialTitle:)]) {
            [segue.destinationViewController performSelector:@selector(setMaterialTitle:) withObject:self.materialTitle];
        }
        if ([segue.destinationViewController respondsToSelector:@selector(setExamID:)]) {
            [segue.destinationViewController performSelector:@selector(setExamID:) withObject:self.examID];
        }
        if ([segue.destinationViewController respondsToSelector:@selector(setExamTitle:)]) {
            [segue.destinationViewController performSelector:@selector(setExamTitle:) withObject:self.examTitle];
        }
        if ([segue.destinationViewController respondsToSelector:@selector(setMaterialItemDelegate:)]) {
            [segue.destinationViewController performSelector:@selector(setMaterialItemDelegate:) withObject:self];
        }
    }
}

- (void)setMasterViewHidden:(BOOL)hidden
{
    for (NSLayoutConstraint *constraint in self.view.constraints) {
        if ((constraint.firstItem == self.detailView && constraint.firstAttribute == NSLayoutAttributeLeading) && constraint.secondItem == self.view) {
            NSLog(@"detailView's leading attribute: %f", constraint.constant);
            if (hidden) {
                constraint.constant = 0;
            } else {
                constraint.constant = 220;
            }
        }
        [UIView animateWithDuration:0.5 animations:^{
            [self.view layoutIfNeeded];
        }];
    }
}

- (void)toThumbnail
{
    if ([self.parentViewController isKindOfClass:[ClassTabBarController class]]) {
        ((ClassTabBarController *)self.parentViewController).selectedIndex = ClassViewType_Thumbnail;
    }
}

- (void)showContentWithContentID:(NSString *)contentID
{
    if ([self.parentViewController respondsToSelector:@selector(setContentID:)]) {
        [self.parentViewController performSelector:@selector(setContentID:) withObject:contentID];
    }
    [self.detailController showContentWithContentID:contentID];
}

@end
