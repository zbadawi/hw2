#ifndef BOOK_H
#define BOOK_H
#include <iostream>
#include <string>
#include <set>
#include <vector>
#include <algorithm>
#include "product.h"

class Book : public Product {
    public:
    Book (const std::string category, const std::string name, double price, int qty, std::string ISBN, std::string author);
    ~Book();

    //returns set of keywords associated with this book
    std::set<std::string> keywords() const;

    //Displays a string with book info for hits of the search
    std::string displayString() const;
    
    //Outputs the product info in the database format
    void dump(std::ostream& os) const;

    //accessors
    std::string getISBN() const;
    std::string getAuthor() const;

    protected:
    std::string ISBN_;
    std::string author_;
};

#endif