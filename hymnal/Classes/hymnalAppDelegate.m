//
//  hymnalAppDelegate.m
//  hymnal
//
//  Created by Daniel Johnson on 10-05-20.
//  Copyright __MyCompanyName__ 2010. All rights reserved.
//

#import "hymnalAppDelegate.h"
#import "HYMainViewController.h"

@implementation hymnalAppDelegate

@synthesize window;
@synthesize viewController;
@synthesize tabBarController;


- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {    
    
	//The viewController is loaded from the main nib file MainWindow.xib at application launch
	//as specified in the Info.plist.  The outlets defined in hymnalAppDelegate.h are then set
	//to point to the correct objects (window and viewController)
    [window addSubview:viewController.view];
	//Create the set of UIViewControllers that will represent each tab of the interface. 
	
	
	//Create a UITabBarController and add in all the UIViewControllers that will need to be part of it.
	
    [window makeKeyAndVisible];
	
	return YES;
}


- (void)dealloc {
    [viewController release];
    [window release];
    [super dealloc];
}


@end
