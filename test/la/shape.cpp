#include<iostream>
#include<vector>
#include<initializer_list>
using namespace std;

#include<qbshape.h>
using namespace qb;

int main(void){
/**
 * Constructor testing section
 */ 
    // empty
    shape s1;
    std::cout<<s1<<std::endl;
    printinfo(std::cout,s1);
    // varaidic
    shape s2(1,2,4,5,6,5);
    std::cout<<s2<<std::endl;
    printinfo(std::cout,s2);
    // init. list
    shape s3={4,8,9};
    std::cout<<s3<<std::endl;
    printinfo(std::cout,s3);
    //vector
    shape s4(std::vector<int>({1,4,5,6}));
    std::cout<<s4<<std::endl;
    printinfo(std::cout,s4);
    //c-style
    shape s5=(int[]){4,5,6,7};
    std::cout<<s5<<std::endl;
    printinfo(std::cout,s5);
    // copy
    shape s6(s2);
    std::cout<<s6<<std::endl;
    printinfo(std::cout,s6);

    std::cout<<"Constructors and accessors are a GO!\n\n"<<std::endl;

    /* assignment operator(s) */
    s1 = s2;
    std::cout<<s1<<std::endl;
    printinfo(std::cout,s1);

    s2 = {1,2,3,4,5,7};
    std::cout<<s2<<std::endl;
    printinfo(std::cout,s2);

    s4 = std::vector<int>({1,2,3});
    std::cout<<s4<<std::endl;
    printinfo(std::cout,s4);

    s6 = (int[]){1,2,4};
    std::cout<<s6<<std::endl;
    printinfo(std::cout,s6);

    std::cout<<"Assignment operators are a GO!\n\n"<<std::endl;


/* Comparison */
    std::cout<<"s1 == s2: "<<(s1==s2?"true":"false")<<std::endl;
    std::cout<<"s3 == s4: "<<(s3==s4?"true":"false")<<std::endl;
    std::cout<<"(1,2,3) == (1,2,3): "<<(shape(1,2,3)==shape(1,2,3)?"true":"false")<<std::endl;
    std::cout<<"s3 != s3: "<<(s3!=s3?"true":"false")<<std::endl;

    std::cout<<"\nComparison working just fine!"<<std::endl;

}