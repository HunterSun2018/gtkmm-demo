#include <iostream>
#include <cpprest/filestream.h>
#include <cpprest/http_client.h>
#include "list_view.h"

using namespace utility;
using namespace web::http;
using namespace web::http::client;
using namespace concurrency::streams;

ListView::ListView()
    : Gtk::Box(Gtk::ORIENTATION_VERTICAL),
      m_Button_Quit("Quit")
{
    set_border_width(5);

    //Add the TreeView, inside a ScrolledWindow, with the button underneath:
    m_ScrolledWindow.add(m_TreeView);

    //Only show the scrollbars when they are necessary:
    m_ScrolledWindow.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);

    pack_start(m_ScrolledWindow);
    pack_start(m_ButtonBox, Gtk::PACK_SHRINK);

    m_ButtonBox.pack_start(m_Button_Quit, Gtk::PACK_SHRINK);
    m_ButtonBox.set_border_width(5);
    m_ButtonBox.set_layout(Gtk::BUTTONBOX_END);
    m_Button_Quit.signal_clicked().connect(sigc::mem_fun(*this,
                                                         &ListView::on_button_quit));

    //Create the Tree model:
    m_refTreeModel = Gtk::ListStore::create(m_Columns);
    m_TreeView.set_model(m_refTreeModel);

    //Fill the TreeView's model
    Gtk::TreeModel::Row row = *(m_refTreeModel->append());
    row[m_Columns.m_col_id] = 1;
    row[m_Columns.m_col_name] = "Billy Bob";
    row[m_Columns.m_col_number] = 10;
    row[m_Columns.m_col_percentage] = 15;

    row = *(m_refTreeModel->append());
    row[m_Columns.m_col_id] = 2;
    row[m_Columns.m_col_name] = "Joey Jojo";
    row[m_Columns.m_col_number] = 20;
    row[m_Columns.m_col_percentage] = 40;

    row = *(m_refTreeModel->append());
    row[m_Columns.m_col_id] = 3;
    row[m_Columns.m_col_name] = "Rob McRoberts";
    row[m_Columns.m_col_number] = 30;
    row[m_Columns.m_col_percentage] = 70;

    //Add the TreeView's view columns:
    //This number will be shown with the default numeric formatting.
    m_TreeView.append_column("ID", m_Columns.m_col_id);
    m_TreeView.append_column("Name", m_Columns.m_col_name);

    m_TreeView.append_column_numeric("Formatted number", m_Columns.m_col_number,
                                     "%010d" /* 10 digits, using leading zeroes. */);

    //Display a progress bar instead of a decimal number:
    auto cell = Gtk::make_managed<Gtk::CellRendererProgress>();
    int cols_count = m_TreeView.append_column("Some percentage", *cell);
    auto pColumn = m_TreeView.get_column(cols_count - 1);
    if (pColumn)
    {
        pColumn->add_attribute(cell->property_value(), m_Columns.m_col_percentage);
    }

    //Make all the columns reorderable:
    //This is not necessary, but it's nice to show the feature.
    //You can use TreeView::set_column_drag_function() to more
    //finely control column drag and drop.
    for (guint i = 0; i < 2; i++)
    {
        auto column = m_TreeView.get_column(i);
        column->set_reorderable();
    }
}

ListView::~ListView()
{
}

/* Can pass proxy information via environment variable http_proxy.
   Example:
   Linux:   export http_proxy=http://192.1.8.1:8080
 */
web::http::client::http_client_config client_config_for_proxy()
{
    web::http::client::http_client_config client_config;
#ifdef _WIN32
    wchar_t *pValue = nullptr;
    std::unique_ptr<wchar_t, void (*)(wchar_t *)> holder(nullptr, [](wchar_t *p) { free(p); });
    size_t len = 0;
    auto err = _wdupenv_s(&pValue, &len, L"http_proxy");
    if (pValue)
        holder.reset(pValue);
    if (!err && pValue && len)
    {
        std::wstring env_http_proxy_string(pValue, len - 1);
#else
    if (const char *env_http_proxy = std::getenv("http_proxy"))
    {
        std::string env_http_proxy_string(env_http_proxy);
#endif
        if (env_http_proxy_string == U("auto"))
            client_config.set_proxy(web::web_proxy::use_auto_discovery);
        else
            client_config.set_proxy(web::web_proxy(env_http_proxy_string));
    }

    return client_config;
}

int test(int argc, char *args[])
{
    const string_t searchTerm = "Hello";
    const string_t outputFileName = "Hello.txt";

    // Open a stream to the file to write the HTTP response body into.
    auto fileBuffer = std::make_shared<streambuf<uint8_t>>();
    file_buffer<uint8_t>::open(outputFileName, std::ios::out)
        .then([=](streambuf<uint8_t> outFile) -> pplx::task<http_response> {
            *fileBuffer = outFile;

            // Create an HTTP request.
            // Encode the URI query since it could contain special characters like spaces.
            http_client client(U("http://www.bing.com/"), client_config_for_proxy());
            return client.request(methods::GET, uri_builder(U("/search")).append_query(U("q"), searchTerm).to_string());
        })

        // Write the response body into the file buffer.
        .then([=](http_response response) -> pplx::task<size_t> {
            printf("Response status code %u returned.\n", response.status_code());

            return response.body().read_to_end(*fileBuffer);
        })

        // Close the file buffer.
        .then([=](size_t) { return fileBuffer->close(); })

        // Wait for the entire response body to be written into the file.
        .wait();

    return 0;
}

void ListView::on_button_quit()
{
    test(0, NULL);
}