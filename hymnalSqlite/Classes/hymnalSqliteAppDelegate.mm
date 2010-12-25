//
//  hymnalSqliteAppDelegate.m
//  hymnalSqlite
//
//  Created by Daniel Johnson on 10-11-28.
//  Copyright __MyCompanyName__ 2010. All rights reserved.
//

#import "hymnalSqliteAppDelegate.h"
#import <string>


#import "DatabaseHelper.h"
#import "ByTitleController.h"
#import "FavouritesController.h"
#import "ByNumberController.h"



@implementation hymnalSqliteAppDelegate

@synthesize window;
@synthesize tabBarController;


#pragma mark -
#pragma mark Application lifecycle

- (NSString *)createEditableCopyOfDatabaseIfNeeded
{
	BOOL dbExists = FALSE;
	NSFileManager *fileManager = [NSFileManager defaultManager];
	NSError *error;
	NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
	NSString *documentsDirectory = [paths objectAtIndex:0];
	NSString *writableDBPath = [documentsDirectory stringByAppendingFormat:@"hymnal.db"];
	
	dbExists = [fileManager fileExistsAtPath:writableDBPath];
	if (dbExists) {
		return writableDBPath;
	}
	NSString *dbPath = [[NSBundle mainBundle] pathForResource:@"exampleDB" ofType:@"db"];
	dbExists = [fileManager copyItemAtPath:dbPath toPath:writableDBPath error:&error];
	if (dbExists) {
		return writableDBPath;
	} else {
		NSAssert1(0, @"Failed to create writable database file: '%@'.", [error localizedDescription]);
		return nil;
	}
}

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {    
    
	NSString *dbPath = [self createEditableCopyOfDatabaseIfNeeded];
	std::string dbPathString = std::string([dbPath UTF8String]);
	openConnectionWithPath(dbPathString);
	//Instantiate the ByNumber list.
	ByNumberController *byNumberController = [[[ByNumberController alloc] initWithNibName:@"HymnListTableViewController" bundle:[NSBundle mainBundle]] autorelease];
	UINavigationController *numNavigationController = [[[UINavigationController alloc] initWithRootViewController:byNumberController] autorelease];
	numNavigationController.tabBarItem = [[[UITabBarItem alloc] initWithTitle:@"By Number" image:[UIImage imageNamed:@"ByNumber"] tag:1] autorelease];
	numNavigationController.navigationBar.barStyle = UIBarStyleBlack;
	//Instantiate the ByTopic list.
	//we don't have this view controller yet but I wanted to add something else to the TabBar.  
	ByTitleController *byTitleController = [[[ByTitleController alloc] initWithNibName:@"HymnListTableViewController" bundle:[NSBundle mainBundle]] autorelease];
	UINavigationController *titleNavigationController = [[[UINavigationController alloc] initWithRootViewController:byTitleController] autorelease];
	titleNavigationController.tabBarItem = [[[UITabBarItem alloc] initWithTitle:@"By Title" image:[UIImage imageNamed:@"ByTitle"] tag:2] autorelease];
	titleNavigationController.navigationBar.barStyle = UIBarStyleBlack;
	
	FavouritesController *favouritesController = [[[FavouritesController alloc] initWithNibName:@"HymnListTableViewController" bundle:[NSBundle mainBundle]] autorelease];
	UINavigationController *favouritesNavigationController = [[[UINavigationController alloc] initWithRootViewController:favouritesController] autorelease];
	favouritesNavigationController.tabBarItem = [[[UITabBarItem alloc] initWithTabBarSystemItem:UITabBarSystemItemFavorites tag:3] autorelease];
	favouritesNavigationController.navigationBar.barStyle = UIBarStyleBlack;

	
	NSArray *viewControllers = [NSArray arrayWithObjects:numNavigationController, titleNavigationController, favouritesNavigationController, nil];
	
	tabBarController.viewControllers = viewControllers;
		
    // Add the navigation controller's view to the window and display.
    [window addSubview:tabBarController.view];
    [window makeKeyAndVisible];
    return YES;
}


- (void)applicationWillResignActive:(UIApplication *)application {
    /*
     Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
     Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
     */
}


- (void)applicationDidEnterBackground:(UIApplication *)application {
    /*
     Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later. 
     If your application supports background execution, called instead of applicationWillTerminate: when the user quits.
     */
}


- (void)applicationWillEnterForeground:(UIApplication *)application {
    /*
     Called as part of  transition from the background to the inactive state: here you can undo many of the changes made on entering the background.
     */
}


- (void)applicationDidBecomeActive:(UIApplication *)application {
    /*
     Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
     */
}


- (void)applicationWillTerminate:(UIApplication *)application {
    /*
     Called when the application is about to terminate.
     See also applicationDidEnterBackground:.
     */
}


#pragma mark -
#pragma mark Memory management

- (void)applicationDidReceiveMemoryWarning:(UIApplication *)application {
    /*
     Free up as much memory as possible by purging cached data objects that can be recreated (or reloaded from disk) later.
     */
}


- (void)dealloc {
	[tabBarController release];
	[window release];
	[super dealloc];
}


@end

