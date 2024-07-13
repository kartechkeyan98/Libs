#define _quantumdebugmode
#include<vector/vector.hpp>
using namespace quantum;

// mainly testing the constructors.....
int main(int argc, char** argv){
#if __cplusplus >= 201103L
    printf("Working with -std>=c++11, you may use std::initializer_list\n");
#else 
    printf("Working with -std<c++11, don't use std::intializer_list\n");
#endif
    vector<int>v=(int[]){1,5,9,3,6};
    vector<int>u(v); u[0]=2;
    vector<int>w=v; w[0]=3;

    std::cout<<u.data()<<": "<<u<<std::endl;
    std::cout<<v.data()<<": "<<v<<std::endl;
    std::cout<<w.data()<<": "<<w<<std::endl;

#if __cplusplus >= 201103L
    vector<float> x={0.25f,0.14f,0.8f,0.6f};
    std::cout<<x.data()<<": "<<x<<std::endl;
#endif

    w=(int[]){5,6,9,7};
    std::cout<<w.data()<<": "<<w<<std::endl;

#if __cplusplus >= 201103L
    u={6,5,0,8,7,9,6};
    std::cout<<u.data()<<": "<<u<<std::endl;
#endif
}