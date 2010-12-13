//
//  HymnViewController.h
//  hymnalSqlite
//
//  Created by Daniel Johnson on 10-12-12.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "Hymn.h"

@interface HymnViewController : UIViewController <UIScrollViewDelegate> {
	UIScrollView *scrollView;
	NSMutableArray* verseViewControllers;
	Hymn hymn;
	NSInteger currentVerse;
	BOOL rotating;
}

@property (nonatomic, retain) IBOutlet UIScrollView *scrollView;

- (id) initWithHymn:(Hymn)h;

@end
