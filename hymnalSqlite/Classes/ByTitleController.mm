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
    [super viewDidLoad];
	hymnal = getHymnal(1);
	
	hymns = getHymnsForHymnal(hymnal.get_hymnalID(), SORT_BY_NAME);
	[self setTitle:[NSString stringWithCString:hymnal.get_title().c_str() encoding:NSUTF8StringEncoding]];
	
	//create a vector to store the filtered hymns
 	filteredHymns = new HymnVector;
	
	if (savedSearchTerm) {
		[self.searchDisplayController setActive:searchWasActive];
		[self.searchDisplayController.searchBar setSelectedScopeButtonIndex:savedScopeButtonIndex];
		[self.searchDisplayController.searchBar setText:savedSearchTerm];
		
		savedSearchTerm = nil;
	}
	
	[self.searchDisplayController.searchBar setScopeButtonTitles:[NSArray arrayWithObjects:@"All", @"Title", @"Number", nil]];
	[self.tableView reloadData];
}



#pragma mark Content Filtering

- (void)filterContentForSearchText:(NSString*)searchText scope:(NSString*)scope
{
	/*
	 Update the filtered array based on the search text and scope.
	 */
	//Eventually I think this should be a call to the database rather than an iteration through the hymns in the list. 
	//Nevertheless, we'll start here and see if there is performance trouble. 
	//If necessary, kick off a background SQL search, return NO and reload the data when the search is complete. TODO?
	
	filteredHymns->clear(); // First clear the filtered vector.
	
	/*
	 Search the main list for hymns that match based on number, name, first line
	 */
	HymnVector::iterator it;
	for (it = hymns.begin(); it != hymns.end(); it++)
	{
		BOOL addToFiltered = NO;
		if (!addToFiltered && ([scope isEqualToString:@"Number"] || [scope isEqualToString:@"All"]) && it->get_hymnNumber() == [searchText intValue])
		{
			addToFiltered = YES;
		}
		if (!addToFiltered && ([scope isEqualToString:@"Title"] || [scope isEqualToString:@"All"]))
		{	
			NSString *title = [NSString stringWithCString:it->get_title().c_str() encoding:NSUTF8StringEncoding];
			NSComparisonResult result = [title compare:searchText options:(NSCaseInsensitiveSearch|NSDiacriticInsensitiveSearch) range:NSMakeRange(0, [searchText length])];
			if (result == NSOrderedSame)
			{
				addToFiltered = YES;
			}
		}
		if (addToFiltered) {
			filteredHymns->push_back(*it);
		}
	}
}


#pragma mark -

@end
