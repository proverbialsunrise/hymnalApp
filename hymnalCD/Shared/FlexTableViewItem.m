//
//  FlexTableViewItem.m
//  hymnalCD
//
//  Created by Daniel Johnson on 10-07-01.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "FlexTableViewItem.h"


@implementation FlexTableViewItem

@synthesize image;
@synthesize title;

- (id) initWithImage:(UIImage *)anImage Title:(NSString *)theTitle {
	if ([self init]) {
		self.image = anImage;
		self.title = theTitle;
	}
	return self;
}

@end
