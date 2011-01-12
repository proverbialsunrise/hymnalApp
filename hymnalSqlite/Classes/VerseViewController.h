//
//  VerseViewController.h
//  hymnalSqlite
//
//  Created by Daniel Johnson on 10-12-05.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "DatabaseHelper.h"

@class VerseViewController;

@protocol VerseViewControllerDelegate <NSObject>

- (void) verseViewControllerTouchUpEvent:(VerseViewController *)verseViewController;

@end


@interface VerseViewController : UIViewController <UIScrollViewDelegate> {
	Hymn hymn;
	HymnSectionVector hymnSections;
	IBOutlet UIScrollView *scrollView;
	
	UIView *contentView;
	CGFloat contentWidth;
	unsigned int verseNumber;
	
	id <VerseViewControllerDelegate> delegate;
	
	NSMutableArray *imageViews;
}

@property (nonatomic, retain) IBOutlet UIScrollView *scrollView;
@property (nonatomic, assign) id <VerseViewControllerDelegate> delegate;

- (id) initWithHymn:(Hymn)h verse:(unsigned int)verseNum;

- (void) removeViewFromSuperview;

- (void) reset;

@end
