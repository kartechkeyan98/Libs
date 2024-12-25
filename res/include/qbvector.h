#pragma once

#include<cstdio>
#include<cstdlib>

// std
#include<iostream>
#include<type_traits>
#include<stdexcept>
#include<iomanip>
#include<new>

#include<initializer_list>
#include<vector>
using namespace std;


namespace qb{

template<typename T>
class vector{
private:
    T* _data=nullptr;
    size_t _size=0;
    bool transposed = false;

    /* Meant to be used inside constructor/destructor */
    void allocate_datamem(){
        if(this->_size==0)return;
        this->_data = static_cast<T*>(malloc(this->_size * sizeof(T)));
        if(!this->_data)throw std::bad_alloc();
    }
    void deallocate_datamem(){
        // this is the destructor
        for (size_t i = 0; i < this->_size; i++){
            if(!is_trivially_destructible_v<T>)this->_data[i].~T();
        }
        if(this->_data!=nullptr)delete[] this->_data;
        this->_data=nullptr;
        this->_size=0;
    }
public:
/**
 * Constructors:
 * 1. vector(): default constructor
 * 2. vector(size_t n): n-length array with all elements as zero
 * 3. vector(size_t n, const T* data): n-length array with deepcopy of data argument
 * 4. vector(const T (&data)[n]): template function to take in from array reference
 * 5. vector(Args... args): restricted variadic constructor
 */
    vector(){}
    vector(size_t n):_size(n){ 
        allocate_datamem();     // allocate space for the things
        // use placement new to create the things
        for(size_t i=0;i<n;i++) new (&(this->_data[i])) T(0);
    }
    vector(size_t n, const T* data):_size(n){
        allocate_datamem();     // allocate space for the things
        // use placement new operator
        for(size_t i=0;i<n;i++)new (&(this->_data[i])) T(data[i]);
    }
    template<size_t n>
    vector(const T (&data)[n]):_size(n){
        allocate_datamem();     // allocate space for the things
        // use placement new operator
        for(size_t i=0;i<n;i++)new (&(this->_data[i])) T(data[i]);
    }
    template<typename... Args,
    typename = std::enable_if_t<(std::is_convertible_v<Args, T> && ...)> >
    vector(Args... args){
        this->_size = sizeof...(args);
        allocate_datamem();     // allocate space for the things
        // assign the things
        size_t i=0;
        if constexpr(sizeof...(args)>0){
            ((new (&(this->_data[i++])) T(args)),...);  // fold expressions to assign elements c++17 and above
        }
    }

    // now to deal with std library objects
    vector(const std::initializer_list<T>& data){
        size_t n = this->_size = data.size();
        allocate_datamem();
        for(size_t i=0;i<n;i++)new (&(this->_data[i])) T(*std::next(data.begin(),i));
    }
    vector(const std::vector<T>& data){
        size_t n = this->_size = data.size();
        allocate_datamem();
        for(size_t i=0;i<n;i++)new (&(this->_data[i])) T(data[i]);
    }

    // copy constructor and assignment
    vector(const vector<T>& u){
        this->_size = u.size();
        allocate_datamem();

        for(size_t i=0;i<this->_size;i++)new (&(this->_data[i])) T(u[i]);
    }
    vector<T>& operator=(const vector<T>& u){   // lvalue assignment
        if(this != &u){
            deallocate_datamem();   // destroy all the memory

            // reconstruct this in the image of u [yes, biblical ref. since they are memable] 
            this->_size=u.size();
            allocate_datamem();
            for(size_t i=0;i<this->_size;i++)new (&(this->_data[i])) T(u[i]);
        }
        return *this;
    }


    

/**
 * Destructor
 */
    ~vector(){
        deallocate_datamem();
    }


/**
 * Accessing operators
 * Cut some slack for the python users by enabling reverse indexing
 * Slices are not there mind you.
 */
    T& operator[](size_t i){
        if(i<0)i+=this->_size;
        if(i<0 || i>=this->_size)throw std::runtime_error("Index out of range!");
        return this->_data[i];
    }
    T operator[](size_t i)const{
        if(i<0)i+=this->_size;
        if(i<0 || i>=this->_size)throw std::runtime_error("Index out of range!");
        return this->_data[i];
    }
    size_t size()const{
        return this->_size;
    }
    const T* data()const{    // use a const cast to do shit like this
        return const_cast<const T*>(this->_data);
    } 

/**
 * Arithmetic operation: negation
 */

