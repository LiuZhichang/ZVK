//
//  singleton.h
//  Test
//
//  Created by 刘致畅 on 2021/9/13.
//

#ifndef singleton_h
#define singleton_h

#include <memory>

namespace ZVK {

    template<typename T>
    class Singleton {
    public:
        static T *GetInstance() {
            static T v;
            return &v;
        }

    protected:
        Singleton() {};

        Singleton(const Singleton &) {}

        Singleton &operator=(const Singleton &) { return *this; }
    };

    template<class T>
    class SingletonPtr {
    public:
        static std::shared_ptr<T> GetInstance() {
            static std::shared_ptr<T> ptr = std::make_shared<T>();
            return ptr;
        }
    };
}

#endif /* singleton_h */
