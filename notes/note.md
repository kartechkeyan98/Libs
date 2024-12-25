# Notes for `qubix2`

<div style="text-align:justify;">
Just to be clear, this is a linear algebra library. It is not a tensor 
processing library or an XLA framework. The goal is to perform simple to advanced linear algebra in this, with support for GPU included. Another motive is to enable usage of multi-dimensional arrays. <em>C++ 17</em> or higher is required for this library.
</div>

# 1. Library Products
## 1.1 vector
This is a fancy way of saying: an array with **n** elements.

### 1.1.1 Attributes
- `size_t _size = 0` to store the size of vector, 0 by default
- `T* _data = nullptr` to store the elements, null by default
- `bool transposed = false` to store whether it is a row or a column vector, column by default, in case you want to make it geometric

These are private variables.

### 1.1.2 Constructors
<div style="text-align:justify;">
Now there are multiple ways to programmatically construct a vector and we
want to encapsulate the most useful of them into our vector class. Below is a
list of the ones proposed:
</div>

1. **Empty Constructor**: `vector()` will do nothing 
```cpp
vector(){}
```

2. **Given only size**: `vector(size_t n)` will allocate space for $n$ elements and then set them all to zero
```cpp
vector(size_t n):_size(n){ 
    allocate_datamem();     // allocate space for the things
    // use placement new to create the things
    for(size_t i=0;i<n;i++) new (&(this->_data[i])) T(0);
}
```

3. **Pass array by pointer**: `vector(size_t n, const T* data)` will create a deep copy of the `data` array and set the size. This method works for stack arrays or heap arrays of size $n$. The `const T*` ensures that the value it points to doesn't change. Don't use this methods for temp arrays though, only for already defined ones!
```cpp
vector(size_t n, const T* data):_size(n){
    allocate_datamem();     // allocate space for the things
    // use placement new operator
    for(size_t i=0;i<n;i++)new (&(this->_data[i])) T(data[i]);
}
```

4. **Pass array by reference**: `vector(const T (&data)[N])` will be a templated function, which will set the size as well. It produces a deep copy. This method can be used for temp arrays.
```cpp
template<size_t n>
vector(const T (&data)[n]):_size(n){
    allocate_datamem();     // allocate space for the things
    // use placement new operator
    for(size_t i=0;i<n;i++)new (&(this->_data[i])) T(data[i]);
}
```

5. **Variadic template**: `vector(Args... args)` will be a variadic way of constructing a vector. This is a very simple method (from end use point of view anyways).
```cpp
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
```
Some issues faced are: variadic templates are very greedy and match just about anything they can. So, you have to restrict them by using gimmicks like `enable_if`. An alternate solution is that you can mark all other constructors as `explicit`.

6. **Using stl containers**: Using containers like `std::vector` or `std::initializer_list`.
```cpp
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
```

7. **Copy Constructor and Assignment Operator**: Both produce deep copies and assignment erases current contents and then creates everything fresh. This thing ensures that you never use the array inside directly, not a bug, but a feature!
```cpp
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
```

### 1.1.3 Destructors
Now there is only one destructor
```cpp
~vector(){
    deallocate_datamem();
}
```

### 1.1.4 Private Member Functions
1. `allocate_datamem()`:
```cpp
void allocate_datamem(){
    if(this->_size==0)return;
    this->_data = static_cast<T*>(malloc(this->_size * sizeof(T)));
    if(!this->_data)throw std::bad_alloc();
}
```
This creates space to allocate $n$ elements of type T, the constructors merely take care of filling those precise spaces

2. `deallocate_datamem()`:
```cpp
void deallocate_datamem(){
    // this is the destructor
    for (size_t i = 0; i < this->_size; i++){
        if(!is_trivially_destructible_v<T>)this->_data[i].~T();
    }
    if(this->_data!=nullptr)delete this->_data;
    this->_data=nullptr;
    this->_size=0;
}
```
This is the destructor tbh. It takes care of destroying array elements. An alternative is to use smart pointers, but this ain't that sort of project.

