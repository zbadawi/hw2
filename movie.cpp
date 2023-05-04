#include <sstream>
#include <iomanip>
#include "movie.h"
#include "util.h"
#include "product.h"

using namespace std;

Movie :: Movie (const string category, const string name, double price, int qty, string genre, string rating) :
Product(category, name, price, qty), genre_(genre), rating_(rating)
{

}

Movie :: ~Movie() {
    //IMPLEMENT??
}

set<string> Movie :: keywords() const {
    set<string> keywords;
    set<string> name = parseStringToWords (this->name_);
    set<string> genre = parseStringToWords(this->genre_);
    
    set_union(name.begin(), name.end(), genre.begin(), genre.end(), inserter(keywords, keywords.begin()));

    return keywords;
}

string Movie :: displayString() const {
    string final = name_ + " " + genre_ + " " + rating_ ;
    return final;
}

void Movie :: dump(std::ostream& os) const {
    Product::dump(os);
    os << genre_ << "\n" << rating_ << endl;
}

string Movie :: getGenre() const {
    return genre_;
}

string Movie :: getRating() const {
    return rating_;
}