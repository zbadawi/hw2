#include <iostream>
#include <fstream>
#include <set>
#include <sstream>
#include <vector>
#include <iomanip>
#include <algorithm>
#include "product.h"
#include "db_parser.h"
#include "product_parser.h"
#include "util.h"
#include "mydatastore.h"
// //for personal debugging - printing keywords
// #include "book.h"
// #include "clothing.h"
// #include "movie.h"


using namespace std;

/*for personal debugging - printing keywrds of a product
ostream& operator<< (ostream& os, const Product &Product) {
    os << "keywords: ";

    set<string> words = Product.keywords();

    for (auto word : words) {
        os << word << " ";
    }

    os << endl;

    return os;
}

//for personal debugging - printing set
void printSet(ostream& os, const set<string>& s) {
    for (const string& item : s) {
        os << item << " ";
    }

    os << endl;
}

//for personal debugging - print products set
void printProdSet (ostream& os, const set<Product*>& s){
    for (auto product : s){
        os << "Product: " << product->getName() << endl;
        os << "Qty: " << product->getQty() << endl;
    }

    os << endl;
}
//for personal debugging - print index map
void printMap(ostream& os, const map<string, set<Product*>>& m) {
    for (const auto& [keyword, products] : m) {
        os << keyword << ": " << endl;
        for (const auto& product : products) {
            os << "  " << product->getName() << ", " << product->getPrice() << std::endl;
        }
    }
}

//for personal debugging - print cart map
void printMap(ostream& os, const map<User*, vector<Product*>>& m) {
    for (const auto& [user, products] : m) {
        os << "User: " << user->getName() << ", " << "Balance: " << user->getBalance() << endl;
        for (const auto& product : products) {
            os << "  " << "Product: " << product->getName() << ", " << "Price: " << product->getPrice() << ", " << "Qty: " << product->getQty() << std::endl;
        }
    }
}

end of personal debugging */

struct ProdNameSorter {
    bool operator()(Product* p1, Product* p2) {
        return (p1->getName() < p2->getName());
    }
};
void displayProducts(vector<Product*>& hits);

int main(int argc, char* argv[])
{
    if(argc < 2) {
        cerr << "Please specify a database file" << endl;
        return 1;
    }

    /****************
     * Declare your derived DataStore object here replacing
     *  DataStore type to your derived type
     ****************/
    MyDS ds;



    // Instantiate the individual section and product parsers we want
    ProductSectionParser* productSectionParser = new ProductSectionParser;
    productSectionParser->addProductParser(new ProductBookParser);
    productSectionParser->addProductParser(new ProductClothingParser);
    productSectionParser->addProductParser(new ProductMovieParser);
    UserSectionParser* userSectionParser = new UserSectionParser;

    // Instantiate the parser
    DBParser parser;
    parser.addSectionParser("products", productSectionParser);
    parser.addSectionParser("users", userSectionParser);

    // Now parse the database to populate the DataStore
    if( parser.parse(argv[1], ds) ) {
        cerr << "Error parsing!" << endl;
        return 1;
    }

    cout << "=====================================" << endl;
    cout << "Menu: " << endl;
    cout << "  AND term term ...                  " << endl;
    cout << "  OR term term ...                   " << endl;
    cout << "  ADD username search_hit_number     " << endl;
    cout << "  VIEWCART username                  " << endl;
    cout << "  BUYCART username                   " << endl;
    cout << "  QUIT new_db_filename               " << endl;
    cout << "====================================" << endl;

    vector<Product*> hits;
    bool done = false;
    while(!done) {
        cout << "\nEnter command: " << endl;
        string line;
        getline(cin,line);
        stringstream ss(line);
        string cmd;
        if((ss >> cmd)) {
            if( cmd == "AND") {
                string term;
                vector<string> terms_;
                while(ss >> term) {
                    term = convToLower(term);
                    terms_.push_back(term);
                }
                hits = ds.search(terms_, 0);
                displayProducts(hits);
            }
            else if ( cmd == "OR" ) {
                string term;
                vector<string> terms_;
                while(ss >> term) {
                    term = convToLower(term);
                    terms_.push_back(term);
                }
                hits = ds.search(terms_, 1);
                displayProducts(hits);
            }
            else if ( cmd == "QUIT") {
                string filename;
                if(ss >> filename) {
                    ofstream ofile(filename.c_str());
                    ds.dump(ofile);
                    ofile.close();
                }
                done = true;
            }
            else if (cmd == "ADD") {
                string uname;
                int hnum;
                ss >> uname >> hnum;
                ds.addToCart(uname, hits, hnum);
            } else if (cmd == "VIEWCART") {
                string uname;
                ss >> uname;
                ds.displayCart(uname);
            } else if (cmd == "BUYCART") {
                string uname;
                ss >> uname;
                ds.buyCart(uname);
            }




            else {
                cout << "Unknown command" << endl;
            }
        }

    }

    return 0;
}

void displayProducts(vector<Product*>& hits)
{
    int resultNo = 1;
    if (hits.begin() == hits.end()) {
    	cout << "No results found!" << endl;
    	return;
    }
    std::sort(hits.begin(), hits.end(), ProdNameSorter());
    for(vector<Product*>::iterator it = hits.begin(); it != hits.end(); ++it) {
        cout << "Hit " << setw(3) << resultNo << endl;
        cout << (*it)->displayString() << endl;
        cout << endl;
        resultNo++;
    }
}
