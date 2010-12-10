//
//  HymnViewController.m
//  hymnalSqlite
//
//  Created by Daniel Johnson on 10-12-05.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "HymnViewController.h"


@implementation HymnViewController
@synthesize scrollView;

- (id) initWithHymn:(Hymn)h {
	if (self = [super initWithNibName:@"HymnViewController" bundle:[NSBundle mainBundle]]) {
		hymn = h;
	}
	return self;
}
/*
 // The designated initializer.  Override if you create the controller programmatically and want to perform customization that is not appropriate for viewDidLoad.
- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
    if ((self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil])) {
        // Custom initialization
    }
    return self;
}
*/


// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
    [super viewDidLoad];
	[self setTitle:[NSString stringWithFormat:@"%s", hymn.get_title().c_str()]];
	hymnSections = getPiecesForHymn(hymn.get_hymnID(), ALLVERSES, ALLPARTS);
	[self refreshImages];
	
}

- (void) removeAllImages {
	for (UIView *view in [self.view subviews]) {
		[view removeFromSuperview];
	}
}

- (void) refreshImages {	
	HymnSectionVector::iterator section;
	NSString *bundleName = (NSString*)[[[NSBundle mainBundle] infoDictionary]  objectForKey:@"CFBundleName"];
	NSString *appDirectory = [NSHomeDirectory() stringByAppendingPathComponent:[NSString stringWithFormat:@"%@.app", bundleName]];
	
	CGFloat contentHeight = 0.0;
	CGFloat contentWidth = 0.0;
	
	contentView = [[UIView alloc] initWithFrame:CGRectMake(0, 0, 0, 0)];
	
	for (section = hymnSections.begin(); section < hymnSections.end(); section++) {
		NSString *path = [NSString stringWithFormat:@"%s",section->imagePath.c_str()];
		NSString *fullPath = [NSString stringWithFormat:@"%@/%@", appDirectory, path];
		NSLog(@"%@/%@", appDirectory, path);
		
		UIImage *sectionImage = [UIImage imageWithContentsOfFile:fullPath];
		UIImageView *sectionImageView = [[UIImageView alloc] initWithImage:sectionImage];
		sectionImageView.frame = CGRectMake(0, contentHeight, sectionImage.size.width, sectionImage.size.height);
		[contentView addSubview:sectionImageView];
		
		contentHeight += sectionImage.size.height;
		contentWidth = sectionImage.size.width;
		[sectionImageView release];
	}
	[contentView setFrame:CGRectMake(0, 0, contentWidth, contentHeight)];

	[scrollView addSubview:contentView];
	[scrollView setContentSize:contentView.frame.size];
	[scrollView setMaximumZoomScale:1.0];
	float minimumScale = scrollView.frame.size.width/contentView.frame.size.width;

	[scrollView setMinimumZoomScale:minimumScale];
	[scrollView setZoomScale:minimumScale];
}

-(UIView *) viewForZoomingInScrollView:(UIScrollView *)scrollView {
	return contentView;
}


// Override to allow orientations other than the default portrait orientation.
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    // Return YES for supported orientations
    return YES;
}

- (void)didRotateFromInterfaceOrientation:(UIInterfaceOrientation)fromInterfaceOrientation {
	float minimumScale = scrollView.frame.size.width/contentView.frame.size.width;
	[scrollView setMinimumZoomScale:minimumScale];
	[scrollView setZoomScale:minimumScale];
}


- (void)didReceiveMemoryWarning {
    // Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
    // Release any cached data, images, etc that aren't in use.
	//release the images.
}

- (void)viewDidUnload {
    [super viewDidUnload];
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}


- (void)dealloc {
	[scrollView release];
    [super dealloc];
}


@end