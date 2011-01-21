//
//  SettingsManager.h
//  hymnalSqlite
//
//  Created by Daniel Johnson on 11-01-04.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "Global.h"

typedef enum VerseOption {
	HORIZONTAL = 0,
	VERTICAL,
	INLINE
} VerseOption;


//A singleton used to manage settings for the application. 

@interface SettingsManager : NSObject {
	VerseOption verseOption;
	PartSpecifier musicOption;
}


@property (nonatomic, assign) VerseOption verseOption;
@property (nonatomic, assign) PartSpecifier musicOption;

+ (SettingsManager *) sharedInstance;

- (id) init;

@end
