//
//  NSBezierPath+quicklines.h
//  Bezier curves
//
//  Created by Emil Bahnsen on 14-07-11.
//
//

#import <Cocoa/Cocoa.h>

@interface NSBezierPath (quicklines)

- (void)strokeLineBetweenPoints:(NSPoint)A and:(NSPoint)B;
- (void)drawCircleCenteredAtPoint:(NSPoint)A withRadius:(CGFloat)radius;

- (void)appedLineBetweenPoints:(NSPoint)A and:(NSPoint)B;
- (void)appedCircleCenteredAtPoint:(NSPoint)A withRadius:(CGFloat)radius;

@end
