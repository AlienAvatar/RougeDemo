#pragma once

#include "Modules/ModuleInterface.h"
#include "Modules/ModuleManager.h"

class IRougeDemoLoadingScreenModule : public IModuleInterface
{
public:
    static inline IRougeDemoLoadingScreenModule& Get()
    {
        return FModuleManager::LoadModuleChecked<IRougeDemoLoadingScreenModule>("LoadingScreen");
    }

    // 在游戏加载时关闭加载屏幕(不是启动时)
    virtual void StartInGameLoadingScreen(bool bPlayUntilStopped, float PlayTime) = 0;

    // 停止加载
    virtual void StopInGameLoadingScreen() = 0;
};
