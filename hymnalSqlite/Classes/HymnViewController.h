//
//  HymnViewController.h
//  hymnalSqlite
//
//  Created by Daniel Johnson on 10-12-05.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "DatabaseHelper.h"

@interface HymnViewController : UIViewController <UIScrollViewDelegate> {
	Hymn hymn;	
	UIScrollView *scrollView;
	UIView *contentView;
	CGFloat contentWidth;
	CGFloat contentHeight;
}

- (id) initWithHymn:(Hymn)h;

@end
