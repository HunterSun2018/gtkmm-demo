#include <gtkmm.h>
//  to compile this file run the following command
//  c++ -O3 src/single_demo.cpp -o demo `pkg-config gtkmm-3.0 --cflags --libs`

int main(int argc, char* argv[])
{
    using namespace Gtk;
    auto app = Application::create(argc, argv, u8"me.hunter.www");

    Window window;
    window.set_border_width(10);
    window.set_default_size(1024, 576);
    window.set_position(Gtk::WIN_POS_CENTER_ALWAYS);

    Button btn(u8"Gtkmm Button");    
    btn.signal_clicked().connect([&]() {
        MessageDialog dialog(window, "This is a MessageDialog");
        dialog.set_secondary_text("And this is the secondary text that explains things.");

        dialog.run();

        window.close();
        });

    window.add(btn);
    window.show_all();

    app->run(window);
}