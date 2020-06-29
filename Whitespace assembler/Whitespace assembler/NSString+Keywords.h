//
//  NSString+Keywords.h
//  Whitespace assembler
//
//  Created by Emil Bahnsen on 14-07-10.
//
//

#import <Foundation/Foundation.h>

@interface NSString (NSString_Keywords)

- (NSArray*)keywordsIn:(NSDictionary*)keywords
		withCommentSet:(NSCharacterSet*)commentSet
	   keepingComments:(BOOL)keepComments;

- (NSString*)binaryValue;
- (NSString*)binaryValueAsString;

@end
