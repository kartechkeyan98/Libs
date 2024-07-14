#pragma once

//std
#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<cassert>
#include<cstdarg>

#if __cplusplus >= 201103L
    #include<initializer_list>
#endif

// our libs
#include<utils.hpp>

namespace quantum{

template<typename T>
class matrix{
private:
    size_t _rows;
    size_t _cols;
    T** _data;
public:
    matrix()
    :_rows(0),_cols(0),_data(NULL)
    {}
    matrix(size_t r, size_t c)
    :_rows(r),_cols(c)
    {
        this->_data=new T*[r];
        for(size_t i=0;i<r;++i)this->_data[i]=new T[c];
    }
    matrix(size_t r,size_t c,const T& k)
    :_rows(r),_cols(c){
        this->_data=new T*[r];
        for(size_t i=0;i<r;++i)this->_data[i]=new T[c];
        for(size_t i=0;i<r;++i)for(size_t j=0;j<c;++j)this->_data[i][j]=k;
    }

    template<size_t M, size_t N>
    matrix(const T (&mat)[M][N]):_rows(M),_cols(N)
    {
        this->_data=new T*[M];
        for(size_t i=0;i<M;++i)this->_data[i]=new T[N];
        for(size_t i=0;i<M;++i)for(size_t j=0;j<N;++j)this->_data[i][j]=mat[i][j];
    }
#if __cplusplus >= 201103L
    matrix(std::initializer_list<std::initializer_list<T> > ls)
    :matrix(ls.size(),ls.begin()[0].size())
    {
        for(size_t i=0;i<this->_rows;++i)for(size_t j=0;j<this->_cols;++j)this->_data[i][j]=((ls.begin()+i)->begin()[j]);
    }
#endif 

    // copy constructor
    matrix(const matrix<T> &A){
        pair<size_t> p=A.size();
        this->_rows=p[0],this->_cols=p[1];
        this->_data=new T*[p[0]];
        for(size_t i=0;i<p[0];++i)this->_data[i]=new T[p[1]];
        for(size_t i=0;i<p[0];++i)for(size_t j=0;j<p[1];++j)this->_data[i][j]=A(i,j);
    }
    
    ~matrix(){
        void* t=this->_data;
        for(size_t i=0;i<this->_rows;++i)delete[] this->_data[i];
        if(t!=NULL)delete[] this->_data;
#ifdef _matrixdebugmode
        printf("Deleted matrix at %p\n",t);
#endif
    }


    pair<size_t> size()const{
        return {this->_rows,this->_cols};
    }
    T** data(){
        return this->_data;
    }

    // access elements individually
    T operator()(size_t i,size_t j)const{
        assert(i>=0&&j>=0&&i<this->_rows&&j<this->_cols);
        return this->_data[i][j];
    }
    T& operator()(size_t i, size_t j){
        assert(i>=0&&j>=0&&i<this->_rows&&j<this->_cols);
        return this->_data[i][j];
    }
    // accessing rows & cols: A(i,:), A(:,j)....not quite possible with this representation
    // an alternate representation too memory intensive....so we make any row/column ops as subroutines
    T* operator()(size_t i){
        assert(i>=0&&i<this->_rows);
        return this->_data[i];
    }
    




    /**
     *  = operator
     */
    matrix<T>& operator=(const matrix<T> &A){
        void* t=this->_data;

        for(size_t i=0;i<this->_rows;++i)delete[] this->_data[i];
        delete[] this->_data;

        this->_rows=A.size()[0];
        this->_cols=A.size()[1];
        
        this->_data=new T*[this->_rows];
        for(size_t i=0;i<this->_rows;++i)this->_data[i]=new T[this->_cols];
        for(size_t i=0;i<this->_rows;++i)for(size_t j=0;j<this->_cols;++j)this->_data[i][j]=A(i,j);

#ifdef _matrixdebugmode
        printf("Reset matrix from %p -> %p\n",t,this->_data);
#endif        

        return *this;
    }
    template<size_t M,size_t N>
    matrix<T>& operator=(const T (&mat)[M][N]){
        void* t=this->_data;

        for(size_t i=0;i<this->_rows;++i)delete[] this->_data[i];
        delete[] this->_data;

        this->_rows=M;
        this->_cols=N;
        
        this->_data=new T*[M];
        for(size_t i=0;i<M;++i)this->_data[i]=new T[N];
        for(size_t i=0;i<M;++i)for(size_t j=0;j<N;++j)this->_data[i][j]=mat[i][j];

#ifdef _matrixdebugmode
        printf("Reset matrix from %p -> %p\n",t,this->_data);
#endif        

        return *this;
    }
#if __cplusplus >= 201103L
    matrix<T>& operator=(std::initializer_list<std::initializer_list<T> > ls){
        void *t=this->_data;

        for(size_t i=0;i<this->_rows;++i)delete[] this->_data[i];
        delete[] this->_data;

        this->_rows=ls.size();
        this->_cols=ls.begin()->size();
        
        this->_data=new T*[this->_rows];
        for(size_t i=0;i<this->_rows;++i)this->_data[i]=new T[this->_cols];
        for(size_t i=0;i<this->_rows;++i)for(size_t j=0;j<this->_cols;++j)this->_data[i][j]=(ls.begin()+i)->begin()[j];

#ifdef _matrixdebugmode
        printf("Reset matrix from %p -> %p\n",t,this->_data);
#endif        

        return *this;
    }
#endif

