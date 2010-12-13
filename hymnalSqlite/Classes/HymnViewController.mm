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


@implementation HymnViewController

@synthesize scrollView;

 // The designated initializer.  Override if you create the controller programmatically and want to perform customization that is not appropriate for viewDidLoad.
- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
    if ((self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil])) {
		verseViewControllers = [NSMutableArray new];
    }
    return self;
}

- (id) initWithHymn:(Hymn)h {
	if ([self initWithNibName:@"HymnViewController" bundle:[NSBundle mainBundle]]) {
		hymn = h;
	}
	return self;
}

- (void) scrollToVerse:(NSInteger)verseNum animated:(BOOL)animate {
	CGFloat curWidth = self.scrollView.frame.size.width;
	NSLog(@"Scroll to verse number: %d", verseNum);
	[self.scrollView scrollRectToVisible:CGRectMake(curWidth * (verseNum - 1), 0, curWidth, 100) animated:animate];
}

// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
    [super viewDidLoad];
	//Load a set of VerseViewControllers.  If necessary, we can load these on demand...but no point until it seems necessary.
	currentVerse = 1;
	CGFloat xOffset = 0.0;
	int numVerses = getNumVersesForHymn(hymn.get_hymnID());
	for (int i = 1; i <= numVerses; i++) {
		VerseViewController *verseViewController = [[[VerseViewController alloc] initWithHymn:hymn verse:i] autorelease];
		[verseViewControllers addObject:verseViewController];
		CGRect viewFrame = verseViewController.view.frame;	
		[verseViewController.view setFrame:CGRectMake(xOffset, 0, viewFrame.size.width, viewFrame.size.height)];
		xOffset += viewFrame.size.width;
		[self.scrollView addSubview:verseViewController.view];
	}
	
	[self.scrollView setContentSize:CGSizeMake(xOffset, self.view.frame.size.height)];
	[self.scrollView setDelegate:self];
	
}



// Override to allow orientations other than the default portrait orientation.
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    // Return YES for supported orientations
	rotating = YES;
    return YES;
}

- (void)didRotateFromInterfaceOrientation:(UIInterfaceOrientation)fromInterfaceOrientation {
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
	rotating = NO;
}


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
				VerseViewController *verseViewController = [verseViewControllers objectAtIndex:(currentVerse - 1)];
				[verseViewController reset];
			}
			NSLog(@"Scrolled To Verse: %d", currentVerse);
		}
	}
}

- (void)scrollViewDidEndScrollingAnimation:(UIScrollView *)sView {
	NSLog(@"Animation ended");
}

#pragma mark -

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
	[verseViewControllers release];
    [super dealloc];
}


@end
