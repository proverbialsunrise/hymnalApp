//
//  HymnListTableViewController.m
//  hymnalSqlite
//
//  Created by Daniel Johnson on 10-12-24.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "HymnListTableViewController.h"
#import "HymnViewController.h"
#import "SettingsViewController.h"


@implementation HymnListTableViewController


#pragma mark -
#pragma mark View lifecycle

- (void) viewDidLoad {
	[self setTitle:[NSString stringWithCString:hymnal.get_title().c_str() encoding:NSUTF8StringEncoding]];
	

	if (savedSearchTerm) {
		[self.searchDisplayController setActive:searchWasActive];
		[self.searchDisplayController.searchBar setSelectedScopeButtonIndex:savedScopeButtonIndex];
		[self.searchDisplayController.searchBar setText:savedSearchTerm];
		
		savedSearchTerm = nil;
	}
	
	//[self.searchDisplayController.searchBar setScopeButtonTitles:[NSArray arrayWithObjects:@"All", @"Title", @"Number", nil]];
	[self.tableView reloadData];
	
	
	//Add the settings button.
	self.navigationItem.rightBarButtonItem = [[[UIBarButtonItem alloc] initWithImage:[UIImage imageNamed:@"gears"] style:UIBarButtonItemStyleBordered target:self action:@selector(openSettings)] autorelease];
	
	//Warning...
	if ([self isMemberOfClass:[HymnListTableViewController class]]) {
		NSLog(@"Warning: You must implement viewDidLoad in your subclass to load a list of hymns");
	}
}

- (void)viewWillAppear:(BOOL)animated {
	//[self setTitle:[NSString stringWithCString:hymnal.get_title().c_str() encoding:NSUTF8StringEncoding]];
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
		return filteredHymns.size();
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
		hymn = filteredHymns[row];
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
		hymn = filteredHymns[indexPath.row];
	} else {
		hymn = (hymns)[indexPath.row];
	}
	
	HymnViewController *hymnViewController = [[HymnViewController alloc] initWithHymn:hymn];
	[self.navigationController pushViewController:hymnViewController animated:YES];
	[hymnViewController release];
	
	//Add the hymn to recently viewed hymns.
	addHymnToRecents(hymnal.get_hymnalID(), hymn.get_hymnID());
}


#pragma mark -



#pragma mark Content Filtering

- (void)filterContentForSearchText:(NSString*)searchText scope:(NSString*)scope
{
	//Universal search.  Query the database by number if the search is a number, otherwise query by title. 
	
	/*
	 Search the main list for hymns that match based on number, name, first line
	 */
	NSInteger hymnNumber = [searchText integerValue];
	if (hymnNumber != 0) {
		filteredHymns = getHymnsForNumericSearch(1, hymnNumber);
	} else {
		std::string searchTextString = std::string([searchText UTF8String]);
		filteredHymns = getHymnsForTitleSearch(1, searchTextString);
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

#pragma mark SettingsControl

- (void) openSettings{
	SettingsViewController *settingsViewController = [[[SettingsViewController alloc] initWithNibName:@"SettingsViewController" bundle:[NSBundle mainBundle]] autorelease];
	[self presentModalViewController:settingsViewController animated:YES];
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
    [super dealloc];
}



@end