    vector<T> operator-()const{
        vector<T> res(this->_size);
        for(size_t i=0;i<this->_size;i++)res[i]=-this->_data[i];
        return res;
    }

/**
 * Arithmetic Operations, vector-vector
 */

    template<typename U>
    auto operator+(const vector<U>& u)const{
        if(this->_size != u.size()) throw std::runtime_error("Ensure that vector sizes are the same!");

        using restype=typename std::common_type<T,U>::type;
        vector<restype> res(u);
        for(size_t i=0;i<this->_size;i++)res[i]+=this->_data[i];

        return res;
    }
    template<typename U>
    auto operator-(const vector<U>& u)const{
        if(this->_size != u.size()) throw std::runtime_error("Ensure that vector sizes are the same!");

        using restype=typename std::common_type<T,U>::type;
        vector<restype> res(u);
        for(size_t i=0;i<this->_size;i++)res[i]-=this->_data[i];

        return res;
    } 
    template<typename U>
    auto operator*(const vector<U>& u)const{
        if(this->_size != u.size()) throw std::runtime_error("Ensure that vector sizes are the same!");

        using restype=typename std::common_type<T,U>::type;
        vector<restype> res(u);
        for(size_t i=0;i<this->_size;i++)res[i]*=this->_data[i];

        return res;
    }
    template<typename U>
    auto operator/(const vector<U>& u)const{
        if(this->_size != u.size()) throw std::runtime_error("Ensure that vector sizes are the same!");

        using restype=typename std::common_type<T,U>::type;
        vector<restype> res(u);
        for(size_t i=0;i<this->_size;i++)res[i]/=this->_data[i];

        return res;
    }  

/**
 * Arithmetic operations, with scalars [vector-scalar]
 */

    template<typename U>
    auto operator+(const U& s)const{
        using restype = typename std::common_type<T,U>::type;
        vector<restype> res(*this);
        for(size_t i=0;i<this->_size;i++)res[i]+=s;
        return res;
    }
    template<typename U>
    auto operator-(const U& s)const{
        using restype = typename std::common_type<T,U>::type;
        vector<restype> res(*this);
        for(size_t i=0;i<this->_size;i++)res[i]-=s;
        return res;
    }
    template<typename U>
    auto operator*(const U& s)const{
        using restype = typename std::common_type<T,U>::type;
        vector<restype> res(*this);
        for(size_t i=0;i<this->_size;i++)res[i]*=s;
        return res;
    }
    template<typename U>
    auto operator/(const U& s)const{
        using restype = typename std::common_type<T,U>::type;
        vector<restype> res(*this);
        for(size_t i=0;i<this->_size;i++)res[i]/=s;
        return res;
    }

/**
 * Transpose operation
 */

    void transpose(){
        this->transposed = !this->transposed;
    }

    


};

/**
 * Arithmetic operations, scalar-vector
 */

template<typename T,typename U, typename=std::enable_if_t<(std::is_scalar_v<U>)> >
auto operator+(const U& s, const vector<T>& u){
    using restype = typename std::common_type<T,U>::type;
    vector<restype> res(u);
    for(size_t i=0;i<u.size();i++)res[i]+=s;
    return res;
}
template<typename T,typename U, typename=std::enable_if_t<(std::is_scalar_v<U>)> >
auto operator*(const U& s, const vector<T>& u){
    using restype = typename std::common_type<T,U>::type;
    vector<restype> res(u);
    for(size_t i=0;i<u.size();i++)res[i]*=s;
    return res;
}
template<typename T,typename U,typename=std::enable_if_t<(std::is_scalar_v<U>)> >
auto operator-(const U& s, const vector<T>& u){
    using restype = typename std::common_type<T,U>::type;
    vector<restype> res(u);
    for(size_t i=0;i<u.size();i++)res[i]=s-res[i];
    return res;
}

}

template<typename T>   
std::ostream& operator<<(std::ostream& out, const qb::vector<T>& v){
    size_t n=v.size();
    out<<"<qb::vector, (";
    for(int i=0;i<n;i++)out<<v[i]<<" ";
    if(n!=0)out<<"\b)";
    else out<<")";
    out<<", length = "<<n<<" >";

    return out;
}

template<typename T>
void printinfo(std::ostream& out,const qb::vector<T>& u){
    for(int i=0;i<30;i++)out<<"-";
    out<<std::endl;

    out<<u<<std::endl;
    out<<"size: "<<u.size()<<std::endl;
    out<<"addr: "<<&u<<std::endl;
    out<<"data@: "<<u.data()<<std::endl;

    for(int i=0;i<30;i++)out<<"-";
    out<<std::endl<<std::endl;
}