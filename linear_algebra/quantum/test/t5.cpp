#include<matrix/matrix.hpp>
using namespace quantum;

int main(void){
    matrix<int> A=(int[][4]){
        {4,2,1,3},
        {15,6,8,9},
        {8,6,7,1}
    };
    A.swap_rows(0,2);
    std::cout<<A<<std::endl<<std::endl;

    A.multiply_row(1,2);
    std::cout<<A<<std::endl<<std::endl;

    A.add_multiple_of_row(0,1,4);
    std::cout<<A<<std::endl<<std::endl;
    
}