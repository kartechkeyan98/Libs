#pragma once
//std
#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<cassert>

#if __cplusplus >= 201103L
    #include<initializer_list>
#endif

// our libs
#include<utils.hpp>

/**
 * If you want debug information to be printed, in order to find out
 * when a vector was destroyed or it's data reset to another address,
 * then before including vector.hpp, add #define _vectordebugmode
 */

namespace quantum{

template<typename T>
class vector{
private:
    T* _data;
    size_t _size;
public:
// constructor delegation purposefully not used due to having a ? on c++ std being used
/**
 * CONSTRUCTORS
 */
    vector()
    :_size(0),_data(NULL)
    {
        //printf("Used the null vector consructor\n");
    }
    vector(size_t size)
    :_size(size)
    {
        this->_data=new T[size];
        //printf("Used the empty vector constructor\n");
    }
    template<size_t N>
    vector(const T (&list)[N])
    :_size(N)
    {
        this->_data=new T[N];
        for(size_t i=0;i<N;++i)this->_data[i]=list[i];
        //printf("Created Vector using array constructor\n");
    }
// optional for general use....
#if __cplusplus >= 201103L
    vector(std::initializer_list<T> ls)
    :vector(ls.size())
    {
        for(size_t i=0;i<ls.size();++i)this->_data[i]=ls.begin()[i];
    }
#endif
    vector(const vector<T>& v)
    :_size(v.size())
    {   
        // copy constructor makes deep copies
        this->_data=new T[v.size()];
        for(size_t i=0;i<this->_size;++i)this->_data[i]=v[i];
    }
/**
 * DESTRUCTOR
 */
    ~vector(){
        void* t=this->_data;

        delete[] this->_data;

#ifdef _vectordebugmode
        printf("Deleted vector %p\n",t);
#endif

    }

/**
 * Some things which are important
 */
    size_t size()const{
        return this->_size;
    }
    T* data(){
        return this->_data;
    }

/**
 * Inbuilt operators
 */
    // [] operator
    T operator[](size_t i)const{
        return this->_data[i];
    }
    T& operator[](size_t i){
        return this->_data[i];
    }

