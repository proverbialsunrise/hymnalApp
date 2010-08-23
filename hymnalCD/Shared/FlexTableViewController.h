//
//  FlexTableViewController.h
//  hymnalCD
//
//  Created by Daniel Johnson on 10-07-01.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>


@interface FlexTableViewController : UITableViewController {
	NSMutableArray *tableViewItems;
}

@property (nonatomic, retain) NSMutableArray *tableViewItems;

- (id) initWithTableViewItems:(NSMutableArray *)items;

@end
