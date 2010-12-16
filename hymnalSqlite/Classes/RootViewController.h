//
//  RootViewController.h
//  hymnalSqlite
//
//  Created by Daniel Johnson on 10-11-28.
//  Copyright __MyCompanyName__ 2010. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "DatabaseHelper.h"

@interface RootViewController : UITableViewController {
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