    // = operator
    vector<T>& operator=(const vector<T>&v){
        void* t=this->_data;

        delete[] this->_data;
        
        size_t N=v.size();
        this->_size=N;
        this->_data=new T[N];
        for(size_t i=0;i<N;++i)this->_data[i]=v[i];

#ifdef _vectordebugmode
        printf("Reset vector from %p -> %p\n",t,this->_data);
#endif        

        return *this;
    }
    template<size_t N>
    vector<T>& operator=(const T (&list)[N]){
        void* t=this->_data;

        delete[] this->_data;
        this->_size=N;
        this->_data=new T[N];
        for(size_t i=0;i<N;++i)this->_data[i]=list[i];

#ifdef _vectordebugmode
        printf("Reset vector from %p -> %p\n",t,this->_data);
#endif        

        return *this;
    }
#if __cplusplus >= 201103L
    vector<T>& operator=(std::initializer_list<T> ls){
        void* t=this->_data;

        delete[] this->_data;
        this->_size=ls.size();
        this->_data=new T[this->_size];
        for(size_t i=0;i<this->_size;++i)this->_data[i]=ls.begin()[i];

#ifdef _vectordebugmode
        printf("Reset vector from %p -> %p\n",t,this->_data);
#endif 

        return *this;
    }
#endif

/**
 * +=, -=, *=, /= with vectors
 */
    // += operator
    vector<T>& operator+=(const vector<T> &v){
        // v += u
        assert(this->_size==v.size());

        size_t n=v.size();
        for(size_t i=0;i<n;++i)this->_data[i]+=v[i];
        return *this;
    }
    // -=
    vector<T>& operator-=(const vector<T> &v){
        // v -= u
        assert(this->_size==v.size());

        size_t n=v.size();
        for(size_t i=0;i<n;++i)this->_data[i]-=v[i];
        return *this;
    }
    // *=
    vector<T>& operator*=(const vector<T> &v){
        // v *= u
        assert(this->_size==v.size());

        size_t n=v.size();
        for(size_t i=0;i<n;++i)this->_data[i]*=v[i];
        return *this;
    }
    // /=
    vector<T>& operator/=(const vector<T> &v){
        // v /= u
        assert(this->_size==v.size());

        size_t n=v.size();
        for(size_t i=0;i<n;++i)this->_data[i]/=v[i];
        return *this;
    }

/**
 * +=, -=, *=, /= with scalars
 */
    // += operator
    vector<T>& operator+=(const T &k){
        // v += k
        for(size_t i=0, n=this->_size;i<n;++i)this->_data[i]+=k;
        return *this;
    }
    // -=
    vector<T>& operator-=(const T &k){
        // v -= k
        for(size_t i=0, n=this->_size;i<n;++i)this->_data[i]-=k;
        return *this;
    }
    // *=
    vector<T>& operator*=(const T &k){
        // v += k
        for(size_t i=0, n=this->_size;i<n;++i)this->_data[i]*=k;
        return *this;
    }
    // /=
    vector<T>& operator/=(const T &k){
        // v /= k
        for(size_t i=0, n=this->_size;i<n;++i)this->_data[i]/=k;
        return *this;
    }


/**
 * Required Functions
 */
    T dot(const vector<T> &v){
        T res=static_cast<T>(0);
        size_t n=v.size();
        for(size_t i=0;i<n;i++)res+=this->data[i]*v[i];
        return res;
    }
};



/**
 * Misc operator overloads....
 */

// << operator
template<typename T>
std::ostream& operator<<(std::ostream& out, const vector<T> &v){
    size_t n=v.size();
    for(size_t i=0;i<n;i++)out<<v[i]<<" ";
    return out;
}

/**
 * Binary operator overloads for two vectors
 */
// + operator
template<typename T>
vector<T> operator+(const vector<T> &v1,const vector<T> &v2){
    assert(v1.size()==v2.size());

    size_t n=v1.size();
    vector<T>res(v1);

    for(size_t i=0;i<n;++i)res[i]+=v2[i];
    return res;
}
// - operator
template<typename T>
vector<T> operator-(const vector<T> &v1,const vector<T> &v2){
    assert(v1.size()==v2.size());

    size_t n=v1.size();
    vector<T>res(v1);

    for(size_t i=0;i<n;++i)res[i]-=v2[i];
    return res;
}
// * operator
template<typename T>
vector<T> operator*(const vector<T> &v1,const vector<T> &v2){
    assert(v1.size()==v2.size());

    size_t n=v1.size();
    vector<T>res(v1);

    for(size_t i=0;i<n;++i)res[i]*=v2[i];
    return res;
}
// / operator
template<typename T>
vector<T> operator/(const vector<T> &v1,const vector<T> &v2){
    assert(v1.size()==v2.size());

    size_t n=v1.size();
    vector<T>res(v1);

    for(size_t i=0;i<n;++i)res[i]/=v2[i];
    return res;
}

/**
 * Binary operator overloads for vectors with scalars
 */
// + operator
template<typename T>
vector<T> operator+(const vector<T> &v1,const T &k){
    // v1 + k
    size_t n=v1.size();
    vector<T>res(v1);

    for(size_t i=0;i<n;++i)res[i]+=k;
    return res;
}
template<typename T>
vector<T> operator+(const T &k,const vector<T> &v1){
    // v1 + k
    size_t n=v1.size();
    vector<T>res(v1);

    for(size_t i=0;i<n;++i)res[i]+=k;
    return res;
}
// - operator
template<typename T>
vector<T> operator-(const T &k,const vector<T> &v1){
    // k - v1
    size_t n=v1.size();
    vector<T>res(v1);

    for(size_t i=0;i<n;++i)res[i]=k-res[i];
    return res;
}
template<typename T>
vector<T> operator-(const vector<T> &v1,const T& k){
    // v1 - k
    size_t n=v1.size();
    vector<T>res(v1);

    for(size_t i=0;i<n;++i)res[i]-=k;
    return res;
}
// * operator
template<typename T>
vector<T> operator*(const vector<T> &v1,const T& k){
    // v1 * k
    size_t n=v1.size();
    vector<T>res(v1);

    for(size_t i=0;i<n;++i)res[i]*=k;
    return res;
}
template<typename T>
vector<T> operator*(const T &k,const vector<T> &v1){
    // k * v1
    size_t n=v1.size();
    vector<T>res(v1);

    for(size_t i=0;i<n;++i)res[i]*=k;
    return res;
}
// / operator
template<typename T>
vector<T> operator/(const vector<T> &v1,const T& k){
    // v1 * k
    size_t n=v1.size();
    vector<T>res(v1);

    for(size_t i=0;i<n;++i)res[i]/=k;
    return res;
}

/**
 * unary operators for vectors
 */
template<typename T>
vector<T> operator-(const vector<T> &v){
    // -v
    vector<T> res(v);
    for(size_t i=0,n=v.size();i<n;++i)res[i]=v[i];
    return res;
}


}