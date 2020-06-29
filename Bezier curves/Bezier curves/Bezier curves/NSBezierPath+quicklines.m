//
//  NSBezierPath+quicklines.m
//  Bezier curves
//
//  Created by Emil Bahnsen on 14-07-11.
//
//

#import "NSBezierPath+quicklines.h"

@implementation NSBezierPath (quicklines)

- (void)strokeLineBetweenPoints:(NSPoint)A and:(NSPoint)B {
	[self moveToPoint:A];
	[self lineToPoint:B];
	[self stroke];
	[self removeAllPoints];
}

- (void)drawCircleCenteredAtPoint:(NSPoint)A withRadius:(CGFloat)radius {
	[self appendBezierPathWithArcWithCenter:A radius:radius startAngle:0 endAngle:360];
	[self stroke];
	[self removeAllPoints];
}

- (void)appedLineBetweenPoints:(NSPoint)A and:(NSPoint)B {
	[self moveToPoint:A];
	[self lineToPoint:B];
}
- (void)appedCircleCenteredAtPoint:(NSPoint)A withRadius:(CGFloat)radius {
	[self appendBezierPathWithArcWithCenter:A radius:radius startAngle:0 endAngle:360];
}

@end
