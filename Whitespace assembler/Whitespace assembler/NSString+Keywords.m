//
//  NSString+Keywords.m
//  Whitespace assembler
//
//  Created by Emil Bahnsen on 14-07-10.
//
//

#import "NSString+Keywords.h"

@implementation NSString (NSString_Keywords)

- (NSArray*)keywordsIn:(NSDictionary*)keywords
		withCommentSet:(NSCharacterSet*)commentSet
	   keepingComments:(BOOL)keepComments {
	
	NSMutableArray *retArray = [[NSMutableArray alloc] init];
	NSArray *lines = [self componentsSeparatedByCharactersInSet:[NSCharacterSet newlineCharacterSet]];
	NSArray *cmdNArg;
	NSUInteger commentLocal;
	BOOL hasArg = NO, continuedString = NO;
	NSMutableString *stringArgument = [[NSMutableString alloc] init];
	NSString *cmdString;
	for (NSString *line in lines) {
		if ([line isEqualToString:@""])
			continue;
		
		//Locate comment
		commentLocal = [line rangeOfCharacterFromSet:commentSet].location;
		if (continuedString || [line rangeOfString:@"\""].location < [line rangeOfCharacterFromSet:commentSet].location) {
			commentLocal = NSNotFound;
		}
		
		//Analyse command and arg
		cmdNArg = [
				   [line substringWithRange:NSMakeRange(0, (commentLocal == NSNotFound) ? line.length : commentLocal)]
				   componentsSeparatedByCharactersInSet:[NSCharacterSet whitespaceAndNewlineCharacterSet]
				   ];
		
		[stringArgument setString:@""];
		
		//Command and arg splittet enum
		for (NSString* cmdNArgsub in cmdNArg) {
			//If nothing is found, i.e. between doubble chars
			if ([cmdNArgsub isEqualToString:@""])
				continue;
			
			//If valid command is found
			cmdString = [keywords objectForKey:cmdNArgsub];
			if (cmdString != nil && !hasArg) { //By "&& !hasArg" args can be named by keywords (althougth that's a bit naughty)
				//Add command-key to return array
				[retArray addObject:cmdNArgsub];
				
				//If command takes argument
				if (([cmdString characterAtIndex:cmdString.length-1] == ':'))
					hasArg=YES;
				continue;
				
			} else if (hasArg || continuedString) {
				if ([cmdNArgsub characterAtIndex:0] == '"' || continuedString) {
					[stringArgument appendFormat:@"%@%@", cmdNArgsub, ([cmdNArgsub characterAtIndex:cmdNArgsub.length-1] == '"') ? @"" : @" "];
					continuedString=YES;
				}
				if ([cmdNArgsub characterAtIndex:cmdNArgsub.length-1] == '"' || !continuedString) {
					[retArray addObject:([stringArgument isEqual: @""]) ? cmdNArgsub : stringArgument.copy];
					continuedString=NO;
				}
				hasArg = NO;
				continue;
			} else if ([cmdNArgsub characterAtIndex:cmdNArgsub.length-1] == ':') { //Instead of "mark abe", one can write "abe:"
				[retArray addObject:@"mark"];
				[retArray addObject:[cmdNArgsub substringToIndex:cmdNArgsub.length-1]];
				continue;
			}
			
			//If enum hits this point no keyword with/[out] argument or comment has been fonud
			//ERROR
			fprintf(stderr, "Invalid cmd: %s\n", cmdNArgsub.UTF8String);
		}
		//Comment not caught
		if (hasArg == YES) {
			//ERROR
			fprintf(stderr, "Missing argument.\n");
		}
		
		//If comment has been found
		if (commentLocal != NSNotFound && keepComments) {
			[retArray addObject:[line substringFromIndex:commentLocal]];
		}
	}
	return retArray;
}

