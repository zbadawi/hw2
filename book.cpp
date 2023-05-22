#include <sstream>
#include <iomanip>
#include "book.h"
#include "util.h"
#include "product.h"

using namespace std;

Book :: Book (const string category, const string name, double price, int qty, string ISBN, string author) :
Product(category, name, price, qty), ISBN_(ISBN), author_(author)

{

}

Book :: ~Book() {
    //IMPLEMENT
}

set<string> Book :: keywords() const {
    set<string> keywords;
    set<string> name = parseStringToWords(this->name_);
    set<string> author = parseStringToWords (this->author_);

    set_union(name.begin(), name.end(), author.begin(), author.end(), inserter(keywords, keywords.begin()));//combines sets into one

    keywords.insert(ISBN_); //insert ISBN as is

    return keywords;
}

string Book :: displayString() const {
    stringstream ss;
    ss << name_ << "\n" << "Author: " << author_ << " ISBN: " << ISBN_ << "\n" << price_ << " " << qty_ << " left." << endl;
    string final = ss.str();
    return final;
}

void Book :: dump(std::ostream& os) const {
    Product::dump(os);
    os << ISBN_ << "\n" << author_ << endl;
}

string Book :: getISBN() const {
    return ISBN_;
}

string Book :: getAuthor() const {
    return author_;
}