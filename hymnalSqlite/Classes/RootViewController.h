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
	HymnVector hymns;
	Hymnal hymnal;
}

@end
