//
//  SettingsManager.m
//  hymnalSqlite
//
//  Created by Daniel Johnson on 11-01-04.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import "SettingsManager.h"

static NSString *const kVerseOption = @"VerseOption";
static NSString *const kMusicOption = @"MusicOption";

static SettingsManager *sharedSettingsManager = nil;

@implementation SettingsManager

+ (SettingsManager *) sharedInstance {
	if (sharedSettingsManager == nil) {
		sharedSettingsManager = [[SettingsManager alloc] init];
	}
	return sharedSettingsManager;
}

- (id) init {
	if (self == [super init]) {
		NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
		verseOption = [defaults integerForKey:kVerseOption];
		musicOption = [defaults integerForKey:kMusicOption];
	}
	return self;
}


- (MusicOption) musicOption{
	return musicOption;
}
- (VerseOption) verseOption{
	return verseOption;
}

- (void) setMusicOption:(MusicOption)mOpt{
	musicOption = mOpt;
	NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
	[defaults setInteger:musicOption forKey:kMusicOption];
}
	
- (void) setVerseOption:(VerseOption)vOpt{
	verseOption = vOpt;
	NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
	[defaults setInteger:verseOption forKey:kVerseOption];
}
@end
