/*

Principle:
Separation of concerns

*/

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <boost/lexical_cast.hpp>

using namespace std;
using namespace boost;

/**
 * Journal should only have to be responsible for its own title and entires
 * and it should give the save function to a separate class PersistenceManager
 */
struct Journal {
public:
    string title;
    vector<string> entries;

    explicit Journal(string title) : title {std::move(title)} {}

    void add_entry(const string &entry) {
        static int count = 1;
        entries.push_back(lexical_cast<string>(count++) + ": " + entry);
    }

    // the reason to move `save` to another struct PersistenceManager is that
    // we don't want to add persistence code to struct Journal
//    void save(const string &filename) {
//        ofstream ofs(filename);
//        for (auto &e : entries) {
//            ofs << e << endl;
//        }
//    }
};

// PersistenceManager stores the persistence code with repect to Journal
struct PersistenceManager {
    static void save(const Journal &j, const string &filename) {
        ofstream ofs(filename);
        for (auto &e : j.entries) {
            ofs << e << endl;
        }
    }
};


int main() {
    Journal journal {"Dear Diary"};
    journal.add_entry("I ate a bug");
    journal.add_entry("I cried today");

//    journal.save("diary.txt");
    PersistenceManager pm;
    pm.save(journal, "diary.txt");

    return 0;
}
