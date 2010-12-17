//
//  ByNumberController.mm
//  hymnalSqlite
//
//  Created by Daniel Johnson on 10-11-28.
//  Copyright __MyCompanyName__ 2010. All rights reserved.
//

#import "ByNumberController.h"
#import "HymnViewController.h"

@implementation ByNumberController


#pragma mark -
#pragma mark View lifecycle


- (void)viewDidLoad {
    [super viewDidLoad];
	hymnal = getHymnal(1);
	
	hymns = getHymnsForHymnal(hymnal.get_hymnalID(), SORT_BY_NUMBER);
	[self setTitle:[NSString stringWithCString:hymnal.get_title().c_str() encoding:NSUTF8StringEncoding]];

	//create a vector to store the filtered hymns
 	filteredHymns = new HymnVector;
	
	if (savedSearchTerm) {
		[self.searchDisplayController setActive:searchWasActive];
		[self.searchDisplayController.searchBar setSelectedScopeButtonIndex:savedScopeButtonIndex];
		[self.searchDisplayController.searchBar setText:savedSearchTerm];
		
		savedSearchTerm = nil;
	}
	
	[self.tableView reloadData];
}



- (void)viewWillAppear:(BOOL)animated {
	[self setTitle:[NSString stringWithCString:hymnal.get_title().c_str() encoding:NSUTF8StringEncoding]];
    [super viewWillAppear:animated];
}

/*
- (void)viewDidAppear:(BOOL)animated {
    [super viewDidAppear:animated];
}
*/
/*
- (void)viewWillDisappear:(BOOL)animated {
	[super viewWillDisappear:animated];
}
*/

- (void)viewDidDisappear:(BOOL)animated {
	[super viewDidDisappear:animated];
	// save the state of the search UI so that it can be restored if the view is re-created
    searchWasActive = [self.searchDisplayController isActive];
    savedSearchTerm = [self.searchDisplayController.searchBar text];
    savedScopeButtonIndex = [self.searchDisplayController.searchBar selectedScopeButtonIndex];
}



 // Override to allow orientations other than the default portrait orientation.
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
	// Return YES for supported orientations.
	return YES;
}
 


#pragma mark -
#pragma mark Table view data source

// Customize the number of sections in the table view.
- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView {
    return 1;
}


// Customize the number of rows in the table view.
- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
	/*
	 If the requesting table view is the search display controller's table view, return the count of
     the filtered list, otherwise return the count of the main list.
	 */
	if (tableView == self.searchDisplayController.searchResultsTableView)
	{
		return filteredHymns->size();
    }
	else
	{
		return hymns.size();
    }
}


// Customize the appearance of table view cells.
- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    
    static NSString *CellIdentifier = @"Cell";
    
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:CellIdentifier];
    if (cell == nil) {
        cell = [[[UITableViewCell alloc] initWithStyle:UITableViewCellStyleValue1 reuseIdentifier:CellIdentifier] autorelease];
    }
	int row = indexPath.row;

	Hymn hymn;
	
	if (tableView == self.searchDisplayController.searchResultsTableView) {
		hymn = (*filteredHymns)[row];
	} else {
		hymn = hymns[row];
	}

	cell.textLabel.text = [NSString stringWithFormat:@"%d", hymn.get_hymnNumber()];
	cell.textLabel.font = [UIFont fontWithName:@"Georgia" size:22.0];
	cell.detailTextLabel.text = [NSString stringWithCString:hymn.get_title().c_str() encoding:NSUTF8StringEncoding];
	cell.detailTextLabel.font = [UIFont fontWithName:@"Georgia" size:16.0];
	cell.detailTextLabel.textAlignment = UITextAlignmentLeft;
	cell.detailTextLabel.textColor = [UIColor blackColor];

    return cell;
}


#pragma mark -
#pragma mark Table view delegate

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath {
    [tableView deselectRowAtIndexPath:indexPath animated:YES];
	[self setTitle:[NSString stringWithCString:hymnal.get_shortName().c_str() encoding:NSUTF8StringEncoding]];
	Hymn hymn;
	if (tableView == self.searchDisplayController.searchResultsTableView) {
		hymn = (*filteredHymns)[indexPath.row];
	} else {
		hymn = (hymns)[indexPath.row];
	}
	
	HymnViewController *hymnViewController = [[HymnViewController alloc] initWithHymn:hymn];
	[self.navigationController pushViewController:hymnViewController animated:YES];
	[hymnViewController release];
}


#pragma mark -
#pragma mark Content Filtering

- (void)filterContentForSearchText:(NSString*)searchText scope:(NSString*)scope
{
	/*
	 Update the filtered array based on the search text and scope.
	 */
	//Eventually I think this should be a call to the database rather than an iteration through the hymns in the list. 
	//Nevertheless, we'll start here and see if there is performance trouble. 
	//If necessary, kick off a background SQL search, return NO and reload the data when the search is complete. 
	
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
		if (!addToFiltered && ([scope isEqualToString:@"First Line"] || [scope isEqualToString:@"All"])){
			NSString *firstLine = [NSString stringWithCString:it->get_firstLine().c_str() encoding:NSUTF8StringEncoding];
			NSComparisonResult result = [firstLine compare:searchText options:(NSCaseInsensitiveSearch|NSDiacriticInsensitiveSearch) range:NSMakeRange(0, [searchText length])];
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
#pragma mark UISearchDisplayController Delegate Methods

- (BOOL)searchDisplayController:(UISearchDisplayController *)controller shouldReloadTableForSearchString:(NSString *)searchString
{
    [self filterContentForSearchText:searchString scope:
	 [[self.searchDisplayController.searchBar scopeButtonTitles] objectAtIndex:[self.searchDisplayController.searchBar selectedScopeButtonIndex]]];
    
    // Return YES to cause the search result table view to be reloaded.
    return YES;
}


- (BOOL)searchDisplayController:(UISearchDisplayController *)controller shouldReloadTableForSearchScope:(NSInteger)searchOption
{
    [self filterContentForSearchText:[self.searchDisplayController.searchBar text] scope:
	 [[self.searchDisplayController.searchBar scopeButtonTitles] objectAtIndex:searchOption]];
    
    // Return YES to cause the search result table view to be reloaded.
    return YES;
}

#pragma mark -
#pragma mark Memory management

- (void)didReceiveMemoryWarning {
    // Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
    
    // Relinquish ownership any cached data, images, etc that aren't in use.
}

- (void)viewDidUnload {
    // Relinquish ownership of anything that can be recreated in viewDidLoad or on demand.
    // For example: self.myOutlet = nil;
}

- (void)dealloc {
	delete filteredHymns;
    [super dealloc];
}


@end

