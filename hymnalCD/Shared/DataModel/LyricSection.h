//
//  LyricSection.h
//  hymnalCD
//
//  Created by Daniel Johnson on 10-08-26.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <CoreData/CoreData.h>


@interface LyricSection :  NSManagedObject  
{
}

@property (nonatomic, retain) NSNumber * verseNumber;
@property (nonatomic, retain) NSNumber * lineNumber;
@property (nonatomic, retain) NSString * image;
@property (nonatomic, retain) NSString * type;
@property (nonatomic, retain) NSString * location;

@end



