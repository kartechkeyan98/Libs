#pragma once

#include<iostream>
#include<vector>
#include<initializer_list>
#include<new>
#include<type_traits>
#include<stdexcept>

namespace qb{

class shape{
/**
 * Class to store the shape of a tensor. This will never be empty initialized, except for the time when 
 * We want to get the shape of scalar quantity, in which case shape of size 0 and nullptr may be there.
 */
private:
    size_t* _shape=nullptr;
    size_t _size=0;

    void allocate_memory(){
        size_t n = this->_size;
        this->_shape = new size_t[n];
        if(!this->_shape)throw std::bad_alloc();
    }
    void deallocate_memory(){
        delete[] this->_shape;
        this->_size = 0;
        this->_shape = nullptr;
    }
public:
    /**
     * Constructors:
     * 1. default, empty constructor
     * 2. variadic constructor
     * 3. std::initializer_list
     * 4. std::vector
     * 5. C-style array
     * 6. copy constructor
     */
    // empty
    shape(){}

    // variadic
    template<typename... Sizes, 
    typename = std::enable_if_t<(std::is_convertible_v<Sizes, size_t> && ...)> >
    shape(Sizes... sizes)
    :_size(sizeof...(sizes)){
        allocate_memory();

        size_t i=0;
        ((this->_shape[i++]=sizes),...);
    }

    // initializer_list
    template<typename T,
    typename = std::enable_if_t<std::is_convertible_v<T, size_t> > >
    shape(const std::initializer_list<T>& ls)
    :_size(ls.size()){
        allocate_memory();

        for(size_t i=0;i<this->_size;++i)this->_shape[i] = ls.begin()[i];
    }

    // vector
    template<typename T,
    typename = std::enable_if_t<std::is_convertible_v<T, size_t> > >
    shape(const std::vector<T>& ls)
    :_size(ls.size()){
        allocate_memory();

        for(size_t i=0;i<this->_size;++i)this->_shape[i] = ls[i];
    }

    // C-style array
    template<typename T, size_t N,
    typename = std::enable_if_t<std::is_convertible_v<T, size_t> > >
    shape(const T (&ls)[N]):_size(N){
        allocate_memory();

        for(size_t i=0;i<N;++i) this->_shape[i] = ls[i];
    }

    // copy constructor
    shape(const shape& sh){
        size_t n = this->_size = sh.size();
        allocate_memory();

        for(size_t i=0;i<n;i++)this->_shape[i]=sh[i];
    }

    /* assignment operator(s) */
    // usual
    shape& operator=(const shape& sh){
        if(this != &sh){
            deallocate_memory();
            size_t n = this->_size = len(sh);
            allocate_memory();
            for(size_t i=0;i<n;i++)this->_shape[i]=sh[i];
        }
        return *this;
    }
    // initializer list
    template<typename T,
    typename=std::enable_if_t< is_convertible_v<T,size_t> > >
    shape& operator=(const std::initializer_list<T>& ls){
        deallocate_memory();
        size_t n = this->_size = ls.size();
        allocate_memory();
        for(size_t i=0;i<n;i++)this->_shape[i] = ls.begin()[i];
        return *this;
    }
    // vector
    template<typename T,
    typename=std::enable_if_t< is_convertible_v<T,size_t> > >
    shape& operator=(const std::vector<T>& ls){
        deallocate_memory();
        size_t n = this->_size = ls.size();
        allocate_memory();
        for(size_t i=0;i<n;i++)this->_shape[i] = ls[i];
        return *this;
    }
    // C-style array
    template<typename T, size_t N,
    typename = std::enable_if_t<std::is_convertible_v<T, size_t> > >
    shape& operator=(const T (&ls)[N]){
        deallocate_memory();
        size_t n = this->_size = N;
        allocate_memory();
        for(size_t i=0;i<n;i++)this->_shape[i] = ls[i];
        return *this;
    }



    // destructor
    ~shape(){
        deallocate_memory();
    }

    // only getters, you can only set via assignment, just like a real shape.
    size_t size()const{
        return this->_size;
    }
    size_t operator[](size_t i)const{
        if(i < 0)i+=this->_size;
        if(i<0 || i>=this->_size)std::runtime_error("Index out of range!");
        return this->_shape[i];
    }


    // friend functions
    friend size_t len(const shape& sh){
        return sh._size;
    }
    friend size_t* data(const shape& sh){
        return sh._shape;
    }

};
// final thing. The thing for which this class was born!
bool operator==(const shape& u,const shape& v){
    if(len(u)!=len(v))throw std::runtime_error("Cannot compare shapes of unequal size!");
    if(&u == &v)return true;
    for(size_t i=0;i<len(u);i++){
        if(u[i]!=v[i])return false;
    }
    return true;
}
bool operator!=(const shape& u,const shape& v){
    if(len(u)!=len(v))throw std::runtime_error("Cannot compare shapes of unequal size!");
    if(&u == &v)return false;
    for(size_t i=0;i<len(u);i++){
        if(u[i]!=v[i])return true;
    }
    return false;
}

}


std::ostream& operator<<(std::ostream& out, const qb::shape& sh){
    size_t n = len(sh);
    out<<"<qb::shape, (";
    for(size_t i=0;i<n;i++)out<<sh[i]<<" ";
    if(n!=0)out<<"\b), ";
    else out<<"), ";
    out<<"length = "<<n<<" >";
    return out;
}

void printinfo(std::ostream& out,const qb::shape& u){
    for(int i=0;i<30;i++)out<<"-";
    out<<std::endl;

    out<<u<<std::endl;
    out<<"size: "<<u.size()<<std::endl;
    out<<"addr: "<<&u<<std::endl;
    out<<"data@: "<<data(u)<<std::endl;

    for(int i=0;i<30;i++)out<<"-";
    out<<std::endl<<std::endl;
}

