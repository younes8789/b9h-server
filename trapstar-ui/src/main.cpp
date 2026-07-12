#include <sdk.hpp>

// ============================================================
//  Trapstar FiveM-Style UI - open.mp Component
//  Author: B9h Crew
// ============================================================

// المؤشرات (Pointers) ديال الـ TextDraws
static ITextDraw* trapBackground = nullptr;
static ITextDraw* trapLineBorder = nullptr;
static ITextDraw* trapText      = nullptr;

// ============================================================
//  دالة إنشاء الـ UI
// ============================================================
static void CreateTrapstarUI(ITextDrawsComponent* td)
{
    if (!td) return;

    // 1. الـ Background: مستطيل زجاجي أسود شفاف فوق الشاشة
    trapBackground = td->create(Vector2(200.0f, 15.0f), "_");
    if (trapBackground)
    {
        trapBackground->setTextSize(Vector2(440.0f, 28.0f));
        trapBackground->setAlignment(TextDrawAlignment::Left);
        trapBackground->setUseBox(true);
        trapBackground->setBoxColour(Colour::FromRGBA(0x0A0A0A99));
        trapBackground->setShadow(0);
        trapBackground->setOutline(0);
    }

    // 2. الـ Bottom Border Line: شريط أحمر رقيق تحت الـ Box
    trapLineBorder = td->create(Vector2(200.0f, 41.0f), "_");
    if (trapLineBorder)
    {
        trapLineBorder->setTextSize(Vector2(440.0f, 2.0f));
        trapLineBorder->setAlignment(TextDrawAlignment::Left);
        trapLineBorder->setUseBox(true);
        trapLineBorder->setBoxColour(Colour::FromRGBA(0xFF0055FF));
        trapLineBorder->setShadow(0);
        trapLineBorder->setOutline(0);
    }

    // 3. النص الأساسي: "T R A P S T A R" بـ لون أحمر على "STAR"
    trapText = td->create(Vector2(320.0f, 18.0f), "T R A P {FF0055}S T A R");
    if (trapText)
    {
        trapText->setAlignment(TextDrawAlignment::Center);
        trapText->setFont(TextDrawFont::Font2);
        trapText->setLetterSize(Vector2(0.4f, 1.6f));
        trapText->setColour(Colour::FromRGBA(0xF5F5F5FF));
        trapText->setShadow(0);
        trapText->setOutline(0);
        trapText->setProportional(true);
    }
}

// ============================================================
//  إظهار الـ UI لجميع اللاعبين
// ============================================================
static void ShowTrapstarToAll(IPlayerPool& players)
{
    if (!trapBackground || !trapLineBorder || !trapText) return;

    for (IPlayer* player : players.entries())
    {
        if (!player) continue;
        trapBackground->showForPlayer(*player);
        trapLineBorder->showForPlayer(*player);
        trapText->showForPlayer(*player);
    }
}

// ============================================================
//  الـ Component الرئيسي
// ============================================================
struct TrapstarUIComponent final : public IComponent, public PlayerConnectEventHandler
{
    PROVIDE_UID(/* UID فريد */ 0xB9C1AB7E3F5D8A12ULL);

    ICore*               core_      = nullptr;
    ITextDrawsComponent* textdraws_ = nullptr;
    IPlayerPool*         players_   = nullptr;

    StringView componentName() const override
    {
        return "TrapstarUI";
    }

    SemanticVersion componentVersion() const override
    {
        return SemanticVersion(1, 0, 0, 0);
    }

    void onLoad(ICore* c) override
    {
        core_    = c;
        players_ = &c->getPlayers();
    }

    void onInit(IComponentList* components) override
    {
        textdraws_ = components->queryComponent<ITextDrawsComponent>();
        if (!textdraws_)
        {
            core_->logLn(LogLevel::Error, "[TrapstarUI] TextDraws component not found!");
            return;
        }

        // ننشئوا الـ TextDraws عند إقلاع السيرفر
        CreateTrapstarUI(textdraws_);

        // نشترك في حدث connection ديال اللعابة
        players_->getPlayerConnectDispatcher().addEventHandler(this);

        core_->logLn(LogLevel::Message, "[TrapstarUI] Loaded successfully!");
    }

    void onReady() override
    {
        // السيرفر جاهز تماماً، نظهروا الـ UI لكاع اللاعبين الموجودين
        if (players_) ShowTrapstarToAll(*players_);
    }

    void onFree(IComponent* component) override {}

    void free() override
    {
        if (players_)
            players_->getPlayerConnectDispatcher().removeEventHandler(this);

        trapBackground = nullptr;
        trapLineBorder = nullptr;
        trapText       = nullptr;

        delete this;
    }

    void reset() override {}

    // ============================================================
    //  كيفاش نظهروا الـ UI لكل لاعب جديد كيدخل
    // ============================================================
    void onPlayerConnect(IPlayer& player) override
    {
        if (trapBackground) trapBackground->showForPlayer(player);
        if (trapLineBorder) trapLineBorder->showForPlayer(player);
        if (trapText)       trapText->showForPlayer(player);
    }
};

// ============================================================
//  Entry Point ديال الـ Component (يتحمل من open.mp تلقائياً)
// ============================================================
COMPONENT_ENTRY_POINT()
{
    return new TrapstarUIComponent();
}
