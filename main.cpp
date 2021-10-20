#include <iostream>
#include "app/Application.h"

#include "zvk/log/logmanager.h"
#include "zvk/ZVKContext.h"

static ZVK::Logger::ptr logger = ZVK_SYS_LOG();

int main(int argc,char** argv) {

    logger->addAppender(std::make_shared<ZVK::StdOutLogAppender>());
    
    std::string pwd = *argv;

    ZVK::Context context;

    /// 设置窗口属性
    ZVK::Window &window = context.window;
    window->title = "ZVK-Main-window";
    window->width = 1024;
    window->height = 768;

    /// 设置APP属性
    ZVK::Instance &inst = context.instance;
    inst->engine = "Application Engine";
    inst->application = "Application";

    context.pipeline->vertexShader = (pwd + "/shader/vert.spv").data();
    context.pipeline->fragmentShader = (pwd + "/shader/frag.spv").data();

    ZVK_LOG_INFO(logger) << "start-----";

    Application::Instance()->run(context);

    ZVK_LOG_INFO(logger) << "end  -----";

    return 0;
}
