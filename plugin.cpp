#include "PCH.h"

// =========================
// Prisma UI API (GLOBAL)
// =========================
#include "PrismaUI_API.h"

static void InGameLog(const char* msg)
{
    RE::ConsoleLog::GetSingleton()->Print(msg);
}

PRISMA_UI_API::IVPrismaUI1* PrismaUI = nullptr;

// =========================
// FORWARD DECLARATION
// =========================
static void SKSEMessageHandler(SKSE::MessagingInterface::Message* message);
void InitializeUI();

// =========================
// SKSE LOAD ENTRY POINT
// =========================

bool SKSEPluginLoad(const SKSE::LoadInterface* skse)
{
    SKSE::Init(skse);

    auto messaging = SKSE::GetMessagingInterface();
    if (messaging) {
        messaging->RegisterListener(SKSEMessageHandler);
    } else {
        SKSE::log::error("Messaging interface not found");
        return false;
    }

    SKSE::log::info("EnxyAbilities loaded successfully");

    return true;
}
// =========================
// MESSAGE HANDLER
// =========================
static void SKSEMessageHandler(SKSE::MessagingInterface::Message* message)
{
    if (!message)
        return;

    switch (message->type)
    {
        case SKSE::MessagingInterface::kDataLoaded:
        {
            InGameLog(">>> kDataLoaded OK");
            SKSE::log::info("kDataLoaded OK");
        
            InGameLog(">>> Checking PrismaUI...");
        
            PrismaUI = static_cast<PRISMA_UI_API::IVPrismaUI1*>(
                PRISMA_UI_API::RequestPluginAPI(PRISMA_UI_API::InterfaceVersion::V1)
            );
        
            if (!PrismaUI) {
                InGameLog(">>> PrismaUI NOT FOUND");
                SKSE::log::error("PrismaUI FAILED");
                return;
            }
        
            InGameLog(">>> PrismaUI CONNECTED");
            SKSE::log::info("PrismaUI FOUND");
        
            InitializeUI();
        
            break;
        }

        default:
            break;
    }
}

// =========================
// UI INITIALIZATION
// =========================
void InitializeUI()
{
    InGameLog(">>> InitializeUI START");
    SKSE::log::info("InitializeUI START");

    if (!PrismaUI) {
        InGameLog(">>> PrismaUI NULL in InitUI");
        return;
    }

    InGameLog(">>> Creating View...");

    static PrismaView g_view = 0;

    g_view = PrismaUI->CreateView("EnxyAbilities/setting.html");

    char buffer[64];
    sprintf_s(buffer, ">>> View ID: %llu", g_view);
    InGameLog(buffer);

    if (!PrismaUI->IsValid(g_view)) {
        InGameLog(">>> VIEW INVALID");
        SKSE::log::error("View invalid");
        return;
    }

    InGameLog(">>> VIEW CREATED SUCCESS");

    PrismaUI->Show(g_view);
    PrismaUI->Focus(g_view, true);

    InGameLog(">>> UI SHOWN");
    SKSE::log::info("UI initialized");
}
