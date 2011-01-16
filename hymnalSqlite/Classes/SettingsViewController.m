//
//  SettingsViewController.m
//  hymnalSqlite
//
//  Created by Daniel Johnson on 11-01-04.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import "SettingsViewController.h"
#import "SettingsManager.h"



@implementation SettingsViewController

@synthesize voicePartExplanatoryLabel, verseDisplayExplanatoryLabel, voicePartSegmentedControl, verseDisplaySegmentedControl;

// The designated initializer.  Override if you create the controller programmatically and want to perform customization that is not appropriate for viewDidLoad.
/*
- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Custom initialization.
    }
    return self;
}
*/


// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
    [super viewDidLoad];
	[self setTitle:@"Settings"];
}


/*
// Override to allow orientations other than the default portrait orientation.
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    // Return YES for supported orientations.
    return (interfaceOrientation == UIInterfaceOrientationPortrait);
}
*/

- (void)didReceiveMemoryWarning {
    // Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
    
    // Release any cached data, images, etc. that aren't in use.
}

- (void)viewDidUnload {
    [super viewDidUnload];
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
	[voicePartExplanatoryLabel release];
	[verseDisplayExplanatoryLabel release];
	[voicePartSegmentedControl release];
	[verseDisplaySegmentedControl release];
}


- (void)dealloc {
	[voicePartExplanatoryLabel release];
	[verseDisplayExplanatoryLabel release];
	[voicePartSegmentedControl release];
	[verseDisplaySegmentedControl release];
    [super dealloc];
}


- (void) updateExplanatoryLabels {
	switch (verseDisplaySegmentedControl.selectedSegmentIndex) {
		case ALLPARTS:
			verseDisplayExplanatoryLabel.text = @"All voice parts will be displayed in displayed.";
			break;
		case ALTOSOPRANO:
			verseDisplayExplanatoryLabel.text = @"Only soprano and alto parts will be displayed.";
			break;
		case TENORBASS:
			verseDisplayExplanatoryLabel.text = @"Only tenor and bass parts will be displayed.";
		default:
			break;
	}
	
	switch (voicePartSegmentedControl.selectedSegmentIndex) {
		case HORIZONTAL:
			voicePartExplanatoryLabel.text = @"Swipe side to sideo to view more verses.";
			break;
		case VERTICAL:
			voicePartExplanatoryLabel.text = @"Scroll down to view more verses.";
			break;
		case INLINE:
			voicePartExplanatoryLabel.text = @"All verse will be displayed inline";
		default:
			break;
	}
}


- (IBAction) segmentedControlValueChanged:(id)sender{
	[self updateExplanatoryLabels];
	[[SettingsManager sharedInstance] setMusicOption:verseDisplaySegmentedControl.selectedSegmentIndex];
	[[SettingsManager sharedInstance] setMusicOption:voicePartSegmentedControl.selectedSegmentIndex];
}
@end
