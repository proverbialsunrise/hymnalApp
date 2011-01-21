//
//  HymnViewController.mm
//  hymnalSqlite
//
//  Created by Daniel Johnson on 10-12-12.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "HymnViewController.h"
#import "DatabaseHelper.h"
#import "VerseViewController.h"

@interface HymnViewController () //Private Interface

- (void) redrawVersesForOrientation:(UIInterfaceOrientation)fromInterfaceOrientation;

- (void) hideNavigationBar;

- (void) showNavigationBar;

- (void) scrollToVerse:(NSInteger)verseNum animated:(BOOL)animate;

- (void) toggleFavourite;

- (void) setupFavouriteButtonForHymnStatus;

@end


@implementation HymnViewController

@synthesize scrollView;

#pragma mark ViewController LifeCycle

 // The designated initializer.  Override if you create the controller programmatically and want to perform customization that is not appropriate for viewDidLoad.
- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
    if ((self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil])) {
		verseViewControllers = [NSMutableArray new];
		self.hidesBottomBarWhenPushed = YES;
    }
    return self;
}

- (id) initWithHymn:(Hymn)h {
	if ([self initWithNibName:@"HymnViewController" bundle:[NSBundle mainBundle]]) {
		hymn = h;
	}
	return self;
}

// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
    [super viewDidLoad];
	currentVerse = 1;
	rotating = NO;
	navigationBarTimer = nil;
	self.title = [NSString stringWithFormat:@"%s", hymn.get_title().c_str()];
	
	//Load a set of VerseViewControllers.  If necessary, we can load these on demand...but no point until it seems necessary.
	int numVerses = getNumVersesForHymn(hymn.get_hymnID());
	for (int i = 1; i <= numVerses; i++) {
		VerseViewController *verseViewController = [[[VerseViewController alloc] initWithHymn:hymn verse:i] autorelease];
		[verseViewController setDelegate:self];
		[self.scrollView addSubview:verseViewController.view];
		[verseViewControllers addObject:verseViewController];
	}
	[self.scrollView setDelegate:self];		
	[self setupFavouriteButtonForHymnStatus];
}

- (void) viewWillAppear:(BOOL)animated {
	[self redrawVersesForOrientation:self.interfaceOrientation];
}

- (void) viewDidAppear:(BOOL)animated {
	navigationBarTimer = [NSTimer scheduledTimerWithTimeInterval:2.0 target:self selector:@selector(hideNavigationBar) userInfo:nil repeats:NO];
}

- (void) viewWillDisappear:(BOOL)animated {
	if (navigationBarTimer != nil) {
		[navigationBarTimer invalidate];
		navigationBarTimer = nil;
	}
}

- (void) viewDidDisappear:(BOOL)animated{
	
}

- (void) didReceiveMemoryWarning {
    // Releases the view if it doesn't have a superview.
	[scrollView release];
	[verseViewControllers release];
    [super didReceiveMemoryWarning];
    
    // Release any cached data, images, etc that aren't in use.
}

- (void) viewDidUnload {
    [super viewDidUnload];
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}

- (void) dealloc {
	[scrollView release];
	[verseViewControllers release];
    [super dealloc];
}

#pragma mark -


#pragma mark Rotation

// Override to allow orientations other than the default portrait orientation.
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    // Return YES for supported orientations
    return !(interfaceOrientation == UIInterfaceOrientationPortraitUpsideDown);
}

- (void)willRotateToInterfaceOrientation:(UIInterfaceOrientation)toInterfaceOrientation duration:(NSTimeInterval)duration{
	rotating = YES;
}

- (void)didRotateFromInterfaceOrientation:(UIInterfaceOrientation)fromInterfaceOrientation {
	[self redrawVersesForOrientation:fromInterfaceOrientation];
	rotating = NO;
}

#pragma mark -
#pragma mark UIScrollViewDelegate

- (void) scrollViewDidScroll:(UIScrollView *)sView {
	if (!rotating) { //this method gets called sometimes during a rotation.  We don't want that. 
		int currentWidth = (int)self.scrollView.frame.size.width;
		CGPoint offset = sView.contentOffset;
		int xOffset = (int)(offset.x);
		if (xOffset % currentWidth == 0) {
			//scrolled to a page break. 
			//reset the last current verse if it's not the same as the current one.
			NSInteger oldVerse = currentVerse;
			//set the new current verse.
			currentVerse = (xOffset / currentWidth) + 1;
			if (oldVerse != currentVerse) {
				VerseViewController *verseViewController = [verseViewControllers objectAtIndex:(oldVerse - 1)];
				[verseViewController reset];
			}
			NSLog(@"Scrolled To Verse: %d", currentVerse);
		}
		NSLog(@"Offset: %d", xOffset);
	}
}

