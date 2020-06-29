//
//  main.m
//  Whitespace assembler
//
//  Created by Emil Bahnsen on 14-07-09.
//
//

//The follewing describes Whitespace syntax: http://compsoc.dur.ac.uk/whitespace/tutorial.php
/*
 A = [space], B = [tap], C = [LF]
 Instruction Modification Parameter (IMP)
 
 IMP	specific	wsa		description
 A	Stack Manipulation:
	A	n	Push the number onto the stack
	CA		Duplicate the top item on the stack
	BA	n	Copy the nth item on the stack (given by the argument) onto the top of the stack
	CB		Swap the top two items on the stack
	CC		Discard the top item on the stack
	BC	n	Slide n items off the stack, keeping the top item
 BA	Arithmetic:
	AA		Addition
	AB		Subtraction
	AC		Multiplication
	BA		Interger Division
	BB		Modulo
 BB	Heap access:
	A		Store
	B		Retrieve
 C	Flow Control:
	AA	l	Mark a location in the program
	AB	l	Call a subroutine
	AC	l	Jump unconditionally to a label
	BA	l	Jump to a label if the top of the stack is zero
	BB	l	Jump to a label if the top of the stack is negative
	BC		End a subroutine and transfer control back to the caller
	CC		End the program
 BC	I/O:
	AA		Output the character at the top of the stack
	AB		Output the number at the top of the stack
	BA		Read a character and place it in the location given by the top of the stack
	BB		Read a number and place it in the location given by the top of the stack
 */

#import <Foundation/Foundation.h>
#import <libgen.h>
#import "NSMutableString+ConstructCode.h"
#import "NSString+Keywords.h"

static const NSString *version = @"0.1";
static const NSString *commentSpacing = @"";

NSDictionary* loadWsatows(void) {
	return @{
			 //Stack Manipulation
			 @"push":	@"  :",
			 @"dup":	@" \n ",
			 @"copy":	@" \t :",
			 @"exch":	@" \n\t",
			 @"pop":	@" \n\n",
			 @"slid":	@" \t\n:",
			 
			 //Arithmetic
			 @"add":	@"\t   ",
			 @"sub":	@"\t  \t",
			 @"mul":	@"\t  \n",
			 @"div":	@"\t \t ",
			 @"mod":	@"\t \t\t",
			 
			 //Heap access
			 @"stor":	@"\t\t ",
			 @"reti":	@"\t\t\t",
			 
			 //Flow Contral
			 @"mark":	@"\n  :",
			 @"call":	@"\n \t:",
			 @"jump":	@"\n \n:",
			 @"jumpz":	@"\n\t :",
			 @"jumpn":	@"\n\t\t:",
			 @"ret":	@"\n\t\n",
			 @"exit":	@"\n\n\n",
			 
			 //I/O
			 @"prtc":	@"\t\n  ",
			 @"prtn":	@"\t\n \t",
			 @"redc":	@"\t\n\t ",
			 @"redn":	@"\t\n\t\t"
			 };
}

NSDictionary* loadWstowsa(void) {
	NSDictionary *wsatows = loadWsatows();
	NSMutableDictionary *wstowsa = [[NSMutableDictionary alloc] init];
	
	for (NSString *wsa in [wsatows allKeys]) {
		NSString *curObj = [wsatows objectForKey:wsa];
		if ([curObj characterAtIndex:curObj.length-1] == ':') {
			[wstowsa setObject:[NSString stringWithFormat:@"%@:", wsa]
						forKey:[curObj substringToIndex:wsa.length-1]];
		} else {
			[wstowsa setObject:wsa
						forKey:curObj];
		}
	}
	
	return wstowsa;
}

NSDictionary* loadWsatoc(void) {
	return @{
			 //Stack Manipulation
			 @"push":	@"push(:",
			 @"dup":	@"dup();\n",
			 @"copy":	@"copy(:",
			 @"exch":	@"exch();\n",
			 @"pop":	@"pop();\n",
			 @"slid":	@"slid(:",
			 
			 //Arithmetic
			 @"add":	@"add();\n",
			 @"sub":	@"sub();\n",
			 @"mul":	@"mul();\n",
			 @"div":	@"div();\n",
			 @"mod":	@"mod();\n",
			 
			 //Heap access
			 @"stor":	@"stor();\n",
			 @"reti":	@"reti();\n",
			 
			 //Flow Contral
			 @"mark":	@"\n:", //ExepEnd: :
			 @"call":	@"goto :", //ExepEnd: ;
			 @"jump":	@"goto :", //ExepEnd: ;
			 @"jumpz":	@"if (top() == 0) goto :", //ExepEnd: ;
			 @"jumpn":	@"if (top() < 0) goto :", //ExepEnd: ;
			 @"ret":	@"return;\n",
			 @"exit":	@"return 0;\n",
			 
			 //I/O
			 @"prtc":	@"printf(\"%c\", top());\n",
			 @"prtn":	@"printf(\"%i\", top());\n",
			 @"redc":	@"push(getchar());\n",
			 @"redn":	@"push(getchar()-48);\n"
			 };
}

