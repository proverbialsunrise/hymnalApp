//
//  testHymnAppAppDelegate.h
//  testHymnApp
//
//  Created by Daniel Johnson on 10-06-01.
//  Copyright __MyCompanyName__ 2010. All rights reserved.
//

#import <UIKit/UIKit.h>

@class testHymnAppViewController;

@interface testHymnAppAppDelegate : NSObject <UIApplicationDelegate> {
    UIWindow *window;
    testHymnAppViewController *viewController;
}

@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) IBOutlet testHymnAppViewController *viewController;

@end