    // how tf will I create submatrices and all......

    /**
     * +=, -=, *=, /= operators for matrices
     */
    matrix<T>& operator+=(const matrix<T> &B){
        // A += B
        assert(this->size()==B.size());

        pair<size_t>shape=B.size();
        for(size_t i=0;i<shape[0];++i)for(size_t j=0;j<shape[1];++j)this->_data[i][j]+=B(i,j);

        return *this;
    }
    matrix<T>& operator-=(const matrix<T> &B){
        // A -= B
        assert(this->size()==B.size());

        pair<size_t>shape=B.size();
        for(size_t i=0;i<shape[0];++i)for(size_t j=0;j<shape[1];++j)this->_data[i][j]-=B(i,j);

        return *this;
    }
    matrix<T>& operator*=(const matrix<T> &B){
        // A *= B
        assert(this->size()==B.size());

        pair<size_t>shape=B.size();
        for(size_t i=0;i<shape[0];++i)for(size_t j=0;j<shape[1];++j)this->_data[i][j]*=B(i,j);

        return *this;
    }
    matrix<T>& operator/=(const matrix<T> &B){
        // A /= B
        assert(this->size()==B.size());

        pair<size_t>shape=B.size();
        for(size_t i=0;i<shape[0];++i)for(size_t j=0;j<shape[1];++j)this->_data[i][j]/=B(i,j);

        return *this;
    }

    /**
     * += and all but with scalars...
     */
    matrix<T>& operator+=(const T &k){
        // A += k
        pair<size_t>shape=this->size();
        for(size_t i=0;i<shape[0];++i)for(size_t j=0;j<shape[1];++j)this->_data[i][j]+=k;

        return *this;
    }
    matrix<T>& operator-=(const T &k){
        // A -= k
        pair<size_t>shape=this->size();
        for(size_t i=0;i<shape[0];++i)for(size_t j=0;j<shape[1];++j)this->_data[i][j]-=k;

        return *this;
    }
    matrix<T>& operator*=(const T &k){
        // A *= k
        pair<size_t>shape=this->size();
        for(size_t i=0;i<shape[0];++i)for(size_t j=0;j<shape[1];++j)this->_data[i][j]*=k;

        return *this;
    }
    matrix<T>& operator/=(const T &k){
        // A /= k
        pair<size_t>shape=this->size();
        for(size_t i=0;i<shape[0];++i)for(size_t j=0;j<shape[1];++j)this->_data[i][j]/=k;

        return *this;
    }


    /**
     * Matrix Multiplication Bitch!
     */
    matrix<T> matmul(const matrix<T> &B){
        assert(this->_cols==B.size()[0]);

        

        size_t m=this->_rows;
        size_t n=this->_cols;
        size_t p=B.size()[1];

        matrix<T> res(m,p,static_cast<T>(0));

        for(size_t i=0;i<m;++i){
            for(size_t j=0;j<p;++j){
                for(size_t k=0;k<n;++k)res(i,j)+=this->_data[i][k]*B(k,j);
            }
        }

        return res;
    }

    // another fundamental matrix operation: transpose
    matrix<T> t(){
        matrix<T> res(this->_cols,this->_rows);
        int m=this->_rows;
        int n=this->_cols;
        for(size_t i=0;i<m;++i)for(size_t j=0;j<n;++j)res(j,i)=this->_data[i][j];
        return res;
    }


