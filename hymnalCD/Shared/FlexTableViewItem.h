//
//  FlexTableViewItem.h
//  hymnalCD
//
//  Created by Daniel Johnson on 10-07-01.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>


@interface FlexTableViewItem : NSObject {
	UIImage *image;
	NSString *title;
}

@property (nonatomic, retain) UIImage *image;
@property (nonatomic, retain) NSString *title;

- (id) initWithImage:(UIImage *)anImage Title:(NSString *)theTitle;

@end
