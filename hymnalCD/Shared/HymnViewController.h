//
//  HymnViewController.h
//  hymnalCD
//
//  Created by Daniel Johnson on 10-08-26.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>

@class Hymn

@interface HymnViewController : UIViewController {
	Hymn *hymn;
}

@property (nonatomic, retain) hymn;

@end
