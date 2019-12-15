PROG = demo
CXX = clang++
CPPFLAGS = -g -std=c++17 `pkg-config gtkmm-3.0 --cflags` -Wdeprecated-declarations
LDFLAGS = `pkg-config gtkmm-3.0 --libs` -lcpprest -lboost_system -lcrypto
OBJS = main.o navigation.o list_view.o

$(PROG) : $(OBJS)
	$(CXX) $(CPPFLAGS) $(LDFLAGS) $(OBJS) -o $(PROG)

main.o : src/main.cpp
	$(CXX) $(CPPFLAGS) -c src/main.cpp

navigation.o : src/navigation.h src/navigation.cpp
	$(CXX) $(CPPFLAGS) -c src/navigation.cpp

list_view.o : src/list_view.h src/list_view.cpp
	$(CXX) $(CPPFLAGS) -c src/list_view.cpp

release :
	$(CXX) -O2 -std=c++17 `pkg-config gtkmm-3.0 --cflags --libs` -lcpprest -lboost_system -lcrypto \
	src/main.cpp src/navigation.cpp src/list_view.cpp -o $(PROG) 

gcc :
	g++ -std=c++17 src/main.cpp src/navigation.cpp src/list_view.cpp -o demo `pkg-config gtkmm-3.0 --cflags --libs` -lcpprest -lboost_system -lcrypto

clean :
	rm -f $(PROG) $(OBJS) Hello.txt