### 1.1.4 vector Operations

1. **Accessing and Setting**: Getter and setter operations
```cpp
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
```

2. **Access attributes**:
```cpp
size_t size()const{
    return this->_size;
}
const T* data(){    // use a const cast to do shit like this
    return const_cast<const T*>(this->_data);
}
```

3. **Arithmetic Operations**: This has many flavors. The ones shown are:
- *vector-vector operations*: 
```cpp
template<typename U>
auto operator+(const vector<U>& u)const{
    if(this->_size != u.size()) throw std::runtime_error("Ensure that vector sizes are the same!");

    using restype=typename std::common_type<T,U>::type;
    vector<restype> res(u);
    for(size_t i=0;i<this->_size;i++)res[i]+=this->_data[i];

    return res;
}
```
This is the sort of code that will be used for -, * and / too. We want to make sure that the resultant vector is of type which can hold both the types *T* and *U*.

- *vector-scalar operations*: This is for operation between a vector and a scalar.

```cpp
template<typename U>
auto operator+(const U& s)const{
    using restype = typename std::common_type<T,U>::type;
    vector<restype> res(*this);
    for(size_t i=0;i<this->_size;i++)res[i]+=s;
    return res;
}
```


We can also define the reverse for few of these ops, we can define the reverse for every other operation other than / since dividing by a vector makes no sense.

- *Unary minus(-) operator*:
```cpp
vector<T> operator-()const{
    vector<T> res(this->_size);
    for(size_t i=0;i<this->_size;i++)res[i]=-this->_data[i];
    return res;
}
```

- *scalar-vector operations*: The implementation has a `std::enable_if<>` in it because it may cause ambiguities with the vector-scalar operator
```cpp
template<typename T,typename U,typename=std::enable_if_t<(std::is_scalar_v<U>)> >
auto operator-(const U& s, const vector<T>& u){
    using restype = typename std::common_type<T,U>::type;
    vector<restype> res(u);
    for(size_t i=0;i<u.size();i++)res[i]=s-res[i];
    return res;
}
```

- *transpose*:
Toggles between row and column interpretation of the `vector` class.
```cpp
void transpose(){
    this->transposed = !this->transposed;
}
```

## 1.2 Shape

It will contain the dimensions of any `tensor` object (covered in next section).

### 1.2.1 Attributes
- `size_t _size = 0` to store the size of array, 0 by default
- `size_t* _data = nullptr` to store the elements, null by default

These are private variables

### 1.2.2 Constructors

1. **Empty Constructor**: This does nothing.

```cpp
// empty
shape(){}
```

2. **Variadic Constructor**: Takes in variable arguments only when all of them can be converted into `size_t`.
```cpp
// variadic
template<typename... Sizes, 
typename = std::enable_if_t<(std::is_convertible_v<Sizes, size_t> && ...)> >
shape(Sizes... sizes)
:_size(sizeof...(sizes)){
    allocate_memory();

    size_t i=0;
    ((this->_shape[i++]=sizes),...);
}
```

3. **Initializer List**: Here we use `std::initializer_list` as the argument.
```cpp
// initializer_list
template<typename T,
typename = std::enable_if_t<std::is_convertible_v<T, size_t> > >
shape(const std::initializer_list<T>& ls)
:_size(ls.size()){
    allocate_memory();

    for(size_t i=0;i<this->_size;++i)this->_shape[i] = ls.begin()[i];
}
```

4. **STL Vector**: Same logic as the initializer list case.
```cpp
// vector
template<typename T,
typename = std::enable_if_t<std::is_convertible_v<T, size_t> > >
shape(const std::vector<T>& ls)
:_size(ls.size()){
    allocate_memory();

    for(size_t i=0;i<this->_size;++i)this->_shape[i] = ls[i];
}
```

