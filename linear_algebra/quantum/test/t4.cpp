#include<iostream>
#include<matrix/matrix.hpp>
using namespace quantum;

int main(void){
    matrix<int> A=(int[][4]){
        {4,2,1,3},
        {15,6,8,9},
        {8,6,7,1}
    };
    matrix<int> B=(int[][4]){
        {9,1,3,4},
        {5,2,6,1},
        {8,9,5,7}
    };
//     std::cout<<A.size()<<" "<<B.size()<<std::endl;

// // matrix binary ops
//     std::cout<<A+B<<std::endl<<std::endl;
//     std::cout<<A-B<<std::endl<<std::endl;
//     std::cout<<A*B<<std::endl<<std::endl;
//     std::cout<<A/B<<std::endl<<std::endl;

// // matrix binary ops with scalar
//     std::cout<<A+5<<std::endl<<std::endl;
//     std::cout<<5+A<<std::endl<<std::endl;

// // unary operations
//     std::cout<<-B<<std::endl<<std::endl;

//     A*=4;
//     std::cout<<A<<std::endl<<std::endl;
//     A/=4;
//     std::cout<<A<<std::endl<<std::endl;

//     std::cout<<A.t()<<std::endl;

//     std::cout<<A.t().matmul(A)<<std::endl;

}