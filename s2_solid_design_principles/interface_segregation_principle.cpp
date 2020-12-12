/*

Interface Segregation Principle:
don't create interfaces which are too large and which require the implementation
to implement too much.

*/

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <boost/lexical_cast.hpp>

using namespace std;
using namespace boost;

struct Document;

/****************************** Before rewrite ******************************/
/**
 * Problem:
 * MultiFunctionalPrinter and Scanner have to implement 3 virtual functions to
 * not be abstract. However, whatever code inside, the client code can send 3
 * requests to them which isn't true.
 */
struct IMachine {
    virtual void print(Document &doc) = 0;

    virtual void scan(Document &doc) = 0;

    virtual void fax(Document &doc) = 0;
};

struct MultiFunctionalPrinter : IMachine {
    virtual void print(Document &doc) override {
        // (implementation)
    }

    virtual void scan(Document &doc) override {
        // what should we put here ???
    }

    virtual void fax(Document &doc) override {
        // what should we put here ???
    }
};

struct Scanner : IMachine {
    void print(Document &doc) override {
        // what should we put here ???
    }

    void scan(Document &doc) override {
        // (implementation)
    }

    void fax(Document &doc) override {
        // what should we put here ???
    }
};

/***************************************************************************/

/****************************** After rewrite ******************************/
/**
 * Solution:
 * Segregate the interface and make each interface small.
 */
struct IPrinter {
    virtual void print(Document &doc) = 0;

};

struct IScanner {
    virtual void scan(Document &doc) = 0;
};

struct IFaxMachine {
    virtual void scan(Document &doc) = 0;
};

struct Printer : IPrinter {
    void print(Document &doc) override {
        // (implementation)
    }
};

// new interface
struct IPSMachine : IPrinter, IScanner {
};

struct SScanner : IScanner {
    void scan(Document &doc) override {

    }
};

struct Machine : IPSMachine {
    IPrinter &printer;
    IScanner &scanner;

    Machine(IPrinter &printer, IScanner &scanner) : printer(printer),
            scanner(scanner) {}

    void print(Document &doc) override {
        printer.print(doc);
    }

    void scan(Document &doc) override {
        scanner.scan(doc);
    }
};

/***************************************************************************/


int main() {

    return 0;
}
