#include <sdk.hpp>
#include <Server/Components/TextDraws/textdraws.hpp>

// ============================================================
//  المؤشرات الـ Global
// ============================================================
static ITextDraw* trapBackground = nullptr;
static ITextDraw* trapLineBorder = nullptr;
static ITextDraw* trapText       = nullptr;

// ============================================================
//  دالة إنشاء الـ UI
//  الـ API الصحيحة مأخوذة مباشرة من textdraws.hpp
// ============================================================
static void CreateTrapstarUI(ITextDrawsComponent* td)
{
    if (!td) return;

    // 1. Background زجاجي شفاف
    trapBackground = td->create(Vector2(200.0f, 15.0f), "_");
    if (trapBackground)
    {
        trapBackground->setTextSize(Vector2(440.0f, 28.0f));
        trapBackground->setAlignment(TextDrawAlignment_Left);   // TextDrawAlignmentTypes enum
        trapBackground->useBox(true);                           // useBox() مش setUseBox()
        trapBackground->setBoxColour(Colour::FromRGBA(0x0A0A0A99)); // setBoxColour() British spelling
        trapBackground->setShadow(0);
        trapBackground->setOutline(0);
    }

    // 2. خط أحمر رقيق
    trapLineBorder = td->create(Vector2(200.0f, 41.0f), "_");
    if (trapLineBorder)
    {
        trapLineBorder->setTextSize(Vector2(440.0f, 2.0f));
        trapLineBorder->setAlignment(TextDrawAlignment_Left);
        trapLineBorder->useBox(true);
        trapLineBorder->setBoxColour(Colour::FromRGBA(0xFF0055FF));
        trapLineBorder->setShadow(0);
        trapLineBorder->setOutline(0);
    }

    // 3. النص الأساسي
    trapText = td->create(Vector2(320.0f, 18.0f), "T R A P {FF0055}S T A R");
    if (trapText)
    {
        trapText->setAlignment(TextDrawAlignment_Center);
        trapText->setStyle(TextDrawStyle_FontBankGothic);       // setStyle() + TextDrawStyle enum
        trapText->setLetterSize(Vector2(0.4f, 1.6f));
        trapText->setColour(Colour::FromRGBA(0xF5F5F5FF));
        trapText->setShadow(0);
        trapText->setOutline(0);
        trapText->setProportional(true);
    }
}

// ============================================================
//  الـ Component الرئيسي
// ============================================================
struct TrapstarComponent final
    : public IComponent
    , public PlayerConnectEventHandler
{
    PROVIDE_UID(0xB9C1AB7E3F5D8A12ULL);

    ICore*               core_      = nullptr;
    ITextDrawsComponent* textdraws_ = nullptr;

    StringView componentName() const override { return "trapstar"; }

    SemanticVersion componentVersion() const override
    {
        return SemanticVersion(1, 0, 0, 0);
    }

    void onLoad(ICore* c) override
    {
        core_ = c;
    }

    void onInit(IComponentList* components) override
    {
        textdraws_ = components->queryComponent<ITextDrawsComponent>();
        if (!textdraws_)
        {
            core_->logLn(LogLevel::Error, "[trapstar] TextDraws component not found!");
            return;
        }

        CreateTrapstarUI(textdraws_);

        core_->getPlayers().getPlayerConnectDispatcher().addEventHandler(this);
        core_->logLn(LogLevel::Message, "[trapstar] Loaded OK!");
    }

    void onReady() override
    {
        // إظهار الـ UI لكاع اللاعبين الموجودين
        for (IPlayer* player : core_->getPlayers().entries())
        {
            if (!player) continue;
            if (trapBackground) trapBackground->showForPlayer(*player);
            if (trapLineBorder) trapLineBorder->showForPlayer(*player);
            if (trapText)       trapText->showForPlayer(*player);
        }
    }

    // كل لاعب جديد يدخل يشوف الـ UI تلقائياً
    void onPlayerConnect(IPlayer& player) override
    {
        if (trapBackground) trapBackground->showForPlayer(player);
        if (trapLineBorder) trapLineBorder->showForPlayer(player);
        if (trapText)       trapText->showForPlayer(player);
    }

    void onFree(IComponent*) override {}

    void free() override
    {
        if (core_)
            core_->getPlayers().getPlayerConnectDispatcher().removeEventHandler(this);
        trapBackground = nullptr;
        trapLineBorder = nullptr;
        trapText       = nullptr;
        delete this;
    }

    void reset() override {}
};

// ============================================================
//  Entry Point — open.mp يحمله تلقائياً
// ============================================================
COMPONENT_ENTRY_POINT()
{
    return new TrapstarComponent();
}