5. **C-style arrays**: To use C-style arrays, we use the following.
```cpp
// C-style array
template<typename T, size_t N,
typename = std::enable_if_t<std::is_convertible_v<T, size_t> > >
shape(const T (&ls)[N]):_size(N){
    allocate_memory();

    for(size_t i=0;i<N;++i) this->_shape[i] = ls[i];
}
```

6. **Copy Constructor**: This always creates deep copies, not shallow ones. Logic same as that for `qb::vector`.
```cpp
// assignment operator
shape& operator=(const shape& sh){
    if(this != &sh){
        deallocate_memory();
        size_t n = this->_size = len(sh);
        allocate_memory();
        for(size_t i=0;i<n;i++)this->_shape[i]=sh[i];
    }
    return *this;
}
```
We are interested in chaining the operator when required actually, which is a rare case, but happens to certain people.

### 1.2.3 Destructors

Same as before:
```cpp
// destructor
~shape(){
    deallocate_memory();
}
```

### 1.2.4 Auxillary methods

1. **Allocate Memory**(`allocate_memory()`): Used for allocating space.
```cpp
void allocate_memory(){
    size_t n = this->_size;
    this->_shape = new size_t[n];
    if(!this->_shape)throw std::bad_alloc();
}
```

2. **Deallocate Memory**(`deallocate_memory()`): Used to free up space.
```cpp
void deallocate_memory(){
    delete[] this->_shape;
    this->_size = 0;
    this->_shape = nullptr;
}
```

### 1.2.5 Methods

1. **Getters**: In shapes, we can only get what the attribs, not set them. The following two functions act as getters for length of your `shape` and element `i` respectively.
```cpp
size_t size()const{
    return this->_size;
}
size_t operator[](size_t i)const{
    if(i < 0)i+=this->_size;
    if(i<0 || i>=this->_size)std::runtime_error("Index out of range!");
    return this->_shape[i];
}
```

2. **Length**: To give it a more pythonic feel, a `friend` function called `len()` has also been incorporated.
```cpp
friend size_t len(const shape& sh){
    return sh._size;
}
```
The logic behind this implementation is that we can have a separate length for each class that requires such functionality and by making it a `friend` function, we have a way to access private attributes like `_size`, which otherwise would have to be accessed by calling `size()` function, adding additional overhead to the call stack.

3. **data**: this is optional and only for debug purposes, so that the `printinfo()` function may be used
```cpp
friend size_t* data(const shape& sh){
    return sh._shape;
}
```


### 1.2.6 Additional Stuff

Printing the `shape`:
```cpp
std::ostream& operator<<(std::ostream& out, const qb::shape& sh){
    size_t n = len(sh);
    out<<"<qb::shape, (";
    for(size_t i=0;i<n;i++)out<<sh[i]<<" ";
    if(n!=0)out<<"\b), ";
    else out<<"), ";
    out<<"length = "<<n<<" >";
    return out;
}
```

Print out the info of our `shape`:
```cpp
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
```

### 1.2.7 Operators

1. **Assignment**: The `=` operator is the key operator used in any class. The operator is overloaded in multiple ways as follows
```cpp
shape& operator=(const shape& sh){
    if(this != &sh){
        deallocate_memory();
        size_t n = this->_size = len(sh);
        allocate_memory();
        for(size_t i=0;i<n;i++)this->_shape[i]=sh[i];
    }
    return *this;
}
```
This takes care of assignment operator when chained together.

If we have a `std::initializer_list` or a `std::vector` in our case, then those can be assigned as well, which means the only way to modify a shape variable is to use the assignment operator.

```cpp
/* assignment operator(s) */
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
    size_t n = this->_size = ls.size();
    allocate_memory();
    for(size_t i=0;i<n;i++)this->_shape[i] = ls[i];
    return *this;
}
```

2. **Comparisons**: Only `==` and `!=` permitted!
```cpp
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
```

## 1.3 Tensors

### 1.3.1 Attributes

 