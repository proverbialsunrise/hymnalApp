//
//  Hymnal.h
//  hymnalCD
//
//  Created by Daniel Johnson on 10-08-26.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <CoreData/CoreData.h>


@interface Hymnal :  NSManagedObject  
{
}

@property (nonatomic, retain) NSString * name;
@property (nonatomic, retain) NSString * copyright;
@property (nonatomic, retain) NSSet* hymns;

@end


@interface Hymnal (CoreDataGeneratedAccessors)
- (void)addHymnsObject:(NSManagedObject *)value;
- (void)removeHymnsObject:(NSManagedObject *)value;
- (void)addHymns:(NSSet *)value;
- (void)removeHymns:(NSSet *)value;

@end

