#define _quantumdebugmode
#include<vector/vector.hpp>
using namespace quantum;

int main(int argc, char** argv){
    vector<float> u=(float[]){1.0f,5.0f,6.0f,9.0f,8.0f,12.0f,3.0f};
    vector<float> v=(float[]){0.5f,2.0f,3.0f,5.0f,45.0f,2.0f,6.0f};

    // binary operators for vec on vec
    std::cout<<"u + v = "<<u+v<<std::endl;
    std::cout<<"u - v = "<<u-v<<std::endl;
    std::cout<<"u * v = "<<u*v<<std::endl;
    std::cout<<"u / v = "<<u/v<<std::endl;

    // binary operators for vec with scalars
    std::cout<<"u + 5.0f = "<<u+5.0f<<std::endl;
    std::cout<<"u - 5.0f = "<<u-5.0f<<std::endl;
    std::cout<<"u * 5.0f = "<<u*5.0f<<std::endl;
    std::cout<<"u / 5.0f = "<<u/5.0f<<std::endl;

    std::cout<<"8.0f + u = "<<8.0f+u<<std::endl;
    std::cout<<"8.0f - u = "<<8.0f-u<<std::endl;
    std::cout<<"8.0f * u = "<<8.0f*u<<std::endl;


    // unary operators
    std::cout<<"-v = "<<-v<<std::endl;

    // inbuilt unary operators with vectors
    v+=u;
    std::cout<<"after v+=u: "<<v<<std::endl;
    v-=u;
    std::cout<<"after v-=u: "<<v<<std::endl;    // should be back to v
    v*=u;
    std::cout<<"after v*=u: "<<v<<std::endl;    
    v/=u;
    std::cout<<"after v/=u: "<<v<<std::endl;    // should be back to v

    // inbuilt unary operators with scalars
    u+=8.0f;
    std::cout<<"after u+=8.0f: "<<u<<std::endl;
    u-=8.0f;
    std::cout<<"after u-=8.0f: "<<u<<std::endl;
    u*=8.0f;
    std::cout<<"after u*=8.0f: "<<u<<std::endl;
    u/=8.0f;
    std::cout<<"after u/=8.0f: "<<u<<std::endl;


}