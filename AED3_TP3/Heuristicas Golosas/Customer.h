
#ifndef CLARK_WRIGHT_CUSTOMER_H
#define CLARK_WRIGHT_CUSTOMER_H

//Conviene clase o Struct?
class Customer {
private:
    unsigned int ID;
    unsigned int demand;
    double xCoord, yCoord;
    bool visited;
public:
    Customer(unsigned int id);
    Customer(unsigned int id, unsigned int dem, double x, double y);
    void setx(double a);
    void sety(double b);
    void setDemand(unsigned int a);
    void setID(unsigned int id);
    void isVisited();

    double getx();
    double gety();
    unsigned int getDemand();
    unsigned int getID();
    bool getstatus();
    bool operator==(Customer a);
    bool isDepot();
    void unvisit();
};


#endif //CLARK_WRIGHT_CUSTOMER_H
