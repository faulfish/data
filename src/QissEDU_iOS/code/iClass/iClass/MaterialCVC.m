//
//  MaterialCVC.m
//  iClass
//
//  Created by Richard Ting on 10/24/13.
//  Copyright (c) 2013 alan.tu. All rights reserved.
//

#import "MaterialCVC.h"
#import "MaterialCardView.h"
#import "MaterialCollectionViewCell.h"

@interface MaterialCVC ()
@property (strong, nonatomic) NSString *selectedContentID;
@end

@implementation MaterialCVC

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
    self.collectionView.backgroundView = [[UIImageView alloc] initWithImage:[UIImage imageNamed:@"bk_submenu.png"]];

    if (self.materialID) {
        self.selectedContentID = self.materialID;
    } else if (self.examID) {
        self.selectedContentID = self.examID;
    } else {
        self.selectedContentID = nil;
    }
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

#pragma mark - UICollectionViewDataSource
- (NSInteger)numberOfSectionsInCollectionView:(UICollectionView *)collectionView
{
    return 2;
}

- (NSInteger)collectionView:(UICollectionView *)collectionView numberOfItemsInSection:(NSInteger)section
{
    return 1;
}

- (UICollectionViewCell *)collectionView:(UICollectionView *)collectionView cellForItemAtIndexPath:(NSIndexPath *)indexPath
{
    UICollectionViewCell *cell = [collectionView dequeueReusableCellWithReuseIdentifier:@"MaterialItem" forIndexPath:indexPath];

    if ([cell isKindOfClass:[MaterialCollectionViewCell class]]) {
        MaterialCardView *materialCardView = ((MaterialCollectionViewCell *)cell).materialCardView;
        if (indexPath.section == 0) {
            materialCardView.contentID = self.materialID;
            materialCardView.contentTitle = self.materialTitle;
        } else {
            materialCardView.contentID = self.examID;
            materialCardView.contentTitle = self.examTitle;
        }
        if ([materialCardView.contentID isEqualToString:self.selectedContentID]) {
            materialCardView.selected = YES;
        } else {
            materialCardView.selected = NO;
        }
    }
    return cell;
}

- (IBAction)selectMaterial:(UITapGestureRecognizer *)gesture {
    CGPoint tapLocation = [gesture locationInView:self.collectionView];
    NSIndexPath *indexPath = [self.collectionView indexPathForItemAtPoint:tapLocation];
    if (indexPath) {
        // set material item selected
        [self updateUI:indexPath];
    }
}

- (void)updateUI:(NSIndexPath *)selectedIndexPath
{
    for (UICollectionViewCell *cell in [self.collectionView visibleCells]) {
        if ([cell isKindOfClass:[MaterialCollectionViewCell class]]) {
            NSIndexPath *indexPath = [self.collectionView indexPathForCell:cell];

            if (NSOrderedSame == [indexPath compare:selectedIndexPath]) {
                ((MaterialCollectionViewCell *)cell).materialCardView.selected = YES;
                self.selectedContentID = ((MaterialCollectionViewCell *)cell).materialCardView.contentID;
                [self.materialItemDelegate showContentWithContentID:((MaterialCollectionViewCell *)cell).materialCardView.contentID];
            } else {
                ((MaterialCollectionViewCell *)cell).materialCardView.selected = NO;
            }
        }
    }
}

@end
