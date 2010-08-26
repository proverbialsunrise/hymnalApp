//
//  Hymn.h
//  hymnalCD
//
//  Created by Daniel Johnson on 10-08-26.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <CoreData/CoreData.h>

@class LyricSection;
@class MusicSection;
@class ScripturalAllusion;
@class UseInWorship;

@interface Hymn :  NSManagedObject  
{
}

@property (nonatomic, retain) NSString * section;
@property (nonatomic, retain) NSString * subSection;
@property (nonatomic, retain) NSString * author;
@property (nonatomic, retain) NSString * tune;
@property (nonatomic, retain) NSString * subSubSection;
@property (nonatomic, retain) NSString * firstLine;
@property (nonatomic, retain) NSString * translator;
@property (nonatomic, retain) NSString * metric;
@property (nonatomic, retain) NSString * title;
@property (nonatomic, retain) NSNumber * number;
@property (nonatomic, retain) NSString * copyrightInfo;
@property (nonatomic, retain) NSString * composer;
@property (nonatomic, retain) NSSet* scripturalAllusions;
@property (nonatomic, retain) NSSet* musicSections;
@property (nonatomic, retain) NSSet* lyricSections;
@property (nonatomic, retain) NSSet* usesInWorship;

@end


@interface Hymn (CoreDataGeneratedAccessors)
- (void)addScripturalAllusionsObject:(ScripturalAllusion *)value;
- (void)removeScripturalAllusionsObject:(ScripturalAllusion *)value;
- (void)addScripturalAllusions:(NSSet *)value;
- (void)removeScripturalAllusions:(NSSet *)value;

- (void)addMusicSectionsObject:(MusicSection *)value;
- (void)removeMusicSectionsObject:(MusicSection *)value;
- (void)addMusicSections:(NSSet *)value;
- (void)removeMusicSections:(NSSet *)value;

- (void)addLyricSectionsObject:(LyricSection *)value;
- (void)removeLyricSectionsObject:(LyricSection *)value;
- (void)addLyricSections:(NSSet *)value;
- (void)removeLyricSections:(NSSet *)value;

- (void)addUsesInWorshipObject:(UseInWorship *)value;
- (void)removeUsesInWorshipObject:(UseInWorship *)value;
- (void)addUsesInWorship:(NSSet *)value;
- (void)removeUsesInWorship:(NSSet *)value;

@end

