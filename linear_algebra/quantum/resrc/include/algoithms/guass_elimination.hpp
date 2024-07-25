// File contains algos related to guass elmination and more
#include<matrix/matrix.hpp>
#include<cmath>


template<typename T>
void guass_elimination(quantum::matrix<T> &A){
    pair<size_t> dims=A.size();
    double tolerance=1e-6;

    for(size_t i=0;i<dims[0];++i){
        // find the first non-zero column in the submatrix A(i:,:)
        size_t cidx;
        size_t rmax;
        bool discont=true;
        for(size_t j=i;j<dims[1];++j){
            rmax=i;
            T tmp=A(i,j);
            for(size_t k=i+1;k<dims[1];++k){
                if(fabs(A(k,j))>fabs(tmp)){
                    tmp=A(k,j);
                    rmax=j;
                }
            }
            if(fabs(A(rmax,j))>tolerance){
                cidx=j;
                discont=false;
                break;
            }
        }
        if(discont)break;

        if(rmax!=i)A.swap_rows(i,rmax);

        for(size_t j=i+1;j<dims[0];++j){
            A.add_multiple_of_row(j,i,-A(j,cidx)/A(i,cidx));
        }
    }
    for(size_t i=0;i<dims[0];++i){
        for(size_t j=0;j<dims[1];++j){
            // introduces numerical stability later on till 6 digits
            A(i,j) = T((double)std::round(1e15*(double)A(i,j))/1e15);
            if(A(i,j)==-0.0f)A(i,j)=+0.0f;
        }
    }
}   // O(m*n^2)

template<typename T>
void rref(quantum::matrix<T> &A){
    // std::cout<<"Inside rref\n";
    pair<size_t> dims=A.size();
    double tolerance=1e-7;

    for(size_t i=0;i<dims[0];++i){
        // find the first non-zero column in the submatrix A(i:,:)
        size_t cidx;
        size_t rmax;
        bool discont=true;
        for(size_t j=i;j<dims[1];++j){
            rmax=i;
            T tmp=A(i,j);
            for(size_t k=i+1;k<dims[1];++k){
                if(fabs(A(k,j))>fabs(tmp)){
                    tmp=A(k,j);
                    rmax=j;
                }
            }
            if(fabs(A(rmax,j))>tolerance){
                cidx=j;
                discont=false;
                break;
            }
        }
        if(discont)break;

        if(rmax!=i)A.swap_rows(i,rmax);

        for(size_t j=0;j<dims[0];++j){
            if(j==i)continue;
            A.add_multiple_of_row(j,i,-A(j,cidx)/A(i,cidx));
        }
        A.multiply_row(i,1/A(i,cidx));
    }
    for(size_t i=0;i<dims[0];++i){
        for(size_t j=0;j<dims[1];++j){
            // introduces numerical stability later on till 6 digits
            A(i,j) = T((double)std::round(1e15*(double)A(i,j))/1e15);
            if(fabs(A(i,j)-(-0.0f))<=tolerance)A(i,j)=+0.0f;
        }
    }
    
} // O(m*n^2)
