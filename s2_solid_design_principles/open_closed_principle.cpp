/*

Open-closed principle:
systems should be open to extensions but closed for modification
so that it avoids to jump to the written code

Open for extention: make and use specification by extending by inheritance
Close for modification: in the context of the specification pattern, we will
never require to go back into the Filter interface or Specification interface,
we will extend them by inheritance to create more specification or combinator.

*/


#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <boost/lexical_cast.hpp>

using namespace std;
using namespace boost;

enum class Color {
    red,
    green,
    blue
};
enum class Size {
    small,
    medium,
    large
};

struct Product {
    string name;
    Color color;
    Size size;
};

/****************************** Before rewrite ******************************/
//struct ProductFilter {
//    vector<Product *> filter_by_color(vector<Product *> items, Color color) {
//        vector<Product *> result;
//        for (auto &i : items) {
//            if (i->color == color) {
//                result.push_back(i);
//            }
//        }
//        return result;
//    }
//
//    vector<Product *> filter_by_size(vector<Product *> items, Size size) {
//        vector<Product *> result;
//        for (auto &i : items) {
//            if (i->size == size) {
//                result.push_back(i);
//            }
//        }
//        return result;
//    }
//
//    vector<Product *> filter_by_color_and_size(vector<Product *> items,
//            Color color, Size size) {
//        vector<Product *> result;
//        for (auto &i : items) {
//            if (i->color == color && i->size == size) {
//                result.push_back(i);
//            }
//        }
//        return result;
//    }
//};
/***************************************************************************/

/****************************** After rewrite ******************************/
// declare struct Specification
template<typename T>
struct Specification;

// Specification And combinator
template<typename T>
struct AndSpecification : Specification<T> {
    Specification<T> &first;
    Specification<T> &second;

    explicit AndSpecification(Specification<T> &first,
            Specification<T> &second) : first(first), second(second) {
    }

    bool is_satisfied(T *item) override {
        return first.is_satisfied(item) && second.is_satisfied(item);
    }
};

template<typename T>
struct Specification {
    virtual bool is_satisfied(T *item) = 0;

    AndSpecification<T> operator&&(Specification<T> &&other) {
        return AndSpecification<T>(*this, other);
    }
};

template<typename T>
struct Filter {
    virtual vector<T *> filter(vector<T *> items, Specification<T> &spec) = 0;
};

struct BetterFilter : Filter<Product> {
    vector<Product *> filter(vector<Product *> items,
            Specification<Product> &spec) override {
        vector<Product *> result;
        for (auto &item : items) {
            if (spec.is_satisfied(item))
                result.push_back(item);
        }
        return result;
    }
};

struct ColorSpecification : Specification<Product> {
    Color color;

    explicit ColorSpecification(const Color color) : color(color) {
    }

    bool is_satisfied(Product *item) override {
        return item->color == color;
    }
};

struct SizeSpecification : Specification<Product> {
    Size size;

    explicit SizeSpecification(const Size size) : size {size} {
    }

    bool is_satisfied(Product *item) override {
        return item->size == size;
    }
};


/***************************************************************************/

int main() {
    Product apple {"Apple", Color::green, Size::small};
    Product tree {"Tree", Color::green, Size::large};
    Product house {"House", Color::blue, Size::large};

    vector<Product *> items {&apple, &tree, &house};

    // before rewrite
//    ProductFilter pf;
//    auto green_things = pf.filter_by_color(items, Color::green);
//    for (auto &item : green_things)
//        cout << item->name << " is green\n";

    // after rewrite
    // use ColorSpecification
    BetterFilter bf;
    ColorSpecification green(Color::green);
    for (auto &item: bf.filter(items, green))
        cout << item->name << " is green\n";

    // use Specification And combinator
    SizeSpecification large(Size::large);
    AndSpecification<Product> green_and_large(green, large);
    for (auto &item: bf.filter(items, green_and_large))
        cout << item->name << " is green and large\n";

    // use Specification && operator overload
    auto spec = ColorSpecification(Color::green)
                && SizeSpecification(Size::large);
    for (auto &item: bf.filter(items, spec))
        cout << item->name << " is green and large\n";

    return 0;
}