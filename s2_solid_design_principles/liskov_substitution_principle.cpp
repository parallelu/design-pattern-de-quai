/*

Liskov Substitution Principle:
Specify that subtypes should immediately substitutable for their base types.


*/

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <boost/lexical_cast.hpp>

using namespace std;
using namespace boost;

/****************************** Before rewrite ******************************/
class Rectangle {
protected:
    int width;
    int height;
public:
    explicit Rectangle(int width, int height) : width(width), height(height) {
    }

    int getWidth() const {
        return width;
    }

    virtual void setWidth(int width) {
        Rectangle::width = width;
    }

    int getHeight() const {
        return height;
    }

    virtual void setHeight(int height) {
        Rectangle::height = height;
    }

    int area() const {
        return width * height;
    }
};

class Square : public Rectangle {
public:
    explicit Square(int size) : Rectangle(size, size) {
    }

    virtual void setWidth(int width) override {
        this->width = this->height = width;
    }

    virtual void setHeight(int height) override {
        this->width = this->height = height;
    }

};

/***************************************************************************/

/****************************** After rewrite ******************************/
/*
 * Solution to fix the issue:
 * 1. add a boolean flag in Rectangle to indicate whether it is a Rectangle or not
 * 2. make a factory (shown below)
 */
struct RectangleFactory {
    static Rectangle create_rectangle(int w, int h) {
        return Rectangle(w, h);
    }

    static Rectangle create_square(int size) {
        return Square(size);
    }
};

/***************************************************************************/

void process(Rectangle &r) {
    int w = r.getWidth();
    r.setHeight(10);    // we break the principle here if r is a Square obj

    cout << "expected area = " << (w * 10)
         << ", got " << r.area() << endl;
}

int main() {
    /**************************** Before rewrite ****************************/
    Rectangle r {3, 4};
    process(r);

    Square sq {5};
    process(sq);

    /***************************** After rewrite ****************************/
    Rectangle rr = RectangleFactory::create_rectangle(3, 4);
    Rectangle sqq = RectangleFactory::create_square(5);
    process(rr);
    process(sqq);

    return 0;
}
