/*

Dependency Inversion Principle:
Specify the best way to form dependencies between different objects

Two ideas:
 A. High-level modules should not depend on low-level modules. Both should
    depend on abstractions. (as generic as possible)
 B. Abstractions should not depend on details. Details should depend on abstractions.

Comment:
Dependency Injection Principle:
Specify a certain defaults.

*/

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <boost/lexical_cast.hpp>

using namespace std;
using namespace boost;

enum class Relationship {
    parent,
    child,
    sibling
};

struct Person {
    string name;

};

/****************************** Before rewrite ******************************/
/**
 * Problem:
 * If low-level module changes its storage of relations or makes relations private
 * the high-level code completely breaks.
 */
// store relationship between persons
struct Relationships {  // low-level module
    vector<tuple<Person, Relationship, Person>> relations;

    void add_parent_and_child(const Person &parent, const Person &child) {
        relations.push_back({parent, Relationship::parent, child});
        relations.push_back({child, Relationship::child, parent});
    }
};

struct Research {   // high-level module
    Research(Relationships &relationships) {
        // high-level module depends on details of low-level module
        auto &relations = relationships.relations;
        for (auto &&[first, rel, second] : relations) {     // c++17
            if (first.name == "John" && rel == Relationship::parent) {
                cout << "John has a child called " << second.name << endl;
            }
        }
    }
};

/***************************************************************************/

/****************************** After rewrite ******************************/
/**
 * Solution:
 * idea 1. move some code to low-level module
 * idea 2. introduce abstraction to fix the issue
 */
// interface
struct RelationshipBrowser {
    virtual vector<Person> find_all_children_of(const string &name) = 0;
};

struct RelationshipsOO : RelationshipBrowser {  // low-level (fixed)
    vector<tuple<Person, Relationship, Person>> relations;

    void add_parent_and_child(const Person &parent, const Person &child) {
        relations.push_back({parent, Relationship::parent, child});
        relations.push_back({child, Relationship::child, parent});
    }

    vector<Person> find_all_children_of(const string &name) override {
        vector<Person> result;
        for (auto &&[first, rel, second] : relations) {
            if (first.name == name && rel == Relationship::parent) {
                result.push_back(second);
            }
        }
        return result;
    }
};

struct ResearchOO {     // high-level (fixed)
    ResearchOO(RelationshipBrowser &browser) {
        for (auto &child : browser.find_all_children_of("John")) {
            cout << "John has a child called " << child.name << endl;
        }
    }
};

/***************************************************************************/


int main() {
    Person parent {"John"};
    Person child1 {"Chris"}, child2 {"Matt"};

    /**************************** Before rewrite ****************************/
    Relationships relationships;
    relationships.add_parent_and_child(parent, child1);
    relationships.add_parent_and_child(parent, child2);

    Research _(relationships);

    /***************************** After rewrite ****************************/
    RelationshipsOO relationshipsOO;
    relationshipsOO.add_parent_and_child(parent, child1);
    relationshipsOO.add_parent_and_child(parent, child2);

    ResearchOO _OO(relationshipsOO);

    return 0;
}
