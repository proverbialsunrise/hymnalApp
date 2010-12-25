//
//  ByNumberController.mm
//  hymnalSqlite
//
//  Created by Daniel Johnson on 10-11-28.
//  Copyright __MyCompanyName__ 2010. All rights reserved.
//

#import "ByNumberController.h"

@implementation ByNumberController

#pragma mark View lifecycle


- (void)viewDidLoad {
	hymnal = getHymnal(1);
	hymns = getHymnsForHymnal(hymnal.get_hymnalID(), SORT_BY_NUMBER);
	[super viewDidLoad];
	self.navigationController.title = @"By Number";
}

- (void)viewWillAppear:(BOOL)animated {
	[super viewWillAppear:animated];
	[self setTitle:@"By Number"];
}
@end

