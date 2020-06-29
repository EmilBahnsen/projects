//
//  NSMutableString+ConstructCode.h
//  Whitespace assembler
//
//  Created by Emil Bahnsen on 14-07-10.
//
//

#import <Foundation/Foundation.h>

@interface NSMutableString (NSMutableString_ConstructCode)

- (void)replacingOccurrencesOfStrings:(NSArray*)from withStrings:(NSArray*)to;
- (void)constructCodeFromDictionary:(NSDictionary*)keywords
					  andComponents:(NSString*)fileString
	   withArgumentStringConvertien:(BOOL)argConvertion
				  andArgumentEndcap:(NSString*)argEnd
	andArgumentExeptionEndcapAppend:(NSDictionary*)argExepEnd
				  withShortenLabels:(BOOL)shortLables
							  after:(NSArray*)lablesAt
					   withComments:(BOOL)keepComments;

@end
