//
//  AppDelegate_Phone.m
//  hymnalCD
//
//  Created by Daniel Johnson on 10-06-10.
//  Copyright __MyCompanyName__ 2010. All rights reserved.
//

#import "AppDelegate_Phone.h"
#import "FlexTableViewItem.h"
#import "FlexTableViewController.h"

@implementation AppDelegate_Phone


#pragma mark -
#pragma mark Application delegate

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {    
	
	tabBarController = [[UITabBarController alloc] init];
	
	FlexTableViewItem *item1 = [[[FlexTableViewItem alloc] initWithImage:[UIImage imageNamed:@"topicTabBarItem.png"] Title:@"Item 1 is the best"] autorelease];
	FlexTableViewItem *item2 = [[[FlexTableViewItem alloc] initWithImage:[UIImage imageNamed:@"topicTabBarItem.png"] Title:@"Item 2 is the best"] autorelease];
	FlexTableViewItem *item3 = [[[FlexTableViewItem alloc] initWithImage:[UIImage imageNamed:@"topicTabBarItem.png"] Title:@"Item 3 is the best"] autorelease];

	FlexTableViewItem *item4 = [[[FlexTableViewItem alloc] initWithImage:[UIImage imageNamed:@"topicTabBarItem.png"] Title:@"Item 4 is the best"] autorelease];
	FlexTableViewItem *item5 = [[[FlexTableViewItem alloc] initWithImage:[UIImage imageNamed:@"topicTabBarItem.png"] Title:@"Item 5 is the best"] autorelease];
	FlexTableViewItem *item6 = [[[FlexTableViewItem alloc] initWithImage:[UIImage imageNamed:@"topicTabBarItem.png"] Title:@"Item 6 is the best"] autorelease];
	
	
	
	FlexTableViewController *topicalTableView = [[[FlexTableViewController alloc] initWithTableViewItems:[NSMutableArray arrayWithObjects:item1, item2, item3, nil]] autorelease];
	FlexTableViewController *favouriteTableView = [[[FlexTableViewController alloc] initWithTableViewItems:[NSMutableArray arrayWithObjects:item4, item5, item6, nil]] autorelease];
	
	UINavigationController *topicalNavigationController = [[[UINavigationController alloc] initWithRootViewController:topicalTableView] autorelease];
	topicalNavigationController.tabBarItem = [[[UITabBarItem alloc] initWithTitle:@"Hymns" image:[UIImage imageNamed:@"topicTabBarItem.png"] tag:0] autorelease];

	UINavigationController *favouriteNavigationController = [[[UINavigationController alloc] initWithRootViewController:favouriteTableView] autorelease];
	favouriteNavigationController.tabBarItem = [[[UITabBarItem alloc] initWithTitle:@"Favorites" image:[UIImage imageNamed:@"topicTabBarItem.png"] tag:1] autorelease];
												

	tabBarController.viewControllers = [NSArray arrayWithObjects:topicalNavigationController, favouriteNavigationController, nil];
	[window addSubview:tabBarController.view];
    [window makeKeyAndVisible];
	
	return YES;
}


/**
 Superclass implementation saves changes in the application's managed object context before the application terminates.
 */
- (void)applicationWillTerminate:(UIApplication *)application {
	[super applicationWillTerminate:application];
}


#pragma mark -
#pragma mark Memory management

- (void)dealloc {
	[tabBarController release];
	[super dealloc];
}


@end

