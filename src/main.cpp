#include <gtkmm.h>
#include "navigation.h"
#include "list_view.h"

class HelloGtk : public Gtk::Window
{
public:
    HelloGtk() : m_paned(),
                 m_btn(" Hello GTKmm")
    {
        set_border_width(10);

        m_btn.signal_clicked().connect(sigc::mem_fun(*this, &HelloGtk::on_button_clicked));

        add(m_paned);

        m_paned.add1(m_navigation);
        m_paned.add2(m_list_view);
        m_paned.set_position(200);

        show_all_children();
    }

    virtual ~HelloGtk()
    {
    }

protected:
    // signal connected
    void on_button_clicked()
    {
        this->close();
    }

    Navigation m_navigation;
    ListView m_list_view;

    Gtk::Paned m_paned;
    Gtk::Button m_btn;
};

int main(int argc, char *argv[])
{
    auto app = Gtk::Application::create(argc, argv, u8"me.hunter.www");
    //app->set_id("test");

    HelloGtk window;

    window.set_default_size(1024, 768);
    window.set_position(Gtk::WIN_POS_CENTER_ALWAYS);

    return app->run(window);
}