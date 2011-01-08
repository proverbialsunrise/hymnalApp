//
//  SettingsManager.h
//  hymnalSqlite
//
//  Created by Daniel Johnson on 11-01-04.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>

typedef enum MusicOption {
	ALLPARTS = 0,
	ALTOSOPRANO,
	TENORBASS
} MusicOption;

typedef enum VerseOption {
	HORIZONTAL = 9,
	VERTICAL,
	INLINE
} VerseOption;


//A singleton used to manage settings for the application. 

@interface SettingsManager : NSObject {
	VerseOption verseOption;
	MusicOption musicOption;
}


@property (nonatomic, assign) VerseOption verseOption;
@property (nonatomic, assign) MusicOption musicOption;

+ (SettingsManager *) sharedInstance;

- (id) init;

@end
