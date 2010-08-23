//
//  testHymnAppAppDelegate.m
//  testHymnApp
//
//  Created by Daniel Johnson on 10-06-01.
//  Copyright __MyCompanyName__ 2010. All rights reserved.
//

#import "testHymnAppAppDelegate.h"
#import "testHymnAppViewController.h"

@implementation testHymnAppAppDelegate

@synthesize window;
@synthesize viewController;


- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {    
    
    // Override point for customization after app launch    
    [window addSubview:viewController.view];
    [window makeKeyAndVisible];
	
	return YES;
}


- (void)dealloc {
    [viewController release];
    [window release];
    [super dealloc];
}


@end
