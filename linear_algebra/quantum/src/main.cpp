#define _matrixdebugmode
#include<matrix/matrix.hpp>
#include<algoithms/guass_elimination.hpp>
#include<iomanip>

int main(int argc, char** argv){
    // input blue
    std::cout<<"\e[1;36m";
    size_t m,n;                 // no. of equations and no.of variables
    std::cin>>m>>n;

    std::cout<<"\e[1;37m";
    quantum::matrix<float> A(m,n+1);
    for(size_t i=0;i<m;++i)for(size_t j=0;j<n+1;++j)std::cin>>A(i,j);
    // last column is for the constant terms
        
    // all the output is gonna be purple
    std::cout<<"\e[1;35m\n";
    

    // guass_elimination(A);
    // std::cout<<std::fixed;
    // std::cout<<std::setprecision(4)<<A<<std::endl;

    // std::cout<<std::endl;

    rref(A);
    std::cout<<std::fixed;
    std::cout<<std::setprecision(4)<<A<<std::endl;
    std::cout<<"\e[0m";

    // std::cout<<sizeof(quantum::matrix<double>)<<std::endl;
}
