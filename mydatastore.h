#ifndef MYDATASTORE_H
#define MYDATASTORE_H
#include <vector>
#include <iostream>
#include <set>
#include <map>
#include "datastore.h"
#include "user.h"
#include "product.h"
#include "util.h"

class MyDS : public DataStore {
    public:
    MyDS(); 
    ~MyDS();

    /**
     * Adds a product to the data store
     */
    void addProduct(Product* p);

    /**
     * Adds a user to the data store
     */
    void addUser(User* u);

    /**
     * Performs a search of products whose keywords match the given "terms"
     *  type 0 = AND search (intersection of results for each term) while
     *  type 1 = OR search (union of results for each term)
     */
    std::vector<Product*> search(std::vector<std::string>& terms, int type);

    /*holds carts for users*/
    //searches for user by username and for product by hit number
    void cartHelper (std::string username, User*& returnUser, int i, Product*& returnProduct, const std::vector<Product*> h);
    //adds product to carts map
    void addToCart(std::string username, const std::vector<Product*>& hits, int i);
    
    /*displays contents of user's cart*/
    void displayCart (std::string username);

    /*iterates through user's cart and buys available items (in stock and available funds)*/
    void buyCart(std::string username);

    /** 
     * Reproduce the database file from the current Products and User values
     */
    void dump(std::ostream& ofile);

    private:
    //map to hold users
    std::map<std::string, User*> usernames;
    //vector to hold products
    std::set<Product*> products;
    //map with keywords as keys and set of products* as values
    std::map<std::string, std::set<Product*>> indexMap;
    //map with users as keys and set of products* as values; representing user's carts
    std::map<User*, std::vector<Product*>> carts;
};

#endif