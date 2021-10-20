//
//  ScopeGuard.h
//  VulkanTemplate
//
//  Created by 刘致畅 on 2021/9/16.
//

#ifndef ScopeGuard_h
#define ScopeGuard_h

#include <utility>

#define __SCOPEGUARD_CAT_IMPL(s1, s2) s1##s2
#define __SCOPEGUARD_CAT(s1, s2) __SCOPEGUARD_CAT_IMPL(s1,s2)
#define ZVK_ON_SCOPE_EXIT \
    auto __SCOPEGUARD_CAT(exit_block__,__LINE__) = ZVK::detail::ScopeGuardOnExit() + [&]()

namespace ZVK {


/**
 * @brief 利用RAII机制,当对象构造完成时,编译器会保证当程序结束时会调用对应的析构(发生异常也会)
 *
 * @tparam func 函数(这里使用lambda表达式)
 */
    template<typename func>
    class scopeguard {
    public:
        scopeguard(func &&fun) : fun_(std::forward<func>(fun)), dismiss_(false) {}

        scopeguard(scopeguard &&oth) : fun_(std::move(oth.fun_)), dismiss_(oth.dismiss_) {}

        ~scopeguard() {
            if (!dismiss_)
                fun_();
        }

        void dismissed() {
            dismiss_ = true;
        }

        scopeguard(const scopeguard &) = delete;

        scopeguard operator=(const scopeguard &) = delete;

    private:
        func fun_;
        bool dismiss_;
    };

    namespace detail {

        enum class ScopeGuardOnExit {
        };

        template<typename func>
        scopeguard<func> operator+(ScopeGuardOnExit, func &&fun) {
            return scopeguard<func>(std::forward<func>(fun));
        }

    }

};

#endif /* ScopeGuard_h */
