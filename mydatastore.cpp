#include "mydatastore.h"

using namespace std;

MyDS::MyDS() {

}

MyDS :: ~MyDS() { //IMPLEMENT: doesn't work right
    /* deleting usernames */
    map<string, User*>::iterator itu; //iterator for usernames
    for (itu = usernames.begin(); itu != usernames.end(); ++itu){
        delete itu->second;
    }
    usernames.clear();
    /*deleted usernames*/

    /*deleting products*/
    set<Product*>::iterator itp; //iterator for products
    for (itp = products.begin(); itp != products.end(); ++itp){
        delete *itp;
    }
    products.clear();
    /*deleted products*/

    /*deleting indexMap*/
    map<string, set<Product*>>::iterator itm; //iterator for indexMap
    for (itm = indexMap.begin(); itm!= indexMap.end(); ++itm){
        set<Product*>& productSet = itm->second;
        set<Product*>::iterator itps; //iterator for products set in indexMap
        for (itps = productSet.begin(); itps != productSet.end(); ++itps){
            delete *itps;
        }
        productSet.clear();
    }
    indexMap.clear();
    /*deleted indexMap*/

    /*deleting carts*/
    map<User*, vector<Product*>>::iterator itc; //iterator for carts
    for (itc = carts.begin(); itc != carts.end(); ++itc){
        vector<Product*>& productVector = itc->second;
        vector<Product*>::iterator itv; //iterator for vector within carts
        for (itv = productVector.begin(); itv != productVector.end(); ++itv){
            delete *itv;
        }
        productVector.clear();
    }
    carts.clear();
    /*deleted carts*/
}

void MyDS :: addProduct(Product* p) {
    //adds to products set
    products.insert(p);

    //indexing product
    set<string> toIndex = p->keywords();
    set<string> :: iterator it; 

    for(it = toIndex.begin(); it != toIndex.end(); ++it) {
        string indexTerm = convToLower(*it);
        //result is a pair type holding an iterator to the map and a bool to check if the insertion was successful
        //it is successful the first time the keyword is encountered. key-value pair instantiated and set with p is created.
        //if not successful (keyword already added, an iterator)
        pair<map<string, set<Product*>>::iterator, bool> result = indexMap.emplace(indexTerm, set<Product*>{p}); //returns iterator to key-value pair and a boolean that checks success of insertion
        if (!result.second) { //checks boolean, 1 if insertion was successful, 0 if not (if keyword is already in the map)
            result.first->second.insert(p); //inserts p into existing key-value pair
        }
    }
    //for personal debugging - announcing we're done
    // cout << "done adding product" << endl;
}

void MyDS :: addUser(User* u) {
    //adds users to username map
    string userName = u->getName();
    if (usernames.find(userName) == usernames.end()){ //only adds if user doesn't already exist
        //adds user to map with usernames as keys
        usernames[userName] = u;
    }

    //creating cart for user
    if (carts.find(u) == carts.end()) { //only adds if user doesn't already exist
        //adds user to map with empty products vector
        carts[u] = vector<Product*>();
    }
    //for personal debugging - announcing we're done
    // cout << "done adding user" << endl;
}

/**
     * Performs a search of products whose keywords match the given "terms"
     *  type 0 = AND search (intersection of results for each term) while
     *  type 1 = OR search (union of results for each term)
     */


vector<Product*> MyDS :: search(vector<string>& terms, int type) {
    vector<Product*> result; //to return
    set<Product*> resultSet; //to use with setintersection and union functions
    set<Product*> potentials; //will hold the product vectors we find at each term match
    
    bool productsFound = true; //turns false when there are no search results
    set<Product*> temp;


    //AND search
    if (type == 0){
        for (int i = 0; i < terms.size(); i++){
            string searchTerm = convToLower(terms[i]);
            map<string, set<Product*>> :: iterator itm = indexMap.find(searchTerm); //iterator for index map
            temp = potentials; //holds previous set of potential search results
            if(itm != indexMap.end()) { //search term found in index map
                potentials = itm->second; //copies vector of products that have the keywords
            } else {
                productsFound = false;
                break;
            }

            if (terms.size() == 1){ //case with only one search term
                resultSet = potentials;
            }

            if (!temp.empty()){ //past the first iteration, ready to compare sets of results
                resultSet = setIntersection(temp, potentials);
            }
        }

        //OR search
    } else if (type == 1) {
        for (int i = 0; i < terms.size(); i++){
            string searchTerm = convToLower(terms[i]);
            map<string, set<Product*>> :: iterator itm = indexMap.find(searchTerm); //iterator for index map
            temp = resultSet; //holds previous set of potential search results
            if(itm != indexMap.end()) { //search term found in index map
                potentials = itm->second; //copies vector of products that have the keywords
                if (i == 0){ //handles case for one search term / sets up resultset for other iterations
                    resultSet = potentials;
                }
            } else {
                productsFound = false;
            }

            if (!temp.empty() || !productsFound){ //past the first iteration, ready to combine sets of results (or the first search term was not found)
                resultSet = setUnion(temp, potentials);
            }
        }
    }

    if (!resultSet.empty()){
        for (set<Product*>::iterator it = resultSet.begin(); it != resultSet.end(); ++it) {
            result.push_back(*it);
        }
    }

    return result;
}


