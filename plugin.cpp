#include "PCH.h"

namespace {
    void OnMessage(SKSE::MessagingInterface::Message* msg)
    {
        if (msg->type == SKSE::MessagingInterface::kDataLoaded) {
            RE::ConsoleLog::GetSingleton()->Print("EnxyAbilities loaded!");
        }
    }
}

extern "C" DLLEXPORT bool SKSEPlugin_Load(const SKSE::LoadInterface* skse)
{
    SKSE::Init(skse);

    auto messaging = SKSE::GetMessagingInterface();
    if (messaging) {
        messaging->RegisterListener(OnMessage);
    }

    return true;
}
