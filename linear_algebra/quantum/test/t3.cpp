#define _matrixdebugmode
#include<matrix/matrix.hpp>
using namespace quantum;

int main(void){
    // testing the constructors
    matrix<float> A;
    A=matrix<float>(1,2);
    A=matrix<float>(1,2,0.0f);
    std::cout<<A<<std::endl;

    A=(float[][4]){
        {0.5f,0.8f,0.9f,0.2f},
        {1.0f,1.2f,1.8f,3.6f},
        {0.5f,0.8f,0.4f,0.6f}
    };

    matrix<float> B=(float[][4]){
        {0.4f,0.6f,1.0f,0.7f},
        {1.4f,1.9f,1.2f,3.9f},
        {0.1f,0.3f,0.4f,0.3f}
    };

    std::cout<<A<<std::endl;
    std::cout<<std::endl;
    std::cout<<B<<std::endl;

}