//
//  mutex.h
//  Test
//
//  Created by 刘致畅 on 2021/9/13.
//

#ifndef mutex_h
#define mutex_h

#include <pthread.h>
#include <semaphore.h>
#include <stdexcept>
#include <cstdint>

#include "../base/noncopyable.h"

namespace ZVK {

/**
 * @brief 信号量
 */
    class Semaphore : NonCopyable {
    public:
        Semaphore(uint32_t count = 0) {
            if (sem_init(&m_semphore, 0, count)) {
                throw std::logic_error("sem_init error");
            }
        }

        ~Semaphore() {
            sem_destroy(&m_semphore);
        }

        void wait() {
            if (sem_wait(&m_semphore) != 0) {
                throw std::logic_error("sem_wait error");
            }
        }

        void notify() {
            if (sem_post(&m_semphore) != 0) {
                throw std::logic_error("sem_post error");
            }
        }

    private:
        sem_t m_semphore;
    };

/**
 * @brief 范围锁
 *
 * @tparam T 类型
 */
    template<class T>
    class ScopedLockImpl {
    public:

        /**
         * @brief 构造函数
         *
         * @param[in] mutex 互斥锁
         */
        ScopedLockImpl(T &mutex)
                : m_mutex(mutex) {
            m_mutex.lock();
            m_locked = true;
        }

        /**
         * @brief 析构函数
         */
        ~ScopedLockImpl() {
            unlock();
        }

        /**
         * @brief 上锁
         */
        void lock() {
            if (!m_locked) {
                m_mutex.lock();
                m_locked = true;
            }
        }

        /**
         * @brief 解锁
         */
        void unlock() {
            if (m_locked) {
                m_mutex.unlock();
                m_locked = false;
            }
        }

    private:
        T &m_mutex;    // 互斥锁
        bool m_locked; // 锁的状态
    };

/**
 * @brief 读范围锁
 *
 * @tparam T 类型
 */
    template<typename T>
    class RdScopedLockImpl {
    public:

        /**
         * @brief 构咱函数
         *
         * @param[in] mutex 互斥锁
         */
        RdScopedLockImpl(T &mutex)
                : m_mutex(mutex) {
            m_mutex.rdlock();
            m_locked = true;
        }

        /**
         * @brief 析构函数
         */
        ~RdScopedLockImpl() {
            unlock();
        }

        /**
         * @brief 上锁
         */
        void lock() {
            if (!m_locked) {
                m_mutex.rdlock();
                m_locked = true;
            }
        }

        /**
         * @brief 解锁
         */
        void unlock() {
            if (m_locked) {
                m_mutex.unlock();
                m_locked = false;
            }
        }

    private:
        T &m_mutex;   // 互斥锁
        bool m_locked;// 锁的状态
    };

/**
 * @brief 写范围锁
 *
 * @tparam T 类型
 */
    template<class T>
    struct WrScopedLockImpl {
    public:

        /**
         * @brief 构造函数
         *
         * @param[in] mutex 互斥锁
         */
        WrScopedLockImpl(T &mutex)
                : m_mutex(mutex) {
            m_mutex.wrlock();
            m_locked = true;
        }

        /**
         * @brief 析构函数
         */
        ~WrScopedLockImpl() {
            unlock();
        }

        /**
         * @brief 上锁
         */
        void lock() {
            if (!m_locked) {
                m_mutex.wrlock();
                m_locked = true;
            }
        }

        /**
         * @brief 解锁
         */
        void unlock() {
            if (m_locked) {
                m_mutex.unlock();
                m_locked = false;
            }
        }

    private:
        T &m_mutex;   // 互斥锁
        bool m_locked;// 锁的状态
    };

/**
 * @brief 互斥锁
 */
    class Mutex : NonCopyable {
    public:
        typedef ScopedLockImpl<Mutex> Lock;

        /**
         * @brief 构造函数,初始化互斥锁
         */
        Mutex() {
            pthread_mutex_init(&m_mutex, nullptr);
        }

        /**
         * @brief 析构函数,销毁互斥锁
         */
        ~Mutex() {
            pthread_mutex_destroy(&m_mutex);
        }

        /**
         * @brief 上锁
         */
        void lock() {
            pthread_mutex_lock(&m_mutex);
        }

        /**
         * @brief 解锁
         */
        void unlock() {
            pthread_mutex_unlock(&m_mutex);
        }

    private:
        pthread_mutex_t m_mutex; // 互斥量
    };

/**
 * @brief 读写互斥锁
 */
    class RWMutex : NonCopyable {
    public:
        typedef RdScopedLockImpl<RWMutex> ReadLock;
        typedef WrScopedLockImpl<RWMutex> WriteLock;

        /**
         * @brief 构造函数,初始化读写互斥锁
         */
        RWMutex() {
            pthread_rwlock_init(&m_lock, nullptr);
        }

        /**
         * @brief 析构函数,销毁互斥锁
         */
        ~RWMutex() {
            pthread_rwlock_destroy(&m_lock);
        }

        /**
         * @brief 上写锁
         */
        void rdlock() {
            pthread_rwlock_rdlock(&m_lock);
        }

        /**
         * @brief 上读锁
         */
        void wrlock() {
            pthread_rwlock_wrlock(&m_lock);
        }

        /**
         * @brief 解锁
         */
        void unlock() {
            pthread_rwlock_unlock(&m_lock);
        }

    private:
        pthread_rwlock_t m_lock; //读写互斥锁
    };

}

#endif /* mutex_h */
