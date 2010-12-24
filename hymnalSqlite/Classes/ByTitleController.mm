    //
//  ByTitleController.m
//  hymnalSqlite
//
//  Created by Daniel Johnson on 10-12-24.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "ByTitleController.h"


@implementation ByTitleController


- (void)viewDidLoad {
	hymnal = getHymnal(1);
	hymns = getHymnsForHymnal(hymnal.get_hymnalID(), SORT_BY_NAME);
    [super viewDidLoad];
}

@end
