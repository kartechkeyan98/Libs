// std
#include<iostream>

// la
#include<qbvector.h>
using namespace qb;



int main(void){
    std::cout<<"sizeof vector<float>: "<<sizeof(qb::vector<float>)<<"B"<<std::endl;
    std::cout<<"sizeof vector<int>: "<<sizeof(qb::vector<int>)<<"B"<<std::endl;
    std::cout<<"sizeof vector<double>: "<<sizeof(qb::vector<double>)<<"B"<<std::endl;
    qb::vector<float> a1;
    qb::vector<int> a2(5);

    int a[5]={1,2,3,4,5};
    qb::vector<int> a3(5, a);
    qb::vector<float> a4=(float[]){1,2,3,4,5,5};
    qb::vector<float> a5(1,2,3);
    qb::vector<float> a6(a5);

    printinfo(std::cout,a1);
    printinfo(std::cout,a2);
    printinfo(std::cout,a3);
    printinfo(std::cout,a4);
    printinfo(std::cout,a5);
    printinfo(std::cout,a6);

    
}