    /**
     * Important Elementary row ops
     */
    void swap_rows(size_t i, size_t j){
        assert(i>=0&&j>=0&&i<this->_rows&&j<this->_rows);
        if(i==j)return;
        T* temp=this->_data[i];
        this->_data[i]=this->_data[j];
        this->_data[j]=temp;
    }
    void multiply_row(size_t i, const T& k){
        assert(i>=0&&i<this->_rows);
        for(size_t j=0;j<this->_cols;++j)this->_data[i][j]*=k;
    }
    void add_multiple_of_row(size_t i, size_t j, const T& c){
        // A(i) += k*A(j)
        assert(i>=0&&j>=0&&i<this->_rows&&j<this->_rows);
        for(size_t k=0;k<this->_cols;++k)this->_data[i][k]+=c*this->_data[j][k];
    }




    
};

/**
 * Operator overloading
 */
template<typename T>
std::ostream& operator<<(std::ostream& out, const matrix<T> &A){
    pair<size_t> p=A.size();
    for(size_t i=0;i<p[0];++i){
        for(size_t j=0;j<p[1];++j)out<<A(i,j)<<" ";
        if(i!=p[0]-1)out<<"\n";
    }
    return out;
}

/**
 * Unary Operators: just '-'
 */
template<typename T>
matrix<T> operator-(const matrix<T> &A){
    matrix<T> res(A);
    pair<size_t>shape=A.size();
    for(size_t i=0;i<shape[0];++i)for(size_t j=0;j<shape[1];++j)res(i,j)=-res(i,j);
    return res;
}

/**
 * Binary operators with matrices
 */
// operator +
template<typename T>
matrix<T> operator+(const matrix<T> &A,const matrix<T> &B){
    assert(A.size()==B.size());

    pair<size_t>shape=A.size();
    matrix<T> res(A);
    res+=B;
    return res;
}
// - operator
template<typename T>
matrix<T> operator-(const matrix<T> &A,const matrix<T> &B){
    assert(A.size()==B.size());

    pair<size_t>shape=A.size();
    matrix<T> res(A);
    res-=B;
    return res;
}
// * operator
template<typename T>
matrix<T> operator*(const matrix<T> &A,const matrix<T> &B){
    assert(A.size()==B.size());

    pair<size_t>shape=A.size();
    matrix<T> res(A);
    res*=B;
    return res;
}
// / operator
template<typename T>
matrix<T> operator/(const matrix<T> &A,const matrix<T> &B){
    assert(A.size()==B.size());

    pair<size_t>shape=A.size();
    matrix<T> res(A);
    res/=B;
    return res;
}

/**
 * Binary operators but with one of the args being a scalar
 */
// + operator
template<typename T>
matrix<T> operator+(const matrix<T> &A,const T &k){
    // A + k
    pair<size_t> shape=A.size();
    matrix<T>res(A);

    res+=k;
    return res;
}
template<typename T>
matrix<T> operator+(const T &k,const matrix<T> &A){
    // A + k
    pair<size_t> shape=A.size();
    matrix<T>res(A);

    res+=k;
    return res;
}
// - operator
template<typename T>
matrix<T> operator-(const matrix<T> &A,const T &k){
    // A - k
    pair<size_t> shape=A.size();
    matrix<T>res(A);

    res+=k;
    return res;
}
template<typename T>
matrix<T> operator-(const T &k,const matrix<T> &A){
    // k - A
    pair<size_t> shape=A.size();
    matrix<T>res(A);

    res-=k;
    res=-res;
    return res;
}
// * operator
template<typename T>
matrix<T> operator*(const matrix<T> &A,const T &k){
    // A * k
    pair<size_t> shape=A.size();
    matrix<T>res(A);

    res*=k;
    return res;
}
template<typename T>
matrix<T> operator*(const T &k,const matrix<T> &A){
    // k * A
    pair<size_t> shape=A.size();
    matrix<T>res(A);

    res*=k;
    return res;
}
// / operator
template<typename T>
matrix<T> operator/(const matrix<T> &A,const T &k){
    // A / k
    pair<size_t> shape=A.size();
    matrix<T>res(A);

    res/=k;
    return res;
}


/**
 * Heart of linear algebra and more:
 * Matrix Multiplication
 */
template<typename T>
matrix<T> matmul(const matrix<T> &A, const matrix<T> &B){
    assert(A.size()[1]==B.size()[0]);

    pair<size_t>ashape=A.size();
    pair<size_t>bshape=B.size();

    matrix<T> res(ashape[0],bshape[1],static_cast<T>(0));

    for(size_t i=0;i<ashape[0];++i){
        for(size_t j=0;j<bshape[1];++j){
            for(size_t k=0;k<bshape[0];++k)res(i,j)+=A(i,k)*B(k,j);
        }
    }
    return res;
}
}
