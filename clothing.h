#ifndef CLOTHING_H
#define CLOTHING_H
#include <iostream>
#include <string>
#include <set>
#include "product.h"

class Clothing : public Product {
    public:
    Clothing (const std::string category, const std::string name, double price, int qty, std::string size, std::string brand);
    ~Clothing();

    //returns set of keywords associated with this clothing item
    std::set<std::string> keywords() const;

    //Displays a string with clothnig info for hits of the search
    std::string displayString() const;
    
    //Outputs the product info in the database format
    void dump(std::ostream& os) const;

    //accessors
    std::string getSize() const;
    std::string getBrand() const;

    //for debugging - printing keywords
    // friend ostream& operator<< (ostream& ostream, const Clothing &Clothing);

    protected:
    std::string size_;
    std::string brand_;
};

#endif