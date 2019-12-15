PROG = demo
CXX = clang++
CPPFLAGS = -c -g -std=c++17 -std=libc++ pkg-config gtkmm-3.0 --cflags` 
LDFLAGS = `pkg-config gtkmm-3.0 --libs` -lcpprest -lboost_system -lcrypto -lpthread
OBJS = main.o navigation.o list_view.o

$(PROG) : $(OBJS)
	$(CXX) $(LDFLAGS) $(OBJS) -o $(PROG)

main.o : src/main.cpp
	$(CXX) $(CPPFLAGS) src/main.cpp

navigation.o : src/navigation.h src/navigation.cpp
	$(CXX) $(CPPFLAGS) src/navigation.cpp

list_view.o : src/list_view.h src/list_view.cpp
	$(CXX) $(CPPFLAGS) src/list_view.cpp

release :
	$(CXX) -O2 -std=c++17 `pkg-config gtkmm-3.0 --cflags --libs` -lcpprest -lboost_system -lcrypto \
	src/main.cpp src/navigation.cpp src/list_view.cpp -o $(PROG) 

gcc :
	g++ src/main.cpp src/navigation.cpp src/list_view.cpp -o demo `pkg-config gtkmm-3.0 --cflags --libs`

clean :
	rm -f $(PROG) $(OBJS) Hello.txt
