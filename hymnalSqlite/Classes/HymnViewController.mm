//
//  HymnViewController
//  hymnalSqlite
//
//  Created by Daniel Johnson on 10-12-05.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "HymnViewController.h"


@implementation HymnViewController

/*
 // The designated initializer.  Override if you create the controller programmatically and want to perform customization that is not appropriate for viewDidLoad.
- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
    if ((self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil])) {
        // Custom initialization
    }
    return self;
}
*/

- (id) initWithHymn:(Hymn)h{
	if (self = [super init]) {
		hymn = h;
	}
	return self;
}


// Implement loadView to create a view hierarchy programmatically, without using a nib.
- (void)loadView {
	[self setTitle:[NSString stringWithFormat:@"%s", hymn.get_title().c_str()]];
	CGRect windowFrame = [[UIScreen mainScreen] bounds];
	self.view = [[UIView alloc] initWithFrame:windowFrame];
	scrollView = [[UIScrollView alloc] initWithFrame:windowFrame];
	scrollView.scrollEnabled = YES;
	scrollView.scrollsToTop = NO;
	scrollView.delegate = self;
	[self.view addSubview:scrollView];
	
	contentView = [[UIView alloc] initWithFrame:CGRectMake(0, 0, 0, 0)];
	
	HymnSectionVector hymnSections = getPiecesForHymn(hymn.get_hymnID(), ALLVERSES, ALLPARTS);
	HymnSectionVector::iterator it;
	
	CGFloat yPos = 0.0;
	contentWidth = 0.0;
	contentHeight = 0.0;
	
	for (it = hymnSections.begin(); it < hymnSections.end(); it++) {
		printf("%d", it->lineNumber);
		printf("%s", it->imagePath.c_str());
		printf("%d", it->part);
	}
}


/*
// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
    [super viewDidLoad];
}
*/


// Override to allow orientations other than the default portrait orientation.
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    // Return YES for supported orientations
    return (interfaceOrientation == UIInterfaceOrientationPortrait || interfaceOrientation == UIInterfaceOrientationLandscapeLeft || interfaceOrientation == UIInterfaceOrientationLandscapeRight);
}


- (void)didReceiveMemoryWarning {
    // Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
    
    // Release any cached data, images, etc that aren't in use.
}

- (void)viewDidUnload {
    [super viewDidUnload];
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}


- (void)dealloc {
	[scrollView release];
	[contentView release];
    [super dealloc];
}


@end
