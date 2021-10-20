//
//  logmanager.hpp
//  Test
//
//  Created by 刘致畅 on 2021/9/13.
//

#ifndef logmanager_hpp
#define logmanager_hpp

#include "log.h"
#include "../base/singleton.h"

#include <map>

#define ZVK_LOG_LEVEL(logger, level) \
    if(logger->getLevel() <= level)\
        ZVK::LogWrap(logger,ZVK::LogEvent::ptr(\
            new ZVK::LogEvent(level, __FILE__,__func__,__LINE__ ,0\
                ,std::this_thread::get_id(), time(0),logger->getName()))\
                ).getStrIO()

#define ZVK_LOG_DEBUG(logger) ZVK_LOG_LEVEL(logger,ZVK::LogLevel::Level::DEBUG)
#define ZVK_LOG_INFO(logger)  ZVK_LOG_LEVEL(logger,ZVK::LogLevel::Level::INFO)
#define ZVK_LOG_WARN(logger)  ZVK_LOG_LEVEL(logger,ZVK::LogLevel::Level::WARN)
#define ZVK_LOG_ERROR(logger) ZVK_LOG_LEVEL(logger,ZVK::LogLevel::Level::ERROR)
#define ZVK_LOG_FATAL(logger) ZVK_LOG_LEVEL(logger,ZVK::LogLevel::Level::FATAL)

#define ZVK_LOG_ROOT() ZVK::LogManager::GetInstance()->getRoot()
#define ZVK_LOG_NAME(name) ZVK::LogManager::GetInstance()->getLogger(name)
#define ZVK_SYS_LOG() ZVK::LogManager::GetInstance()->getLogger("system")

namespace ZVK {

/**
 * @brief 日志管理器
 */
    class LoggerManager {
    public:
        typedef Mutex Mutex_t;

        /**
         * @brief 日志管理器构造函数
         */
        LoggerManager() {
            m_root.reset(new Logger);
            m_root->addAppender(std::make_shared<StdOutLogAppender>());
            m_loggers[m_root->getName()] = m_root;
        }

        /**
         * @brief 获取日志器
         *
         * @param[in] name 日志器名
         *
         * @return 日志器
         */
        Logger::ptr getLogger(const std::string &name) {
            Mutex_t::Lock lock(m_mutex);
            auto it = m_loggers.find(name);
            if (it != m_loggers.end())
                return it->second;
            Logger::ptr logger = std::make_shared<Logger>(name);
            m_loggers[name] = logger;
            return logger;
        }

        /**
         * @brief 获取主日志器
         *
         * @return 主日志器
         */
        Logger::ptr getRoot() const {
            return m_root;
        }

    private:
        Logger::ptr m_root;                             // 主日志器
        std::map<std::string, Logger::ptr> m_loggers;   // 所有日志器
        Mutex_t m_mutex;                         // 互斥量
    };


    typedef SingletonPtr<LoggerManager> LogManager;


}

#endif /* logmanager_hpp */
