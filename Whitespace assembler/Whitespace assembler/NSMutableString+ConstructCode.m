//
//  NSMutableString+ConstructCode.m
//  Whitespace assembler
//
//  Created by Emil Bahnsen on 14-07-10.
//
//

#import "NSMutableString+ConstructCode.h"
#import "NSString+Keywords.h"

@implementation NSMutableString (NSMutableString_ConstructCode)

- (void)replacingOccurrencesOfStrings:(NSArray*)from
						  withStrings:(NSArray*)to {
	
	for (NSUInteger i=0; i<from.count; i++) {
		NSString* stringFrom = [from objectAtIndex:i];
		NSString* stringTo = [to objectAtIndex:i];
		[self replaceOccurrencesOfString:stringFrom
							  withString:stringTo
								 options:NSBackwardsSearch
								   range:NSMakeRange(0, self.length)];
	}
}

- (void)constructCodeFromDictionary:(NSDictionary*)keywords
					  andComponents:(NSString*)fileString
	   withArgumentStringConvertien:(BOOL)argConvertion
				  andArgumentEndcap:(NSString*)argEnd
	andArgumentExeptionEndcapAppend:(NSDictionary*)argExepEnd
				  withShortenLabels:(BOOL)shortLables
							  after:(NSArray*)lablesAt
					   withComments:(BOOL)keepComments {
	
	const NSArray* cmdNargNcomment = [fileString keywordsIn:keywords
											 withCommentSet:[NSCharacterSet characterSetWithCharactersInString:@";"]
											keepingComments:keepComments];
	NSLog(@"%@", cmdNargNcomment);
	
	NSMutableString *lastCmd = [[NSMutableString alloc] init];
	NSMutableString *lastSuperCmd = [[NSMutableString alloc] init];
	BOOL hasArg = NO, hasArgExepEnd = NO;
	NSMutableDictionary *shortendLabels = [[NSMutableDictionary alloc] init];
	NSUInteger shortLabelNumber = 0;
	//Constscruts file from array of command, args, and comments
	for (NSString* curCmdArgComm in cmdNargNcomment) {
		
		NSString *cmdString = [keywords objectForKey:curCmdArgComm];
		//Commad encountered
		if (cmdString != nil && !hasArg) {
			[lastCmd setString:[cmdString substringToIndex:cmdString.length-1]];
			[lastSuperCmd setString:curCmdArgComm];
			//If command takes argument
			if (([cmdString characterAtIndex:cmdString.length-1] == ':')) {
				hasArg=YES;
				[self appendString:lastCmd];
				hasArgExepEnd = ([argExepEnd objectForKey:curCmdArgComm] == nil) ? NO : YES ;
				asm("nop");
			} else {
				[self appendString:cmdString];
				asm("nop");
			}
			continue;
		} else if (hasArg) { //Argument
			if (argConvertion) {
				//If argument is string, i.e. with quotes sequentiate eatch char (exept for quotes)
				if ([curCmdArgComm characterAtIndex:0] == '"') {
					NSMutableString *concatString = [[NSMutableString alloc] initWithString:curCmdArgComm];
					[concatString replacingOccurrencesOfStrings:@[@"\\n"] withStrings:@[@"\n"]];
					for (NSUInteger i=curCmdArgComm.length-2; i>0; i--) {
						[self appendFormat:@"%@%@%@",
						 (i<curCmdArgComm.length-2)?lastCmd:@"",	//Append "push" from 2. itterration
						 [NSString stringWithFormat:@"%c", [curCmdArgComm characterAtIndex:i]].binaryValueAsString,	//Append binary value of string char
						 (i>1) ? @"\n":@""];							//Append newline til penultimate itterration
						asm("nop");
					}
				} else {
					if (shortLables && [lablesAt containsObject:lastSuperCmd]) {
						//NoeNotHave
						if ([shortendLabels objectForKey:curCmdArgComm] == nil) {
							[shortendLabels setObject:[NSNumber numberWithUnsignedInteger:shortLabelNumber] forKey:curCmdArgComm];
							shortLabelNumber++;
						}
						[self appendString:[(NSNumber*)[shortendLabels objectForKey:curCmdArgComm] stringValue].binaryValue];
					} else {
						[self appendString:curCmdArgComm.binaryValue];
					}
				}
			} else { //Do not convert argument to binary
				[self appendString:curCmdArgComm];
			}
			
			if (hasArgExepEnd) {
				[self appendFormat:@"%@\n", [argExepEnd objectForKey:lastSuperCmd]];
			} else {
				[self appendFormat:@"%@\n", argEnd];
			}
			hasArg=NO;
		} else if (keepComments) { //Comment
			[self appendString:[[curCmdArgComm capitalizedString]
							  stringByReplacingOccurrencesOfString:@" "
							  withString:@""]
			 ];
			asm("nop");
		}
		
	}
	if (argConvertion) {
		[self replacingOccurrencesOfStrings:[NSArray arrayWithObjects:@"0", @"1", nil]
								withStrings:[NSArray arrayWithObjects:@" ", @"\t", nil]];
	}

}

@end
