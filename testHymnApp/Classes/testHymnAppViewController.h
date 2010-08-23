//
//  testHymnAppViewController.h
//  testHymnApp
//
//  Created by Daniel Johnson on 10-06-01.
//  Copyright __MyCompanyName__ 2010. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface testHymnAppViewController : UIViewController <UIScrollViewDelegate> {
	UIScrollView *scrollView;
	UIView *contentView;
	CGFloat contentWidth;
	CGFloat contentHeight;
}

@property (nonatomic, retain) IBOutlet UIScrollView *scrollView;

@end

