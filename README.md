# gtkmm-demo
a demo for gtkmm

# dependencies
    1. sudo apt install libgtkmm-3.0-dev
    2. add the following code to /usr/include/gtkmm-3.0/gtkmm/object.h, refer to                https://github.com/GNOME/gtkmm/blob/4a82e56c2c0883f17f492f6592540582b07d89c3/gtk/gtkmm/object.h
    ```
    template<class T, class... T_Args>
    auto make_managed(T_Args&&... args)
    {
      return manage(new T(std::forward<T_Args>(args)...));
    }
    ```
# build
    make
