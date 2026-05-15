#include "PCH.h"
#include "PrismaUI_API.h"
#include <filesystem>

static bool g_uiVisible = false;
static PrismaView g_view = 0;

static void InGameLog(const char* msg)
{
    if (RE::ConsoleLog::GetSingleton()) {
        RE::ConsoleLog::GetSingleton()->Print(msg);
    }
}

PRISMA_UI_API::IVPrismaUI1* PrismaUI = nullptr;

// =========================
// FORWARD DECLARATIONS
// =========================
void InitializeUI();
void ToggleUI();

// =========================
// INPUT HANDLER
// =========================
class InputHandler : public RE::BSTEventSink<RE::InputEvent*>
{
public:
    static InputHandler* GetSingleton()
    {
        static InputHandler instance;
        return &instance;
    }

    RE::BSEventNotifyControl ProcessEvent(
        RE::InputEvent* const* a_events,
        RE::BSTEventSource<RE::InputEvent*>*) override
    {
        if (!a_events)
            return RE::BSEventNotifyControl::kContinue;

        for (auto event = *a_events; event; event = event->next)
        {
            auto button = event->AsButtonEvent();
            // Memastikan input berasal dari Keyboard dan tombol sedang ditekan
            if (!button || !button->IsDown() || button->GetDevice() != RE::INPUT_DEVICE::kKeyboard)
                continue;

            // PERBAIKAN: Menggunakan 0x40 untuk tombol F6 (0x3F adalah F5)
            if (button->GetIDCode() == 0x40) 
            {
                ToggleUI();
            }
        }

        return RE::BSEventNotifyControl::kContinue;
    }
};

static void SKSEMessageHandler(SKSE::MessagingInterface::Message* message);

// =========================
// SKSE LOAD ENTRY POINT
// =========================
SKSEPluginLoad(const SKSE::LoadInterface* skse)
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
            
            auto input = RE::BSInputDeviceManager::GetSingleton();
            if (input)
            {
                input->AddEventSink(InputHandler::GetSingleton());
                InGameLog("Input Handler Registered");
            }
        
            InitializeUI();
            break;
        }

        default:
            break;
    }
}

// =========================
// UI FUNCTIONS
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

    // PERBAIKAN: Disederhanakan menjadi "EnxyAbilities/setting.html"
    // PrismaUI akan otomatis mencari ke file:///views/EnxyAbilities/setting.html
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

    PrismaUI->Hide(g_view); // Mulai dalam keadaan tersembunyi

    InGameLog(">>> UI INITIALIZED");
    SKSE::log::info("UI initialized");
}

void ToggleUI()
{
    if (!PrismaUI || !PrismaUI->IsValid(g_view))
        return;

    if (g_uiVisible)
    {
        PrismaUI->Hide(g_view);
        PrismaUI->Unfocus(g_view);
        g_uiVisible = false;
        InGameLog("UI HIDDEN");
    }
    else
    {
        PrismaUI->Show(g_view);
        PrismaUI->Focus(g_view, false);
        g_uiVisible = true;
        InGameLog("UI SHOWN");
    }
}