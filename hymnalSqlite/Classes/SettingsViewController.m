//
//  SettingsViewController.m
//  hymnalSqlite
//
//  Created by Daniel Johnson on 11-01-04.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import "SettingsViewController.h"
#import "SettingsManager.h"
#include "Global.h"

@interface SettingsViewController ()

- (void) updateExplanatoryLabels;

- (void) loadSettings;

@end



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
	/*
	UISegmentedControl* voiceSControl = [[[UISegmentedControl alloc] initWithItems:[NSArray arrayWithObjects:@"All", @"Treble", @"Bass", nil]] autorelease];
	[voiceSControl setSegmentedControlStyle:UISegmentedControlStyleBezeled];
	[voiceSControl setSelectedSegmentIndex:[[SettingsManager sharedInstance] musicOption]];
	[voiceSControl setFrame:CGRectMake(20, 90, 280, 44)];
	[voiceSControl addTarget:self action:@selector(segmentedControlValueChanged:) forControlEvents:UIControlEventValueChanged];
	self.voicePartSegmentedControl = voiceSControl;
	[self.view addSubview:voicePartSegmentedControl];

	
	self.voicePartExplanatoryLabel = [[[UILabel alloc] initWithFrame:CGRectMake(26, 140, 274, 80)] autorelease];
	voicePartExplanatoryLabel.numberOfLines = 2;
	[self.view addSubview:voicePartExplanatoryLabel];
	
	UISegmentedControl* lyricSControl = [[[UISegmentedControl alloc] initWithItems:[NSArray arrayWithObjects:@"Horizontal", @"Vertical", @"Inline", nil]] autorelease];
	[lyricSControl setSegmentedControlStyle:UISegmentedControlStyleBezeled];
	[lyricSControl setSelectedSegmentIndex:[[SettingsManager sharedInstance] verseOption]];
	[lyricSControl setFrame:CGRectMake(20, 254, 280, 44)];
	[lyricSControl addTarget:self action:@selector(segmentedControlValueChanged:) forControlEvents:UIControlEventValueChanged];
	self.verseDisplaySegmentedControl = lyricSControl;
	[self.view addSubview:verseDisplaySegmentedControl];

	
	self.verseDisplayExplanatoryLabel = [[[UILabel alloc] initWithFrame:CGRectMake(26, 305, 274, 80)] autorelease];
	verseDisplayExplanatoryLabel.numberOfLines = 2;
	 [self.view addSubview:verseDisplayExplanatoryLabel];
	 */
	
	[self loadSettings];
	[self updateExplanatoryLabels];
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

- (void) loadSettings {
	SettingsManager *settingsManager = [SettingsManager sharedInstance];
	verseDisplaySegmentedControl.selectedSegmentIndex = [settingsManager verseOption];
	voicePartSegmentedControl.selectedSegmentIndex = [settingsManager musicOption];
}

- (void) updateExplanatoryLabels {
	switch (voicePartSegmentedControl.selectedSegmentIndex) {
		case ALLPARTS:
			voicePartExplanatoryLabel.text = @"All voice parts will be displayed.";
			break;
		case TREBLE:
			voicePartExplanatoryLabel.text = @"Only soprano and alto parts will be displayed.";
			break;
		case BASS:
			voicePartExplanatoryLabel.text = @"Only tenor and bass parts will be displayed.";
		default:
			break;
	}
	
	switch (verseDisplaySegmentedControl.selectedSegmentIndex) {
		case HORIZONTAL:
			verseDisplayExplanatoryLabel.text = @"Swipe side to side to view more verses.";
			break;
		case VERTICAL:
			verseDisplayExplanatoryLabel.text = @"Scroll down to view more verses.";
			break;
		case INLINE:
			verseDisplayExplanatoryLabel.text = @"All verse will be displayed inline";
		default:
			break;
	}
}


- (IBAction) segmentedControlValueChanged:(id)sender{
	[self updateExplanatoryLabels];
}

- (IBAction) saveButtonPressed:(id)sender	{
	[[SettingsManager sharedInstance] setVerseOption:verseDisplaySegmentedControl.selectedSegmentIndex];
	[[SettingsManager sharedInstance] setMusicOption:voicePartSegmentedControl.selectedSegmentIndex];
	[self.parentViewController dismissModalViewControllerAnimated:YES];
}

- (IBAction) cancelButtonPressed:(id)sender{
	[self.parentViewController dismissModalViewControllerAnimated:YES];
}
@end
