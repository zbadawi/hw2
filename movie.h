#ifndef MOVIE_H
#define MOVIE_H
#include <iostream>
#include <string>
#include <set>
#include <vector>
#include <algorithm>
#include "product.h"

class Movie : public Product {
    public:
    Movie (const std::string category, const std::string name, double price, int qty, std::string genre, std::string rating);
    ~Movie();

    //returns set of keywords associated with this movie
    std::set<std::string> keywords() const;

    //Displays a string with movie info for hits of the search
    std::string displayString() const;
    
    //Outputs the product info in the database format
    void dump(std::ostream& os) const;

    //accessors
    std::string getGenre() const;
    std::string getRating() const;

    protected:
    std::string genre_;
    std::string rating_;
};

#endif