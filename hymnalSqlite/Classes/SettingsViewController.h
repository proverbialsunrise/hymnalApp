//
//  SettingsViewController.h
//  hymnalSqlite
//
//  Created by Daniel Johnson on 11-01-04.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>


@interface SettingsViewController : UIViewController {
	UILabel *voicePartExplanatoryLabel;
	UILabel *verseDisplayExplanatoryLabel;
	
	UISegmentedControl *voicePartSegmentedControl;
	UISegmentedControl *verseDisplaySegmentedControl;
}

@property (nonatomic, retain) IBOutlet UILabel *voicePartExplanatoryLabel;
@property (nonatomic, retain) IBOutlet UILabel *verseDisplayExplanatoryLabel;
@property (nonatomic, retain) IBOutlet UISegmentedControl *voicePartSegmentedControl;
@property (nonatomic, retain) IBOutlet UISegmentedControl *verseDisplaySegmentedControl;


- (IBAction) segmentedControlValueChanged:(id)sender;

@end
