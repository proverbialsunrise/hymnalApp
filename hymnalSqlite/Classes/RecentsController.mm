//
//  RecentsController.mm
//  hymnalSqlite
//
//  Created by Daniel Johnson on 10-12-30.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "RecentsController.h"
#import "DatabaseHelper.h"


@implementation RecentsController

- (void)viewDidLoad {
	
	hymnal = getHymnal(1);
	
	hymns = getRecents(hymnal.get_hymnalID());
	
	[super viewDidLoad];
	[self setTitle:@"Recently Viewed"];
}

- (void)viewWillAppear:(BOOL)animated {
	[super viewWillAppear:animated];
	[self setTitle:@"Recently Viewed"];
	hymns = getRecents(hymnal.get_hymnalID());
	[self.tableView reloadData];
}

@end
