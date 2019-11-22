CXX = clang++

demo : clean
	$(CXX) -g src/main.cpp src/navigation.cpp src/list_view.cpp -o demo `pkg-config gtkmm-3.0 --cflags --libs`

release :
	$(CXX) src/main.cpp src/navigation.cpp src/list_view.cpp -o demo `pkg-config gtkmm-3.0 --cflags --libs`

gcc :
	g++ src/main.cpp src/navigation.cpp src/list_view.cpp -o demo `pkg-config gtkmm-3.0 --cflags --libs`

clean :
	rm -f demo