#include <iostream>
#include <vector>
#include "Factory.h"



class Product {
public:
    Product() : id("Product") {}
    std::string id;
};



class Chair : public Product {
public:
    Chair() { id = "Chair"; }
};

class Table : public Product {
public:
    Table() { id = "Table"; }
};

class Sofa : public Product {
public:
    Sofa() { id = "Sofa"; }
};



class WoodenChair : public Chair {
public:
    WoodenChair() { id = "WoodenChair"; }
};

class WoodenTable : public Table {
public:
    WoodenTable() { id = "WoodenTable"; }
};

class WoodenSofa : public Sofa {
public:
    WoodenSofa() { id = "WoodenSofa"; }
};



class MetalChair : public Chair {
public:
    MetalChair() { id = "MetalChair"; }
};

class MetalTable : public Table {
public:
    MetalTable() { id = "MetalTable"; }
};

class MetalSofa : public Sofa {
public:
    MetalSofa() { id = "MetalSofa"; }
};



class MetalRichChair : public MetalChair {
public:
    MetalRichChair() { id = "MetalRichChair"; }
};

class MetalRichTable : public MetalTable {
public:
    MetalRichTable() { id = "MetalRichTable"; }
};

class MetalRichSofa : public MetalSofa {
public:
    MetalRichSofa() { id = "MetalRichSofa"; }
};



class MetalPoorChair : public MetalChair {
public:
    MetalPoorChair() { id = "MetalPoorChair"; }
};

class MetalPoorTable : public MetalTable {
public:
    MetalPoorTable() { id = "MetalPoorTable"; }
};

class MetalPoorSofa : public MetalSofa {
public:
    MetalPoorSofa() { id = "MetalPoorSofa"; }
};



/// Получение фабрики
using MyFactoryHierarchy = GetAbstractFactory< 3, 5,
        TypeList<Chair, Table, Sofa>,
        TypeList<WoodenChair, WoodenTable, WoodenSofa>,
        TypeList<MetalChair, MetalTable, MetalSofa>,
        TypeList<MetalRichChair, MetalRichTable, MetalRichSofa>,
        TypeList<MetalPoorChair, MetalPoorTable, MetalPoorSofa>
>;



int main() {
    std::cout << std::endl;

    /// Metal Poor Factory
    MyFactoryHierarchy::Factory *metalFactory = new typename MyFactoryHierarchy::GetConcreteFactory<MetalPoorChair>::Result;

    Product *chair_1  = metalFactory->Get<Chair>();
    Product *sofa_1   = metalFactory->Get<Sofa>();
    Product *table_1  = metalFactory->Get<Table>();

    std::cout << chair_1->id << std::endl;
    std::cout << sofa_1->id << std::endl;
    std::cout << table_1->id << std::endl;
    std::cout << std::endl;


    /// Wooden Factory
    MyFactoryHierarchy::Factory *woodenFactory = new typename MyFactoryHierarchy::GetConcreteFactory<WoodenSofa>::Result;

    Product *chair_2  = woodenFactory->Get<Chair>();
    Product *sofa_2   = woodenFactory->Get<Sofa>();
    Product *table_2  = woodenFactory->Get<Table>();

    std::cout << chair_2->id << std::endl;
    std::cout << sofa_2->id << std::endl;
    std::cout << table_2->id << std::endl;
    std::cout << std::endl;

}
