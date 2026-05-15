#include "PCH.h"

// =========================
// Prisma UI API (GLOBAL)
// =========================
#include "PrismaUI_API.h"

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
            SKSE::log::info("Game data loaded");

            // =========================
            // INIT PRISMA UI API
            // =========================
            PrismaUI = static_cast<PRISMA_UI_API::IVPrismaUI1*>(
                PRISMA_UI_API::RequestPluginAPI(PRISMA_UI_API::InterfaceVersion::V1)
            );

            if (!PrismaUI) {
                SKSE::log::error("Failed to initialize PrismaUI API");
                return;
            }

            SKSE::log::info("PrismaUI API initialized successfully");

            // =========================
            // INIT YOUR UI SYSTEM
            // =========================
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
    SKSE::log::info("Initializing EnxyAbilities UI...");

    if (!PrismaUI) {
        SKSE::log::error("PrismaUI is null during UI init");
        return;
    }

    // TODO:
    // - register UI view
    // - bind JS bridge
    // - load config
    // - setup hotkey sync

    SKSE::log::info("EnxyAbilities UI initialized");
}
