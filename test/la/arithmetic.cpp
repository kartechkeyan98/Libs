#include<iostream>
using namespace std;

#include<qbvector.h>
using namespace qb;

int main(void){
    qb::vector<float> v1(1.0f,1.5f,1.2f,5.3f);
    printinfo(std::cout,v1);

    qb::vector<float> v2(1.0f,1.8f,1.0f,4.3f);
    printinfo(std::cout,v2);

    std::cout<<std::fixed<<std::setprecision(4);

    // vector-vector
    std::cout<<v1+v2<<std::endl;
    std::cout<<v1-v2<<std::endl;
    std::cout<<v1*v2<<std::endl;
    std::cout<<v1/v2<<std::endl;
    std::cout<<std::endl;

    // vector-scalar
    std::cout<<v1+5<<std::endl;
    std::cout<<v1-5<<std::endl;
    std::cout<<v1*5<<std::endl;
    std::cout<<v1/5<<std::endl;
    std::cout<<std::endl;

    // scalar-vector
    std::cout<<6+v1<<std::endl;
    std::cout<<6-v1<<std::endl;
    std::cout<<6*v1<<std::endl;
    std::cout<<std::endl;
    
    // unary -
    std::cout<<-v2<<std::endl;
}