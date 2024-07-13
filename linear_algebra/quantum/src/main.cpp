#define _matrixdebugmode
#include<matrix/matrix.hpp>
#include<algoithms/guass_elimination.hpp>
#include<iomanip>

int main(int argc, char** argv){
    quantum::matrix<float> A=(float[][4]){
        {1.0, 1.0, 1.0, 3.0},
        {1.0, 2.0, 2.0, 5.0},
        {3.0, 4.0, 4.0, 12.0}
    };
    // all the output is gonna be purple
    std::cout<<"\e[1;35m";
    std::cout<<std::fixed;
    std::cout<<std::setprecision(4)<<A<<std::endl;

    std::cout<<std::endl;

    // guass_elimination(A);
    // std::cout<<std::fixed;
    // std::cout<<std::setprecision(4)<<A<<std::endl;

    // std::cout<<std::endl;

    rref(A);
    std::cout<<std::fixed;
    std::cout<<std::setprecision(4)<<A<<std::endl;
    std::cout<<"\e[0m";
}
