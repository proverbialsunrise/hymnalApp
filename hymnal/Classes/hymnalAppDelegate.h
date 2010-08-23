//
//  hymnalAppDelegate.h
//  hymnal
//
//  Created by Daniel Johnson on 10-05-20.
//  Copyright __MyCompanyName__ 2010. All rights reserved.
//

#import <UIKit/UIKit.h>

@class HYMainViewController;

@interface hymnalAppDelegate : NSObject <UIApplicationDelegate> {
    UIWindow *window;
    HYMainViewController *viewController;
	UITabBarController *tabBarController;
}

@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) IBOutlet HYMainViewController *viewController;
@property (nonatomic, retain) UITabBarController *tabBarController;

@end