- (NSArray*)newKeywordsIn:(NSDictionary*)keywords
		   withCommentSet:(NSCharacterSet*)commentSet
		  keepingComments:(BOOL)keepComments {
	
	NSMutableArray *retArray = [[NSMutableArray alloc] init];
	NSArray *lines = [self componentsSeparatedByCharactersInSet:[NSCharacterSet newlineCharacterSet]];
	NSArray *cmdNArg;
	NSUInteger commentLocal;
	BOOL hasArg = NO, continuedString = NO;
	NSMutableString *stringArgument = [[NSMutableString alloc] init];
	NSString *cmdString;
	for (NSString *line in lines) {
		if ([line isEqualToString:@""])
			continue;
		
		//Locate comment
		commentLocal = [line rangeOfCharacterFromSet:commentSet].location;
		if (continuedString || [line rangeOfString:@"\""].location < [line rangeOfCharacterFromSet:commentSet].location) {
			commentLocal = NSNotFound;
		}
		
		//Analyse command and arg
		cmdNArg = [
				   [line substringWithRange:NSMakeRange(0, (commentLocal == NSNotFound) ? line.length : commentLocal)]
				   componentsSeparatedByCharactersInSet:[NSCharacterSet whitespaceAndNewlineCharacterSet]
				   ];
		
		[stringArgument setString:@""];
		
		//Command and arg splittet enum
		for (NSString* cmdNArgsub in cmdNArg) {
			//If nothing is found, i.e. between doubble chars
			if ([cmdNArgsub isEqualToString:@""])
				continue;
			
			//If valid command is found
			cmdString = [keywords objectForKey:cmdNArgsub];
			if (cmdString != nil && !hasArg) { //By "&& !hasArg" args can be named by keywords (althougth that's a bit naughty)
											   //Add command-key to return-array
				[retArray addObject:cmdNArgsub];
				
				//If command takes argument
				if (([cmdString characterAtIndex:cmdString.length-1] == ':'))
					hasArg=YES;
				continue;
				
			} else if (hasArg || continuedString) {
				if ([cmdNArgsub characterAtIndex:0] == '"' || continuedString) {
					[stringArgument appendFormat:@"%@%@", cmdNArgsub, ([cmdNArgsub characterAtIndex:cmdNArgsub.length-1] == '"') ? @"" : @" "];
					continuedString=YES;
				}
				if ([cmdNArgsub characterAtIndex:cmdNArgsub.length-1] == '"' || !continuedString) {
					[retArray addObject:([stringArgument isEqual: @""]) ? cmdNArgsub : stringArgument.copy];
					continuedString=NO;
				}
				hasArg = NO;
				continue;
			} else if ([cmdNArgsub characterAtIndex:cmdNArgsub.length-1] == ':') { //Instead of "mark abe", one may write "abe:"
				[retArray addObject:@"mark"];
				[retArray addObject:[cmdNArgsub substringToIndex:cmdNArgsub.length-1]];
				continue;
			}
			
			//If enum hits this point no keyword with/[out] argument or comment has been fonud
			//ERROR
			printf("Invalid cmd: %s\n", cmdNArgsub.UTF8String);
		}
		//Comment not caught
		if (hasArg == YES) {
			//ERROR
			printf("Missing argument.\n");
		}
		
		//If comment has been found
		if (commentLocal != NSNotFound && keepComments) {
			[retArray addObject:[line substringFromIndex:commentLocal]];
		}
	}
	return retArray;
}

NSString* dictobi(NSUInteger dec) {
	NSMutableString *retNumber = [[NSMutableString alloc] initWithString:@""];
	while (dec>0) {
		[retNumber setString:[NSString stringWithFormat:@"%lu%@", dec%2, retNumber]];
		dec*=.5;
	}
	return retNumber;
}

- (NSString*)binaryValue {
	NSMutableString *retNumber = [[NSMutableString alloc] initWithString:@""];
	
	//Determine if string is number
	BOOL isNegative=([self rangeOfCharacterFromSet:[NSCharacterSet characterSetWithCharactersInString:@"-"]].location == 0) ? YES : NO;
	if ([self rangeOfCharacterFromSet:[NSCharacterSet decimalDigitCharacterSet]].location == 0 || isNegative) {
		if (isNegative) {
			[retNumber appendString:[self substringFromIndex:1]];
		} else {
			[retNumber appendString:self];
		}
		[retNumber setString:dictobi(retNumber.integerValue)];
	} else {
		//Appends chars as integers, if string is not a number
		for (NSUInteger i=0; i<self.length; i++) {
			if ([self characterAtIndex:i] > 0 && [self characterAtIndex:i] < 9) {
				[retNumber appendString:dictobi([self characterAtIndex:i]+'0')];
			} else {
				[retNumber appendString:dictobi([self characterAtIndex:i])];
			}
		}
	}
	
	//Bit determineing if number is negative
	(isNegative) ? [retNumber setString:[NSString stringWithFormat:@"1%@", retNumber]]
				 : [retNumber setString:[NSString stringWithFormat:@"0%@", retNumber]];
	
	return retNumber;
}

- (NSString*)binaryValueAsString {
	NSMutableString *retNumber = [[NSMutableString alloc] initWithString:@""];
	
	//Appends chars as integers, if string is not a number
	for (NSUInteger i=0; i<self.length; i++) {
		if ([self characterAtIndex:i] > 0 && [self characterAtIndex:i] < 9) {
			[retNumber appendString:dictobi([self characterAtIndex:i]+'0')];
		} else {
			[retNumber appendString:dictobi([self characterAtIndex:i])];
		}
	}
	[retNumber setString:[NSString stringWithFormat:@"0%@", retNumber]];
	
	return retNumber;
}

@end
