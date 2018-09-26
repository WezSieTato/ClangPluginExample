#import <Foundation/Foundation.h>

@interface ToyObject : NSObject

@end

@implementation ToyObject

@end

@interface bad_ToyObject : NSObject

@end

@implementation bad_ToyObject

@end

@interface ToyObjectMissingSuperInit : NSObject

- (instancetype)init {
        self = [super init];
        return self;
}

@end

@implementation ToyObjectMissingSuperInit

- (instancetype)init
{
//    self = [super init];
//    return self;
}

@end
