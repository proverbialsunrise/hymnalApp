//
//  VerseViewController.m
//  hymnalSqlite
//
//  Created by Daniel Johnson on 10-12-05.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "VerseViewController.h"
#import "SettingsManager.h" 

@interface VerseViewController () //Private Interface
- (void) setToMinimumZoom;

- (void) removeAllImages;

- (void) touchUpInside;

- (void) refreshImages;

@end

@implementation VerseViewController
@synthesize scrollView, delegate;


- (id) initWithHymn:(Hymn)h verse:(unsigned int)verseNum{
	if (self = [super initWithNibName:@"VerseViewController" bundle:[NSBundle mainBundle]]) {
		hymn = h;
		verseNumber = verseNum;
		imageViews = [NSMutableArray new];
		
	}
	return self;
}

- (void) setToMinimumZoom {
	float minimumScale = scrollView.frame.size.width/contentWidth;
	[scrollView setMinimumZoomScale:minimumScale];
	[scrollView setZoomScale:minimumScale];
}

#pragma mark ViewController Lifecycle

// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
    [super viewDidLoad];
	[self setTitle:[NSString stringWithFormat:@"%s", hymn.get_title().c_str()]];
	hymnSections = getPiecesForHymn(hymn.get_hymnID(), verseNumber, (PartSpecifier)[[SettingsManager sharedInstance] musicOption]);
	[self refreshImages];
	UIButton* showNavBarButton = [[[UIButton alloc] initWithFrame:self.scrollView.frame] autorelease];
	[showNavBarButton addTarget:self action:@selector(touchUpInside) forControlEvents:UIControlEventTouchUpInside];
	[showNavBarButton setAutoresizingMask:UIViewAutoresizingFlexibleWidth | UIViewAutoresizingFlexibleHeight | UIViewAutoresizingFlexibleTopMargin | UIViewAutoresizingFlexibleBottomMargin | UIViewAutoresizingFlexibleRightMargin | UIViewAutoresizingFlexibleLeftMargin];
	[self.scrollView addSubview:showNavBarButton];
	
}

- (void)didReceiveMemoryWarning {
    // Releases the view if it doesn't have a superview.
	[self removeAllImages];
    [super didReceiveMemoryWarning];
    // Release any cached data, images, etc that aren't in use.
	//release the images.
}

- (void)viewDidUnload {
	hymnSections.clear();
	[imageViews release];
    [super viewDidUnload];
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}

#pragma mark -


- (void) reset {
	[self setToMinimumZoom];
	[scrollView scrollRectToVisible:CGRectMake(0, 0, 100, 100) animated:YES];
	[self.view setNeedsDisplay];
	NSLog(@"Reset Verse %d", verseNumber);
}

#pragma mark ScrollViewDelegate

-(UIView *) viewForZoomingInScrollView:(UIScrollView *)scrollView {
	return contentView;
}

#pragma mark -

#pragma mark Rotation
// Override to allow orientations other than the default portrait orientation.
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    // Return YES for supported orientations
    return YES;
}

- (void)didRotateFromInterfaceOrientation:(UIInterfaceOrientation)fromInterfaceOrientation {
	[self setToMinimumZoom];
}



#pragma mark  -

#pragma mark Private Interface 

- (void) removeAllImages {
	for (UIImageView *view in imageViews) {
		[view removeFromSuperview];
	}
	[imageViews removeAllObjects];
}

- (void) refreshImages {	
	[self removeAllImages];
	HymnSectionVector::iterator section;
	NSString *bundleName = (NSString*)[[[NSBundle mainBundle] infoDictionary]  objectForKey:@"CFBundleName"];
	NSString *appDirectory = [NSHomeDirectory() stringByAppendingPathComponent:[NSString stringWithFormat:@"%@.app", bundleName]];
	
	CGFloat contentHeight = 0.0;
	contentWidth = 0.0;
	
	contentView = [[UIView alloc] initWithFrame:CGRectMake(0, 0, 0, 0)];
	
	for (section = hymnSections.begin(); section < hymnSections.end(); section++) {
		NSString *path = [NSString stringWithFormat:@"%s",section->imagePath.c_str()];
		NSString *fullPath = [NSString stringWithFormat:@"%@/%@", appDirectory, path];
		NSLog(@"%@/%@", appDirectory, path);
		
		UIImage *sectionImage = [[UIImage alloc] initWithContentsOfFile:fullPath];
		UIImageView *sectionImageView = [[UIImageView alloc] initWithImage:sectionImage];
		[sectionImage release];
		sectionImageView.frame = CGRectMake(0, contentHeight, sectionImage.size.width, sectionImage.size.height);
		[imageViews addObject:sectionImageView];
		[contentView addSubview:sectionImageView];
		
		contentHeight += sectionImage.size.height;
		contentWidth = sectionImage.size.width;
		[sectionImageView release];
	}
	[contentView setFrame:CGRectMake(0, 0, contentWidth, contentHeight)];
	
	[scrollView addSubview:contentView];
	[scrollView setContentSize:contentView.frame.size];
	[scrollView setMaximumZoomScale:1.0];
	
	float minimumScale = scrollView.frame.size.width/contentWidth;
	[scrollView setMinimumZoomScale:minimumScale];
	[scrollView setZoomScale:minimumScale];
}

- (void) removeViewFromSuperview{
	[self.view removeFromSuperview];
	[self removeAllImages];
}


- (void) touchUpInside {
	[delegate verseViewControllerTouchUpEvent:self];
}

#pragma mark -



- (void)dealloc {
	[imageViews release];
	[scrollView release];
    [super dealloc];
}


@end
