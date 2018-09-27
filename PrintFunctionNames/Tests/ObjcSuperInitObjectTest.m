#import <Foundation/Foundation.h>

@interface OSI_CorrectInitWithSuper : NSObject

@end

@implementation OSI_CorrectInitWithSuper

- (instancetype)init {
    self = [super init];
    if (self) {
        NSLog(@"This init is correct");
    }
    return self;
}

@end

@interface OSI_CorrectInitWithSelf : NSObject

@end

@implementation OSI_CorrectInitWithSelf

- (instancetype)initWithHello:(NSString *)helloMessage {
    self = [super init];
    if (self) {
        NSLog(@"This init is correct: %@", helloMessage);
    }
    return self;
}

- (instancetype)init {
    self = [self initWithHello:@"Hello World!"];
    return self;
}

@end

@interface OSI_InvalidInit : NSObject

@end

@implementation OSI_InvalidInit

- (instancetype)initWithHello:(NSString *)helloMessage {
    NSLog(@"This init is invalid: %@", helloMessage);
    return self;
}

- (instancetype)init {
    return self;
}

@end
