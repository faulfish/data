//
//  ThumbnailViewController.m
//  iClass
//
//  Created by alan.tu on 8/21/13.
//  Copyright (c) 2013 alan.tu. All rights reserved.
//

#import "ThumbnailViewController.h"
#import "ThumbnailCell.h"
#import "ClassViewController.h"
#import "ClassTabBarController.h"

@interface ThumbnailViewController ()

@property NSInteger thumbnailH;
@property NSInteger thumbnailW;
@property (weak, nonatomic) IBOutlet UISegmentedControl *SizeSegment;

@end

@implementation ThumbnailViewController

@synthesize thumbnailH = _thumbnailH;
@synthesize thumbnailW = _thumbnailW;

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
    //CGRect rect = [self.view frame];
    //CGRect rect1 = [self.collectionView frame];
    ClassTabBarController *tabbarCV = (ClassTabBarController *)[self parentViewController];
    CGRect rect2 = [tabbarCV.view frame];
    [self.collectionView setFrame:rect2];
    [self small:nil];//default small size
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (IBAction)SwitchSize:(UISegmentedControl *)sender forEvent:(UIEvent *)event {
    if([sender selectedSegmentIndex] == 0){
        [self small:nil];
    }else if([sender selectedSegmentIndex] == 1){
        [self middle:nil];
    }else{
        [self large:nil];
    }
}

- (IBAction)small:(UIBarButtonItem *)sender {
    _thumbnailH = 242/2;
    _thumbnailW = 324/2;
    [self.collectionView reloadData];
}

- (IBAction)middle:(UIBarButtonItem *)sender {
    _thumbnailH = 480/2;
    _thumbnailW = 640/2;
    [self.collectionView reloadData];
}

- (IBAction)large:(UIBarButtonItem *)sender {
    _thumbnailH = 734/2;
    _thumbnailW = 980/2;
    [self.collectionView reloadData];
}

- (IBAction)unwindToThumbnail:(UIStoryboardSegue *)segue {
    //
}

- (IBAction)toMaterial:(UIButton *)sender {
    ClassTabBarController *childViewController = (ClassTabBarController *)[self parentViewController];
    [childViewController setSelectedIndex:ClassViewType_Material];
}

- (BOOL)textFieldShouldReturn:(UITextField *)textField {
    // Call your calculation and clearing method.
    [self.collectionView reloadData];
    
    return YES;
}

- (void)setContentID:(NSString *)contentID
{
    _contentID = contentID;
    [self.collectionView reloadData];
}

#pragma mark - UICollectionView Datasource
// 1
- (NSInteger)collectionView:(UICollectionView *)view numberOfItemsInSection:(NSInteger)section {
    
    NSArray *searchPaths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *documentPath = [searchPaths objectAtIndex:0];
    NSString *zippathTemp = [documentPath stringByAppendingPathComponent:@"zip"];
    NSString *docsDir = [zippathTemp stringByAppendingPathComponent:self.contentID];
    NSError *error;
    //NSArray* array = [[NSFileManager defaultManager] componentsToDisplayForPath:docsDir];
    NSArray* files = [[NSFileManager defaultManager] contentsOfDirectoryAtPath:docsDir error:&error];
    //NSString *docsDir = [NSHomeDirectory() stringByAppendingPathComponent:  @"Documents"];
    /*
    NSFileManager *localFileManager=[[NSFileManager alloc] init];
    NSDirectoryEnumerator *dirEnum =
    [localFileManager enumeratorAtPath:docsDir];
    
    NSString *file;
    while ((file = [dirEnum nextObject])) {
        if ([[file pathExtension] isEqualToString: @"png"]) {
            // process the document
            NSLog(@"%@",file);
        }
    }
     */
    return files.count;
}
// 2
- (NSInteger)numberOfSectionsInCollectionView: (UICollectionView *)collectionView {
    return 1;
}
// 3
- (UICollectionViewCell *)collectionView:(UICollectionView *)cv cellForItemAtIndexPath:(NSIndexPath *)indexPath {
    ThumbnailCell *cell = (ThumbnailCell *)[cv dequeueReusableCellWithReuseIdentifier:@"ThumbnailCell" forIndexPath:indexPath];

    if (self.contentID) {
        NSArray *searchPaths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
        NSString *documentPath = [searchPaths objectAtIndex:0];
        NSString *zippathTemp = [documentPath stringByAppendingPathComponent:@"zip"];
        NSString *docsDir = [zippathTemp stringByAppendingPathComponent:self.contentID];
        NSError *error;
        //NSArray* array = [[NSFileManager defaultManager] componentsToDisplayForPath:docsDir];
        NSArray* files = [[NSFileManager defaultManager] contentsOfDirectoryAtPath:docsDir error:&error];

        //cell.backgroundColor = [UIColor whiteColor];
        NSString *file = [files objectAtIndex:[indexPath indexAtPosition:1]];
        NSString *filepath = [docsDir stringByAppendingPathComponent:file];
        NSLog(@"%@",filepath);
        //UIImage *image = [UIImage imageNamed:filepath];
        UIImage *image = [[UIImage alloc] initWithContentsOfFile:filepath];
        cell.image.image = image;
    }
    return cell;
}
// 4
/*- (UICollectionReusableView *)collectionView:
 (UICollectionView *)collectionView viewForSupplementaryElementOfKind:(NSString *)kind atIndexPath:(NSIndexPath *)indexPath
 {
 return [[UICollectionReusableView alloc] init];
 }*/

#pragma mark - UICollectionViewDelegate
- (void)collectionView:(UICollectionView *)collectionView didSelectItemAtIndexPath:(NSIndexPath *)indexPath
{
    // TODO: Select Item
}
- (void)collectionView:(UICollectionView *)collectionView didDeselectItemAtIndexPath:(NSIndexPath *)indexPath {
    // TODO: Deselect item
}

#pragma mark – UICollectionViewDelegateFlowLayout

// 1
- (CGSize)collectionView:(UICollectionView *)collectionView layout:(UICollectionViewLayout*)collectionViewLayout sizeForItemAtIndexPath:(NSIndexPath *)indexPath {
    // 2
    CGSize retval = CGSizeMake(_thumbnailW, _thumbnailH);
    return retval;
}

// 3
- (UIEdgeInsets)collectionView:
(UICollectionView *)collectionView layout:(UICollectionViewLayout*)collectionViewLayout insetForSectionAtIndex:(NSInteger)section {
    return UIEdgeInsetsMake(7, 7, 7, 7);
}

@end
