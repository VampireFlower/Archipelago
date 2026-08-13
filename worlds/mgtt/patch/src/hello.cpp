class Hello {
    public:
    int x;
    int y;
    Hello();
};

Hello::Hello() {
    this->x = 4;
    this->y = 5;
}

void foo() {

    Hello x;
    x.x = 2;
    x.y = 3;
}