void MyDS :: cartHelper (string username, User*& returnUser, int i, Product*& returnProduct, const std::vector<Product*> h){
    map<string, User*>:: iterator it = usernames.find(username); //finds user in map
    if (it != usernames.end()){
        returnUser = it->second; //sets user reference for addToCart function
    } else {
        cerr << "Invalid username" << endl;
    }

    if (i <= h.size()) { //checks if hit number is within bounds
        returnProduct = h[i-1]; //sets product reference for addToCart function
    } else {
        cerr << "Invalid request" << endl;
    }
    
}


void MyDS :: addToCart(string username, const vector<Product*>& h, int i) {
    if (i>0 && !username.empty()){
        User* u; //to hold user found from username search in user map
        Product* pAdd; //to hold product found from hits
        cartHelper(username, u, i, pAdd, h);
        map<User*, vector<Product*>>:: iterator itm = carts.find(u); //find user's cart
        if (itm != carts.end()){
            itm->second.push_back(pAdd); //add product to respective cart
        }

        //for personal debugging
        // cout << "done adding to cart!" << endl;
    } else {
        cerr << "Invalid request" <<endl;
    }
}


void MyDS :: displayCart (string username){
    map<string, User*>:: iterator it = usernames.find(username); //find user*
    User* uDis; //to hold user*
    if (it != usernames.end()){
        uDis = it->second; //user*
        map<User*, vector<Product*>>:: iterator itm = carts.find(uDis); //find cart belonging to user*
        vector<Product*> cDis; //to hold cart
        if (itm != carts.end()){
            cDis = itm->second;
        }

        for (int i=0; i<cDis.size(); i++){
            string output = cDis[i]->displayString();
            cout << "|" << i+1 << "|" << endl << output << endl;
        }
    } else {
        cerr << "Invalid username" << endl;
    }
}


void MyDS :: buyCart(string username) {
    map<string, User*>:: iterator it = usernames.find(username); //finds user in map
    if (it != usernames.end()){
        User* u; //to hold found user
        u = it->second; //user*

        map<User*, vector<Product*>>:: iterator itm = carts.find(u); //find cart belonging to user*
        if (itm != carts.end()){
            vector<Product*>& uCart = itm->second; //to hold cart
            
            vector<Product*>::iterator itv; //iterator to iterate through user's cart
            for (itv = uCart.begin(); itv != uCart.end();){ //iterate through cart
                int stock = (*itv)->getQty(); //product's stock
                double price = (*itv)->getPrice(); //product's price
                double balance = u->getBalance(); //user's available balance

                if (stock>0 && (balance-price)>=0){ //only if stock is at least 1 and user can afford it
                    // //for personal debugging - printing out products as we buy them
                    // cout << "SUCCESSFULLY buying: " << (*itv)->getName() << ", STOCK: " << stock << ", BALANCE: " << balance << endl;
                    (*itv)->subtractQty(1); //subtract 1 from stock
                    // cout << "STOCK now: " << (*itv)->getQty() << endl;
                    u->deductAmount(price); //subtract price from balance
                    // cout << "BALANCE now: " << u->getBalance() << endl;
                    itv = uCart.erase(itv); //delete from vector and passes pointer to next element
                } else {
                    ++itv; //move onto next product
                }
            }
        } else {
            cerr << "No cart found for username entered" << endl; //personal debugging
        }
    } else {
        cerr << "Invalid username" << endl;
    }
}

void MyDS :: dump(ostream& ofile) {
    ofile << "<products>\n";
    set<Product*>::iterator it = products.begin();

    for (it; it!=products.end(); ++it){
        (*it)->dump(ofile);
    }

    ofile << "</products>\n";

    ofile << "<users> \n";
    map<string, User*>::iterator itm = usernames.begin();

    for (itm; itm != usernames.end(); ++itm) {
        itm->second->dump(ofile);
    }

    ofile << "</users>" << endl;
}