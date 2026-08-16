extern "C" {
    #include <text.h>
    #include <mgtt.h>
}

class Hello {
    public:
    int x;
    int y;
    Hello(int X = 4, int Y = 5);
    ~Hello();
};

Hello::Hello(int X, int Y) : x(X), y(Y) {
}

extern int bar;
Hello X{bar};



void foo() {

    Hello x{};
    x.x = 2;
    x.y = 3;

    Hello* h = new Hello;
    delete h;
}



Hello::~Hello() {
    TextManager();
}