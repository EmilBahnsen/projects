//
//  Bezier_curvesView.m
//  Bezier curves
//
//  Created by Emil Bahnsen on 14-07-10.
//
//

#import "Bezier_curvesView.h"
#import "NSBezierPath+quicklines.h"

@implementation Bezier_curvesView

NSRect back;
CGFloat t;
BOOL backwards = NO;
NSMutableArray *P, *subPoints, *interPoints, *colours, *traceP;
NSUInteger pointCount;
const CGFloat inc=.02;
NSMutableArray *frames;
NSUInteger superframe;
BOOL buffered = YES;
BOOL didChange;

- (void)initFrames {
	const NSString *bundelID = [[NSBundle mainBundle] bundleIdentifier];

	ScreenSaverDefaults *defaults;
	defaults = [ScreenSaverDefaults defaultsForModuleWithName:bundelID];

	P = [[NSMutableArray alloc] init];
	colours = [[NSMutableArray alloc] init];
	for (NSUInteger i=0; i<5; i++) {
		if (!self.isPreview) {
			[P addObject:[NSValue valueWithPoint:
						  SSRandomPointForSizeWithinRect(NSMakeSize(50, 50),
														 NSMakeRect(50, 50,
																	self.bounds.size.width,
																	self.bounds.size.height))]];
		} else {
			[P addObject:[NSValue valueWithPoint:
						  SSRandomPointForSizeWithinRect(NSMakeSize(50, 50),
														 NSMakeRect(50, 50,
																	294,
																	183))]];
		}

		//Stdpash, for test purposes
		//			[P addObject:[NSValue valueWithPoint:NSMakePoint(100*i, 50*i+(5%(i+1))*50)]];

		[colours addObject:[NSColor colorWithCalibratedRed:SSRandomFloatBetween(0, 1)
													 green:SSRandomFloatBetween(0, 1)
													  blue:SSRandomFloatBetween(0, 1)
													 alpha:0.4]];
	}
	traceP = [[NSMutableArray alloc] init];
	subPoints = [[NSMutableArray alloc] init];
	interPoints = [[NSMutableArray alloc] init];

	t=0;
	buffered = (self.isPreview == YES) ? NO : YES;
	if (buffered) {
		frames = [[NSMutableArray alloc] init];
		NSImage *image = [[NSImage alloc] init];

		//Stores first tree cycles
		//The initial one is discharted later
		while (t < 3.0) {
			image = [NSImage imageWithSize:self.bounds.size flipped:NO drawingHandler:^BOOL(NSRect dstRect) {
				[self animateOneSubframe];
				return YES;
			}];
			//Load into memory before draw, see below
			//[image setCacheMode:NSImageCacheAlways];
			[frames addObject:image];
			t+=inc;
		}

	}

}

- (id)initWithFrame:(NSRect)frame isPreview:(BOOL)isPreview {
    self = [super initWithFrame:frame isPreview:isPreview];
    if (self) {
		[self setAnimationTimeInterval:1/20.0];
		[self initFrames];
    }

    return self;
}

- (void)startAnimation {
	didChange=YES;
	t=0;
	superframe=0;
	pointCount=1.0/inc;
	//inc+=.001;

	//When one exits preview and the mini-preview continues
	if (traceP.count > 0) {
		[self initFrames];
	}

	//Load into memory before draw, see above
//	for (NSImage *image in frames) {
//		[image drawInRect:self.bounds];
//	}
	
    [super startAnimation];
}

- (void)stopAnimation {
	[frames removeAllObjects];
    [super stopAnimation];
}

- (void)drawRect:(NSRect)rect {
	back=rect;
    [super drawRect:rect];
}

NSPoint subtractPoints(NSPoint a, NSPoint b) {
	return NSMakePoint(a.x-b.x, a.y-b.y);
}

NSPoint addPoints(NSPoint a, NSPoint b) {
	return NSMakePoint(a.x+b.x, a.y+b.y);
}

NSPoint pctPoints(CGFloat a, NSPoint b) {
	return NSMakePoint(a*b.x, a*b.y);
}

NSPoint B(NSArray *P, CGFloat t) {
	NSMutableArray *Q = [[NSMutableArray alloc] initWithArray:P];
	NSMutableArray *T = [[NSMutableArray alloc] initWithArray:P];
	
	if (P.count == 2) {
		NSPoint deltaPct1 = pctPoints(t,
									  subtractPoints([Q[1] pointValue],
													 [Q[0] pointValue]));
		return addPoints(deltaPct1, [Q[0] pointValue]);
	}
	for (NSUInteger i=0; i<P.count-2; i++) {
		NSPoint deltaPct1 = pctPoints(t,
									  subtractPoints([Q[i+1] pointValue],
													 [Q[i] pointValue]));
		NSPoint deltaPct2 = pctPoints(t,
									  subtractPoints([Q[i+2] pointValue],
													 [Q[i+1] pointValue]));
		
		NSPoint A = addPoints(deltaPct1, [Q[i] pointValue]);
		NSPoint B = addPoints(deltaPct2, [Q[i+1] pointValue]);
		
		T[i] = [NSValue valueWithPoint:A];
		T[i+1] = [NSValue valueWithPoint:B];
	}
	
	[T removeLastObject];
	[Q setArray:T];
	return B(Q, t);
}

