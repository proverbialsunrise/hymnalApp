//
//  HymnListTableViewController.h
//  hymnalSqlite
//
//  Created by Daniel Johnson on 10-12-24.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "DatabaseHelper.h"


@interface HymnListTableViewController : UITableViewController {
	Hymnal hymnal;
	
	//Stores list content.  
	HymnVector hymns;
	HymnVector *filteredHymns;
	
	//Stores search state if the view is unloaded
	NSString *savedSearchTerm;
	NSInteger savedScopeButtonIndex;
	BOOL searchWasActive;
}

@end
