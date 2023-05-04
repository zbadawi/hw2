#include <sstream>
#include <iomanip>
#include "clothing.h"
#include "util.h"
#include "product.h"

using namespace std;

Clothing :: Clothing (const string category, const string name, double price, int qty, string size, string brand) :
Product(category, name, price, qty), size_(size), brand_(brand)
{

}

Clothing :: ~Clothing() {
    //IMPLEMENT??
}

set<string> Clothing :: keywords() const {
    set<string> keywords;
    set<string> name = parseStringToWords(this->name_);
    set<string> brand = parseStringToWords (this->brand_);

    set_union(name.begin(), name.end(), brand.begin(), brand.end(), inserter(keywords, keywords.begin()));

    return keywords;
}

string Clothing :: displayString() const {
    string final = name_ + " " + size_ + " " + brand_ ;
    return final;
}

void Clothing :: dump(std::ostream& os) const {
    Product::dump(os);
    os << size_ << "\n" << brand_ << endl;
}

string Clothing :: getSize() const {
    return size_;
}

string Clothing :: getBrand() const {
    return brand_;
}