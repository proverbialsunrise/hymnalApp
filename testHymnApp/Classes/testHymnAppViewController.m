//
//  testHymnAppViewController.m
//  testHymnApp
//
//  Created by Daniel Johnson on 10-06-01.
//  Copyright __MyCompanyName__ 2010. All rights reserved.
//

#import "testHymnAppViewController.h"

@interface testHymnAppViewController (private)

- (UIImageView *)insertImageResourceWithName:(NSString *)imageName atYPosition:(CGFloat)yPos;

@end


@implementation testHymnAppViewController

#define BASS_NOTES 1
#define TREBLE_NOTES 1
#define WORDS 1

@synthesize scrollView;

/*
// The designated initializer. Override to perform setup that is required before the view is loaded.
- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
    if ((self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil])) {
        // Custom initialization
    }
    return self;
}
*/

/*
// Implement loadView to create a view hierarchy programmatically, without using a nib.
- (void)loadView {
}
*/



// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
    [super viewDidLoad];	
	//Initialize the contentView.
	//default size is 0, but this will soon change to the correct size.
	contentView = [[UIView alloc] initWithFrame:CGRectMake(0, 0, 0, 0)];
	
	//Add the necessary imageViews to the contentView.
	CGFloat yPos = 0.0;
	contentWidth = 0.0;
	contentHeight = 0.0;
	if (TREBLE_NOTES) {
		UIImageView *imageView = [self insertImageResourceWithName:@"treble1.png" atYPosition:yPos];
		yPos += imageView.frame.size.height;
		contentWidth = imageView.frame.size.width;
	}
	if (WORDS) {
		UIImageView *imageView = [self insertImageResourceWithName:@"text1_1.png" atYPosition:yPos];
		yPos += imageView.frame.size.height;
	}
	if (BASS_NOTES) {
		UIImageView *imageView = [self insertImageResourceWithName:@"bass1.png" atYPosition:yPos];
		yPos += imageView.frame.size.height;
	}
	if (TREBLE_NOTES) {
		UIImageView *imageView = [self insertImageResourceWithName:@"treble2.png" atYPosition:yPos];
		yPos += imageView.frame.size.height;
	}
	if (WORDS) {
		UIImageView *imageView = [self insertImageResourceWithName:@"text1_2.png" atYPosition:yPos];
		yPos += imageView.frame.size.height;
	}
	if (BASS_NOTES) {
		UIImageView *imageView = [self insertImageResourceWithName:@"bass2.png" atYPosition:yPos];
		yPos += imageView.frame.size.height;
	}
	if (TREBLE_NOTES) {
		UIImageView *imageView = [self insertImageResourceWithName:@"treble3.png" atYPosition:yPos];
		yPos += imageView.frame.size.height;
	}
	if (WORDS) {
		UIImageView *imageView = [self insertImageResourceWithName:@"text1_3.png" atYPosition:yPos];
		yPos += imageView.frame.size.height;	
	}
	if (BASS_NOTES) {
		UIImageView *imageView = [self insertImageResourceWithName:@"bass3.png" atYPosition:yPos];
		yPos += imageView.frame.size.height;
	}
	if (TREBLE_NOTES) {
		UIImageView *imageView = [self insertImageResourceWithName:@"treble4.png" atYPosition:yPos];
		yPos += imageView.frame.size.height;
	}
	if (WORDS) {
		UIImageView *imageView = [self insertImageResourceWithName:@"text1_4.png" atYPosition:yPos];
		yPos += imageView.frame.size.height;	
	}
	if (BASS_NOTES) {
		UIImageView *imageView = [self insertImageResourceWithName:@"bass4.png" atYPosition:yPos];
		yPos += imageView.frame.size.height;
	}
	//Reset frame for ContentView.
	[contentView setFrame:CGRectMake(0,0,contentWidth,contentHeight)];
	[scrollView addSubview:contentView];
	
	//Set up the scrollView.
	[scrollView setDelegate:self];
	[scrollView setContentSize:contentView.frame.size];
	[scrollView setMaximumZoomScale:1.0];
	float minimumScale = scrollView.frame.size.width/contentWidth;
	NSLog(@"minimumScale: %f",minimumScale);
	[scrollView setMinimumZoomScale:minimumScale];
	[scrollView setZoomScale:minimumScale];
	[scrollView zoomToRect:CGRectMake(0, 0, 1000, 1000) animated:NO];
}

-(UIImageView *) insertImageResourceWithName:(NSString *)name atYPosition:(CGFloat)yPos {
	UIImage *image = [UIImage imageNamed:name];
	UIImageView *imageView = [[[UIImageView alloc] initWithImage:image] autorelease];
	imageView.frame = CGRectMake(0, yPos, image.size.width, image.size.height);
	[contentView addSubview:imageView];
	contentHeight += image.size.height;
	return imageView;
}


#pragma mark UIScrollViewDelegate Methods

-(UIView *) viewForZoomingInScrollView:(UIScrollView *)scrollView {
	return contentView;
}

#pragma mark -


//Rotation Methods.
// Override to allow orientations other than the default portrait orientation.
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    // Return YES for supported orientations
    //return (interfaceOrientation == UIInterfaceOrientationPortrait);
	return YES;
}

- (void)didRotateFromInterfaceOrientation:(UIInterfaceOrientation)fromInterfaceOrientation {
	float minimumScale = scrollView.frame.size.width/contentWidth;
	NSLog(@"minimumScale: %f",minimumScale);
	[scrollView setMinimumZoomScale:minimumScale];
	[scrollView setZoomScale:minimumScale];
	[scrollView zoomToRect:CGRectMake(0, 0, 1000, 1000) animated:NO];
}



- (void)didReceiveMemoryWarning {
	// Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
	
	// Release any cached data, images, etc that aren't in use.
	//If the view is not visible, release the imageViews.
}

- (void)viewDidUnload {
	// Release any retained subviews of the main view.
	// e.g. self.myOutlet = nil;
	[self.scrollView release];
	self.scrollView = nil;
	//Release all images and the content view.
	[contentView release];
	contentView = nil;
}


- (void)dealloc {
	
    [super dealloc];
}

@end