int main(int argc, const char *argv[]) {
	
	//Description
	if (argc == 1)
		printf(
"Whitespace Assembler v%s\n\
%s [-acwmh] <file ...>\n\
 -a, --wssam	Compile WS to WSA source code\n\
 -c, --csource	Compile WSA to C source code\n\
 -w, --ws	Compile WSA to WS source code (implicitly assumend)\n\
 -m, --comm	Transferer comments\n\
 -s, --short	Shorten lables\n\
 -h, --help	Show help\n", [version UTF8String], basename((char*)argv[0]));
	
	@autoreleasepool {
	    
		int ch;
		BOOL aflag = NO;
		BOOL cflag = NO;
		BOOL wflag = NO;
		BOOL commflag = NO;
		BOOL sflag = NO;
		//Getting args
	    while ((ch = getopt(argc, (char *const*)argv, "acwmhs")) != -1) {
			switch (ch) {
				case 'a':
					aflag=YES;
					break;
					
				case 'c':
					cflag=YES;
					break;
					
				case 'w':
					wflag=YES;
					break;
					
				case 'm':
					commflag=YES;
					break;

				case 'h':
					main(1, argv); //Writes generel help
					
					//Describes the language further...
					printf("...\n");
					printf("By Emil Bahnsen\n");
					break;
					
				case 's':
					sflag=YES;
					break;
					
				default:
					//--doubles
					break;
			}
		}
		
		//Looping throuth files
		for (NSUInteger fileN=optind; fileN<argc; fileN++) {
			NSString *fileName = [[NSString alloc] initWithUTF8String:argv[fileN]];
			NSString *fileBaseName = [[NSString alloc] initWithUTF8String:basename((char*)fileName.UTF8String)];
			NSString *fileDirName = [[NSString alloc] initWithUTF8String:dirname((char*)fileName.UTF8String)];
			
			NSString *fileString = [[NSString alloc] initWithContentsOfFile:fileName
																   encoding:NSUTF8StringEncoding
																	  error:nil];
			
			NSMutableString *outputString = [[NSMutableString alloc] initWithString:@""];
			NSMutableString *fileExtention = [[NSMutableString alloc] init];
			if (aflag) {
				NSDictionary *wstowsa = loadWstowsa();
				for (NSUInteger i=0; i<fileString.length; i++) {
					//If char of comment is encountered append to string, delete it, continue, i.e. process without comment char.
					//Find first unconizable string
					//Step one back, next replace char1(space)
					//If arg read to \n, replace char2(\n)
					//Append: space, colon and aqured comment-stirng
				}
				//Replace space with char3, tab with char4 and \n with char5
				//Replace char1 and 2 with space and \n respectively
				asm("nop");
			} else if (cflag) { //WSA to C
				NSDictionary *wsatoc = loadWsatoc();
				[outputString constructCodeFromDictionary:wsatoc
											andComponents:fileString
							 withArgumentStringConvertien:NO
										andArgumentEndcap:@");"
						  andArgumentExeptionEndcapAppend:@{@"mark": @":",
															@"call": @";",
															@"jump": @";",
															@"jumpz": @";",
															@"jumpn": @";"}
										withShortenLabels:sflag
													after:@[@"mark", @"call", @"jump", @"jumpz", @"jumpn"]
											 withComments:commflag];
				[fileExtention setString:@"c"];
				
			} else { //WSA to WS
				NSDictionary *wsatows = loadWsatows();
				[outputString constructCodeFromDictionary:wsatows
											andComponents:fileString
							 withArgumentStringConvertien:YES
										andArgumentEndcap:@""
						  andArgumentExeptionEndcapAppend:nil
										withShortenLabels:sflag
													after:@[@"mark", @"call", @"jump", @"jumpz", @"jumpn"]
											 withComments:commflag];
				[fileExtention setString:@"ws"];
				asm("nop");
			}
			NSString *savePath = [NSString stringWithFormat:
								  @"%s.%s",
								  basename((char*)[fileName stringByDeletingPathExtension].UTF8String),
								  fileExtention.UTF8String];
			NSLog(@"Save: %@", savePath);
			
			[outputString writeToFile:savePath
						   atomically:YES
							 encoding:NSUTF8StringEncoding
								error:nil];
		}
	    
	}
    return 0;
}

