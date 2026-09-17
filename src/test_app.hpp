#pragma once

#include <miqutoolkit/miqutoolkit.hpp>
#include <memory>

namespace miqutest {

class TestApp {
public:
    explicit TestApp(std::shared_ptr<miqu::AppEngine> engine);
    ~TestApp() = default;

    bool init();
    int run();

private:
    void setup_ui();
    void handle_key(const miqu::KeyPressEvent& ev);

    std::shared_ptr<miqu::AppEngine> m_engine;
    std::shared_ptr<miqu::Window> m_window;
};

} // namespace miqutest
