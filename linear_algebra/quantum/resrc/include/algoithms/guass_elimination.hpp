// File contains algos related to guass elmination and more
#include<matrix/matrix.hpp>
#include<cmath>

template<typename T>
void guass_elimination(quantum::matrix<T> &A){
    pair<size_t> shape=A.size();

    size_t m=shape[0];
    size_t n=shape[1];

    size_t row=0;
    for(size_t j=0;j<n;++j){
        if(row>=m)break;   // nothing left to do

        // find the non-zero value with maximum abs val
        size_t top=row;
        T mx=fabs(A(row,j));
        for(size_t i=row+1;i<m;++i){
            if(abs(A(i,j))>mx){
                top=i;
                mx=abs(A(i,j));
            }
        }
        if(mx==static_cast<T>(0)){
            continue;      // whole column is zero, useless
        }
        A.swap_rows(row,top);

        // now start elminating
        for(size_t i=row+1;i<m;++i){
            A.add_multiple_of_row(i,row,-A(i,j)/A(row,j));
        }
        ++row;
    }
    for(size_t i=0;i<m;++i){
        for(size_t j=0;j<n;++j){
            if(A(i,j)==-0.0f)A(i,j)=+0.0f;
        }
    }
}

template<typename T>
void rref(quantum::matrix<T> &A){
    std::cout<<"Inside rref\n";
    pair<size_t> shape=A.size();

    size_t m=shape[0];
    size_t n=shape[1];

    size_t row=0;
    for(size_t j=0;j<n;++j){
        std::cout<<"Iteration "<<j+1<<std::endl;
        if(row>=m)break;   // nothing left to do

        // find the non-zero value with maximum abs val
        size_t top=row;
        T mx=fabs(A(row,j));
        for(size_t i=row+1;i<m;++i){
            if(abs(A(i,j))>mx){
                top=i;
                mx=abs(A(i,j));
            }
        }
        if(std::round(mx*1000000)/1000000==0){      // this works well till 6 decimal places
            continue;      // whole column is zero, useless
        }
        A.swap_rows(row,top);
        std::cout<<A<<std::endl<<std::endl;

        // now start elminating
        for(size_t i=0;i<m;++i){
            if(i==row)continue;
            A.add_multiple_of_row(i,row,-A(i,j)/A(row,j));
        }
        A.multiply_row(row,1/A(row,j));
        ++row;
        std::cout<<A<<std::endl<<std::endl;
        
    }
    for(size_t i=0;i<m;++i){
        for(size_t j=0;j<n;++j){
            if(A(i,j)==-0.0f)A(i,j)=+0.0f;
        }
    }
    std::cout<<"rref done\n";
}