- (void)animateOneSubframe {
	
	[[NSColor lightGrayColor] set];
	[NSBezierPath fillRect:back];
	
	NSBezierPath *line = [NSBezierPath bezierPath];
	[line setLineWidth:3.0];
	[[NSColor colorWithCalibratedRed:1 green:1 blue:1 alpha:.5] set];

	//Init points and controlepoints
	for (NSUInteger i=0; i<P.count-1; i++) {
		[line strokeLineBetweenPoints:[P[i] pointValue]
								  and:[P[i+1] pointValue]];
	}

	//Circles araund init points
	[line setLineWidth:5.0];
	for (NSValue *curPoint in P) {
		[line drawCircleCenteredAtPoint:curPoint.pointValue
							 withRadius:10.0];
	}
	
	[subPoints setArray:P];
	[interPoints setArray:P];
	
	NSUInteger ci=0;
	[line setLineWidth:4.0];
	
slid:
	if (subPoints.count == 2) {
		goto end;
	}

	//Lines and sublines
	[(NSColor*)colours[ci++] set];
	for (NSUInteger i=0; i<subPoints.count-2; i++) {
		NSPoint deltaPct1 = pctPoints(t,
									  subtractPoints([subPoints[i+1] pointValue],
													 [subPoints[i] pointValue]));
		NSPoint deltaPct2 = pctPoints(t,
									  subtractPoints([subPoints[i+2] pointValue],
													 [subPoints[i+1] pointValue]));
		
		NSPoint A = addPoints(deltaPct1, [subPoints[i] pointValue]);
		NSPoint B = addPoints(deltaPct2, [subPoints[i+1] pointValue]);
		
		interPoints[i] = [NSValue valueWithPoint:A];
		interPoints[i+1] = [NSValue valueWithPoint:B];
		
		[line strokeLineBetweenPoints:A and:B];
		[line drawCircleCenteredAtPoint:A withRadius:4.0];
		[line drawCircleCenteredAtPoint:B withRadius:4.0];
	}

	[interPoints removeLastObject];
	[subPoints setArray:interPoints];
	goto slid;
	
end: {
	
	NSColor *first = [NSColor greenColor];
	NSColor *second = [NSColor redColor];
	[line setLineWidth:5.0];
	
	for (NSUInteger i=0; i<traceP.count; i++) {
		if (i >= pointCount && i < pointCount*2) {
			[second set];
		} else {
			[first set];
		}
		
		[line drawCircleCenteredAtPoint:[traceP[i] pointValue]
							   withRadius:1.0];

		//Do append
//		if (i > 0) {
//			[line strokeLineBetweenPoints:[[traceP[i-1] pointValue]
//									  and:[[traceP[i] pointValue]];
//		}
	}

	[traceP addObject:[NSValue valueWithPoint:B(P,t)]];
	[line drawCircleCenteredAtPoint:[[traceP lastObject] pointValue]
						 withRadius:10.0];
	
//	if (t <= 1-inc) {
//		[line strokeLineBetweenPoints:[[traceP lastObject] pointValue] and:B(P,t+inc)];
//	}

	if (!backwards) {
		t+=inc;
		if (t>=1.0) {
			backwards=YES;
			t=1.0;
		}
	} else {
		t-=inc;
		if (t<=0.0) {
			backwards=NO;
			t=0.0;
		}
	}
	
	//Remove first two "way" of points
	if (traceP.count == 3*pointCount) {
		[traceP removeObjectsAtIndexes:[NSIndexSet indexSetWithIndexesInRange:NSMakeRange(0, 2*pointCount)]];
	}
    return;
}
	
}

- (void)animateOneFrame {
	if (buffered) {
		//Remove first 1/3 part of buffer
		if (superframe == frames.count/3) {
			if (didChange) {
				[frames removeObjectsAtIndexes:[NSIndexSet indexSetWithIndexesInRange:NSMakeRange(0, (int)frames.count/3)]];

				//Hereafter load frames into memory, for saveing energy
				for (NSImage *frame in frames) {
					[frame setCacheMode:NSImageCacheAlways];
				}
				didChange=NO;
			}
		}

		//[(NSImage*)[frames[superframe] lockFocus];
		[(NSImage*)frames[superframe] drawInRect:self.bounds];
		//[(NSImage*)[frames[superframe] unlockFocus];

		(superframe == 2*pointCount-1) ? superframe=0 : superframe++;
	} else {
		[self animateOneSubframe];
	}
	return;
}

- (BOOL)hasConfigureSheet {
    return YES;
}

- (NSWindow*)configureSheet {
	if (_configSheet == nil) {
		if (![NSBundle loadNibNamed:@"configSheet" owner:self]) {
			NSLog(@"Failed to load configuration sheet.");
			NSBeep();
		}
	}
    return _configSheet;
}

- (IBAction)clickOK:(id)sender {
	[[NSApplication sharedApplication] endSheet:_configSheet];
}

- (IBAction)clickCancel:(id)sender {
	[[NSApplication sharedApplication] endSheet:_configSheet];
}

@end
