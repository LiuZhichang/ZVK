//
//  noncopyable.h
//  Test
//
//  Created by 刘致畅 on 2021/9/13.
//

#ifndef noncopyable_h
#define noncopyable_h

namespace ZVK {

    class NonCopyable {
    public:
        NonCopyable() = default;

        ~NonCopyable() = default;

        NonCopyable(const NonCopyable &) = delete;

        NonCopyable &operator=(const NonCopyable &) = delete;

    private:
    };

}

#endif /* noncopyable_h */