- (void)scrollViewDidEndScrollingAnimation:(UIScrollView *)sView {
	//NSLog(@"Animation ended");
}

#pragma mark -

#pragma mark VerseViewControllerDelegate
- (void) verseViewControllerTouchUpEvent:(VerseViewController *)verseViewController {
	[self showNavigationBar];
}

#pragma mark -

#pragma mark Private Interface 
- (void) scrollToVerse:(NSInteger)verseNum animated:(BOOL)animate {
	CGFloat curWidth = self.scrollView.frame.size.width;
	NSLog(@"Scroll to verse number: %d", verseNum);
	[self.scrollView scrollRectToVisible:CGRectMake(curWidth * (verseNum - 1), 0, curWidth, 100) animated:animate];
}

- (void) hideNavigationBar{
	self.wantsFullScreenLayout = YES;
	[[UIApplication sharedApplication] setStatusBarHidden:YES withAnimation:UIStatusBarAnimationSlide];
	[[self navigationController] setNavigationBarHidden:YES animated:YES];
	if (navigationBarTimer != nil) {
		[navigationBarTimer invalidate];
		navigationBarTimer = nil;
	}
}

- (void) showNavigationBar{
	[navigationBarTimer invalidate];
	self.wantsFullScreenLayout = NO;
	[[UIApplication sharedApplication] setStatusBarHidden:NO withAnimation:UIStatusBarAnimationSlide];
	[[self navigationController] setNavigationBarHidden:NO animated:YES];
	navigationBarTimer = [NSTimer scheduledTimerWithTimeInterval:2.0 target:self selector:@selector(hideNavigationBar) userInfo:nil repeats:NO];
}

- (void) redrawVersesForOrientation:(UIInterfaceOrientation)fromInterfaceOrientation{
	//Redraw the Verses the appropriate size. 
	CGFloat xOffset = 0.0;
	for (VerseViewController  *v in verseViewControllers) {
		CGRect viewFrame = v.view.frame;	
		[v.view setFrame:CGRectMake(xOffset, 0, viewFrame.size.width, viewFrame.size.height)];
		xOffset += viewFrame.size.width;
		[v didRotateFromInterfaceOrientation:fromInterfaceOrientation];
	}
	[self.scrollView setContentSize:CGSizeMake(xOffset, self.view.frame.size.height)];
	[self scrollToVerse:currentVerse animated:NO];
}

- (void) toggleFavourite{
	hymn.set_favourite(!hymn.get_favourite());
	setFavouriteStatusForHymn(hymn.get_hymnal().get_hymnalID(), hymn.get_hymnID(), hymn.get_favourite());
	[self setupFavouriteButtonForHymnStatus];
}

- (void) setupFavouriteButtonForHymnStatus{
	if (hymn.get_favourite()) {
		//Setup Gold Button from image. 
		UIImage *goldButtonImage = [UIImage imageNamed:@"GoldStarButton"];
		UIButton *favouriteButton = [UIButton buttonWithType:UIButtonTypeCustom];
		[favouriteButton setFrame:CGRectMake(0, 0, goldButtonImage.size.width, goldButtonImage.size.height)];
		[favouriteButton setImage:goldButtonImage forState:UIControlStateNormal];
		[favouriteButton setAdjustsImageWhenDisabled:YES];
		[favouriteButton setAdjustsImageWhenHighlighted:YES];
		[favouriteButton addTarget:self action:@selector(toggleFavourite) forControlEvents:UIControlEventTouchUpInside];
		UIBarButtonItem *favouriteBarButton = [[[UIBarButtonItem alloc] initWithCustomView:favouriteButton] autorelease];
		self.navigationItem.rightBarButtonItem = favouriteBarButton;
	} else {
		self.navigationItem.rightBarButtonItem = [[[UIBarButtonItem alloc] initWithImage:[UIImage imageNamed:@"ClosedStar"] style:UIBarButtonItemStyleBordered target:self action:@selector(toggleFavourite)] autorelease];
	}

}

#pragma mark -

@end
