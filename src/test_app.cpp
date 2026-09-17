#include "test_app.hpp"
#include <xkbcommon/xkbcommon-keysyms.h>
#include <iostream>

using namespace miqu;

namespace miqutest {

TestApp::TestApp(std::shared_ptr<AppEngine> engine)
    : m_engine(std::move(engine)) {
}

bool TestApp::init() {
    if (!m_engine) return false;
    setup_ui();
    return m_window != nullptr;
}

void TestApp::setup_ui() {
    auto root_container = std::make_shared<LinearLayout>(Orientation::Vertical);
    root_container->set_layout_params(LayoutParams(
        static_cast<int>(LayoutDimension::MatchParent),
        static_cast<int>(LayoutDimension::MatchParent)
    ));
    root_container->set_padding(0);

    // Toolbar
    auto toolbar = ToolbarBuilder::create()
        ->title("miqutest")
        ->subtitle("Widget Testing & Playground")
        ->titleAlignment(TitleAlignment::Center)
        ->onClose([this]() {
            if (m_engine) m_engine->quit();
        })
        ->build();
    toolbar->set_margin(14, 12, 14, 8);
    root_container->add_view(toolbar);

    // Main content area
    auto content_col = std::make_shared<LinearLayout>(Orientation::Vertical);
    content_col->set_layout_params(LayoutParams(
        static_cast<int>(LayoutDimension::MatchParent),
        static_cast<int>(LayoutDimension::WrapContent)
    ));
    content_col->set_padding(20, 16);

    // Welcome / Hello World Card
    auto hello_card = CardViewBuilder::create()
        ->cornerRadius(12)
        ->padding(20, 16)
        ->build();
    hello_card->set_margin(0, 0, 0, 16);

    auto card_col = std::make_shared<LinearLayout>(Orientation::Vertical);
    card_col->set_layout_params(LayoutParams(
        static_cast<int>(LayoutDimension::MatchParent),
        static_cast<int>(LayoutDimension::WrapContent)
    ));

    auto greeting_text = TextViewBuilder::create()
        ->text("Hello, World! 👋")
        ->h1()
        ->bold(true)
        ->build();
    greeting_text->set_margin(0, 0, 0, 8);
    card_col->add_view(greeting_text);

    auto subtitle_text = TextViewBuilder::create()
        ->text("Welcome to miqutest — a test harness and playground for miqutoolkit & miquland widgets.")
        ->textSize(13)
        ->muted(true)
        ->wrap(true)
        ->build();
    subtitle_text->set_margin(0, 0, 0, 16);
    card_col->add_view(subtitle_text);

    // Interactive counter button
    auto click_count = std::make_shared<int>(0);
    auto status_label = TextViewBuilder::create()
        ->text("Button not clicked yet.")
        ->textSize(12)
        ->muted(true)
        ->build();
    status_label->set_margin(0, 0, 0, 12);

    auto test_btn = ButtonBuilder::create()
        ->text("Click Me!")
        ->primary(true)
        ->cornerRadius(8)
        ->padding(16, 10)
        ->onClick([this, click_count, status_label]() {
            (*click_count)++;
            status_label->set_text("Button clicked " + std::to_string(*click_count) + " time(s)!");
            if (m_window) m_window->schedule_redraw();
        })
        ->build();
    test_btn->set_margin(0, 0, 0, 8);

    card_col->add_view(test_btn);
    card_col->add_view(status_label);

    hello_card->add_view(card_col);
    content_col->add_view(hello_card);

    auto scroll_view = ScrollViewBuilder::create()
        ->contentView(content_col)
        ->build();
    scroll_view->set_layout_params(LayoutParams(
        static_cast<int>(LayoutDimension::MatchParent),
        0,
        1.0f
    ));
    scroll_view->set_margin(14, 0, 14, 14);
    root_container->add_view(scroll_view);

    // Window
    m_window = WindowBuilder::create()
        ->title("miqutest")
        ->appId("miqutest")
        ->role(WindowRole::Toplevel)
        ->preferredSize(520, 480)
        ->closeOnEscape(true)
        ->contentView(root_container)
        ->onClose([this]() {
            if (m_engine) m_engine->quit();
        })
        ->onKey([this](const KeyPressEvent& ev) {
            handle_key(ev);
        })
        ->build();

    if (m_window) {
        m_window->show();
    }
}

void TestApp::handle_key(const KeyPressEvent& ev) {
    if (!ev.pressed) return;
    if (ev.keysym == XKB_KEY_q || ev.keysym == XKB_KEY_Q || ev.keysym == XKB_KEY_Escape) {
        if (m_engine) m_engine->quit();
    }
}

int TestApp::run() {
    if (!m_engine) return 1;
    return m_engine->enter_loop();
}

} // namespace miqutest
