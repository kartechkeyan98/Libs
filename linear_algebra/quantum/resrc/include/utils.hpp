#pragma once
#include<cstdio>
#include<cstddef>
#include<cstdarg>

template<typename T>
struct pair{
    T x,y;

    T operator [](size_t i)const{
        if(i==0)return x;
        else if(i==1)return y;
        else {
            printf("Stop giving out of bounds index! You are gay!\n");
            exit(1);
        }
    }
    bool operator==(const pair<T>& p){
        return p[0]==this->x&&p[1]==this->y;
    }
};

template<typename T> 
std::ostream& operator<<(std::ostream& out, const pair<T> &p){
    out<<"("<<p[0]<<", "<<p[1]<<")";
    return out;
}

// template<typename T, size_t N>
// struct tuple{
//     size_t _size=N;
//     T _data[N];
// };