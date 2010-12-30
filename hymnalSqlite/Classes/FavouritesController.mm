    //
//  FavouritesController.m
//  hymnalSqlite
//
//  Created by Daniel Johnson on 10-12-24.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "FavouritesController.h"


@implementation FavouritesController

- (void)viewDidLoad {

	hymnal = getHymnal(1);
	
	hymns = getFavouriteHymnsForHymnal(hymnal.get_hymnalID(), SORT_BY_NUMBER);

	[super viewDidLoad];
	[self setTitle:@"Favorites"];
}

- (void)viewWillAppear:(BOOL)animated {
	[super viewWillAppear:animated];
	[self setTitle:@"Favorites"];
	hymns = getFavouriteHymnsForHymnal(hymnal.get_hymnalID(), SORT_BY_NUMBER);
	[self.tableView reloadData];
}

@end
