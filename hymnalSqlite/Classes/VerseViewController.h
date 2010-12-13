//
//  VerseViewController.h
//  hymnalSqlite
//
//  Created by Daniel Johnson on 10-12-05.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "DatabaseHelper.h"


@interface VerseViewController : UIViewController <UIScrollViewDelegate> {
	Hymn hymn;
	HymnSectionVector hymnSections;
	IBOutlet UIScrollView *scrollView;
	
	UIView *contentView;
	CGFloat contentWidth;
	unsigned int verseNumber;
}

@property (nonatomic, retain) IBOutlet UIScrollView *scrollView;

- (id) initWithHymn:(Hymn)h verse:(unsigned int)verseNum;

- (void) refreshImages;

- (void) reset;

@end
