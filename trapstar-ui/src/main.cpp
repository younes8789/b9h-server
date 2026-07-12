#include <sdk.hpp>
#include <Server/Components/TextDraws/textdraws.hpp>

// ============================================================
//  الـ Component الرئيسي — Per-Player TextDraws
//  ما تحتاجش ITextDrawsComponent — كل لاعب عنده نسخته
// ============================================================
struct TrapstarComponent final
    : public IComponent
    , public PlayerConnectEventHandler
{
    PROVIDE_UID(0xB9C1AB7E3F5D8A12ULL);

    ICore* core_ = nullptr;

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
        core_->getPlayers().getPlayerConnectDispatcher().addEventHandler(this);
        core_->logLn(LogLevel::Message, "[trapstar] Loaded OK!");
    }

    void onReady() override {}

    // ============================================================
    //  كيفما دخل لاعب، نصاوبوا الـ TextDraws ديالو مباشرة
    // ============================================================
    void onPlayerConnect(IPlayer& player) override
    {
        // نجيبوا الـ Per-Player TextDraw pool ديال هاد اللاعب
        IPlayerTextDrawData* tdData = player.queryExtension<IPlayerTextDrawData>();
        if (!tdData)
        {
            core_->logLn(LogLevel::Warning, "[trapstar] No IPlayerTextDrawData for player %d", player.getID());
            return;
        }

        // 1. Background زجاجي شفاف
        IPlayerTextDraw* bg = tdData->create(Vector2(200.0f, 15.0f), "_");
        if (bg)
        {
            bg->setTextSize(Vector2(440.0f, 28.0f));
            bg->setAlignment(TextDrawAlignment_Left);
            bg->useBox(true);
            bg->setBoxColour(Colour::FromRGBA(0x0A0A0A99));
            bg->setShadow(0);
            bg->setOutline(0);
            bg->show();
        }

        // 2. خط أحمر رقيق
        IPlayerTextDraw* line = tdData->create(Vector2(200.0f, 41.0f), "_");
        if (line)
        {
            line->setTextSize(Vector2(440.0f, 2.0f));
            line->setAlignment(TextDrawAlignment_Left);
            line->useBox(true);
            line->setBoxColour(Colour::FromRGBA(0xFF0055FF));
            line->setShadow(0);
            line->setOutline(0);
            line->show();
        }

        // 3. النص الأساسي
        IPlayerTextDraw* txt = tdData->create(Vector2(320.0f, 18.0f), "T R A P {FF0055}S T A R");
        if (txt)
        {
            txt->setAlignment(TextDrawAlignment_Center);
            txt->setStyle(TextDrawStyle_FontBankGothic);
            txt->setLetterSize(Vector2(0.4f, 1.6f));
            txt->setColour(Colour::FromRGBA(0xF5F5F5FF));
            txt->setShadow(0);
            txt->setOutline(0);
            txt->setProportional(true);
            txt->show();
        }
    }

    void onFree(IComponent*) override {}

    void free() override
    {
        if (core_)
            core_->getPlayers().getPlayerConnectDispatcher().removeEventHandler(this);
        delete this;
    }

    void reset() override {}
};

// ============================================================
//  Entry Point
// ============================================================
COMPONENT_ENTRY_POINT()
{
    return new TrapstarComponent();
}
