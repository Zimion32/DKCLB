#pragma once
#pragma GCC system_header
#include <vector>
#include <string>
template <class T>
struct checked_vector : public std::vector<T>
{
    using std::vector<T>::vector;
    T& operator[] (size_t i)
    { return this->at(i); }
    T const& operator[] (size_t i) const
    { return this->at(i); }
};
extern char* checked_string_zero;
template <class T>
struct checked_string
{
    T value;
    U64 tracking;
    //==================================================================
    checked_string<T>& set_track(U64 track){tracking = track; return *this;}
    U64 get_track(){return tracking;}
    //==================================================================
    checked_string<T>(){}
    checked_string<T>(const T& str): value(str), tracking(0LL) {}
    checked_string<T>(const checked_string<T>& str): value(str.value), tracking(str.tracking) {}
    checked_string<T>(const checked_string<T>& str, size_t pos, size_t len = npos): value(str.value,pos,len), tracking(str.tracking) {}
    checked_string<T>(const char* s): value(s), tracking(0LL) {}
    checked_string<T>(const char* s, size_t n): value(s, n), tracking(0LL) {}
    checked_string<T>(size_t n, char c): value(n,c), tracking(0LL) {}
    template<class InputIterator> checked_string<T>(InputIterator first, InputIterator last): value(first,last), tracking(0LL) {}
    ~checked_string<T>(){}
    checked_string<T>& operator=(const checked_string<T>& str){value = str.value; tracking = str.tracking; return *this;}
    checked_string<T>& operator=(const char* s){value = s; tracking = 0LL; return *this;}
    checked_string<T>& operator=(char c){value = c; tracking = 0LL; return *this;}
    typename T::iterator begin() {return value.begin();}  typename T::const_iterator begin()  const {return value.begin();}
    typename T::iterator end()   {return value.end();}    typename T::const_iterator end()    const {return value.end();}
    typename T::iterator rbegin(){return value.rbegin();} typename T::const_iterator rbegin() const {return value.rbegin();}
    typename T::iterator rend()  {return value.rend();}   typename T::const_iterator rend()   const {return value.rend();}
    typename T::const_iterator cbegin()  const {return value.cbegin();}
    typename T::const_iterator cend()    const {return value.cend();}
    typename T::const_iterator crbegin() const {return value.crbegin();}
    typename T::const_iterator crend()   const {return value.crend();}
    size_t size() const {return value.size();}
    size_t length() const {return value.length();}
    size_t max_size() const {return value.max_size();}
    void resize(size_t n)         {return value.resize(n);} 
    void resize(size_t n, char c) {return value.resize(n,c);}
    size_t capacity() const {return value.capacity();}
    void reserve(size_t n = 0) {return value.reserve(n);} 
    void clear() {return value.clear();} 
    bool empty() const {return value.empty();}
    void shrink_to_fit() {return value.shrink_to_fit();}
    char& operator[] (size_t pos){
        ///THE IMPORTANT PART
        if(pos == size()) return checked_string_zero[0];
        return value.at(pos); 
    }
    const char& operator[] (size_t pos) const{
        ///THE IMPORTANT PART
        if(pos == size()) return checked_string_zero[0];
        return value.at(pos);
    }
    char& at(size_t pos)             { return value.at(pos); }
    const char& at(size_t pos) const { return value.at(pos); }
    char& back()             { return value.back(); }
    const char& back() const { return value.back(); }
    char& front()             { return value.front(); }
    const char& front() const { return value.front(); }
    checked_string<T>& operator+=(const checked_string<T>& str){value += str.value; return *this;}
    checked_string<T>& operator+=(const char* s){value += s; return *this;}
    checked_string<T>& operator+=(char c){value += c; return *this;}
    checked_string<T>& append(const std::string& str) { value.append(str); return *this;}
    checked_string<T>& append(const std::string& str, size_t pos, size_t len = npos) { value.append(str,pos,len); return *this;}
    checked_string<T>& append(const char* s) { value.append(s); return *this;}
    checked_string<T>& append(const char* s, size_t n) { value.append(s, n); return *this;}
    checked_string<T>& append(size_t n, char c) { value.append(n,c); return *this;}
    template<class InputIterator> checked_string<T>& append(InputIterator first, InputIterator last) { value.append(first,last); return *this;}
    void push_back(char c) {value.push_back(c);}
    checked_string<T>& assign(const std::string& str) { value.assign(str); return *this;}
    checked_string<T>& assign(const std::string& str, size_t pos, size_t len = npos) { value.assign(str,pos,len); return *this;}
    checked_string<T>& assign(const char* s) { value.assign(s); return *this;}
    checked_string<T>& assign(const char* s, size_t n) { value.assign(s, n); return *this;}
    checked_string<T>& assign(size_t n, char c) { value.assign(n,c); return *this;}
    template<class InputIterator> checked_string<T>& assign(InputIterator first, InputIterator last) { value.assign(first,last); return *this;}
    checked_string<T>& insert(size_t p, const checked_string& str) { value.insert(p,str.value); return *this;}
    checked_string<T>& insert(size_t p, const checked_string& str, size_t pos, size_t len = npos) { value.insert(p,str.value,pos,len); return *this;}
    checked_string<T>& insert(size_t p, const char* s) { value.insert(p,s); return *this;}
    checked_string<T>& insert(size_t p, const char* s, size_t n) { value.insert(p,s, n); return *this;}
    checked_string<T>& insert(size_t p, size_t n, char c) { value.insert(p,n,c); return *this;}
    checked_string<T>& insert(typename T::iterator p, size_t n, char c) { value.insert(p,n,c); return *this;}
    typename T::iterator insert(typename T::iterator p, char c) { value.insert(p,c); return *this;}
    template<class InputIterator> checked_string<T>& insert(typename T::iterator p, InputIterator first, InputIterator last) {value.insert(p,first,last); return *this;}
    checked_string<T>& erase(size_t pos = 0, size_t len = npos){value.erase(pos,len); return *this;}
    typename T::iterator erase(typename T::iterator p){return value.erase(p);}
    typename T::iterator erase(typename T::iterator first, typename T::iterator last){return value.erase(first,last);}
    checked_string<T>& replace(size_t pos, size_t len, const checked_string<T>& str) {value.replace(pos, len, str.value); return *this;}
    checked_string<T>& replace(typename T::iterator i1, typename T::iterator i2, const checked_string<T>& str) {value.replace(i1, i2, str.value); return *this;}
    checked_string<T>& replace(size_t pos, size_t len, const checked_string<T>& str, size_t subpos, size_t sublen) {value.replace(pos, len, str.value, subpos, sublen); return *this;}
    checked_string<T>& replace(size_t pos, size_t len, const char* s) {value.replace(pos, len, s); return *this;}
    checked_string<T>& replace(typename T::iterator i1, typename T::iterator i2, const char* s) {value.replace(i1, i2, s); return *this;}
    checked_string<T>& replace(size_t pos, size_t len, const char* s, size_t n){value.replace(pos, len, s, n); return *this;}
    checked_string<T>& replace(typename T::iterator i1, typename T::iterator i2, const char* s, size_t n) {value.replace(i1, i2, s, n); return *this;}
    checked_string<T>& replace(size_t pos, size_t len, size_t n, char c) {value.replace(pos, len, n, c); return *this;}
    template<class InputIterator> checked_string<T>& replace(typename T::iterator i1, typename T::iterator i2, InputIterator first, InputIterator last){value.replace(i1,i2,first,last); return *this;}
    void swap(checked_string<T>& str) {value.swap(str.value);}
    void pop_back() {value.pop_back();}
    const char* c_str() const {return value.c_str();}
    const char* data() const {return value.data();}
    typename T::allocator_type get_allocator() const {return value.get_allocator();}
    size_t copy(char* s, size_t len, size_t pos = 0) const {return value.copy(s, len, pos);}
    size_t find(const checked_string<T>& str, size_t pos = 0) const {return value.find(str.value, pos);}
    size_t find(const char* str, size_t pos = 0) const {return value.find(str, pos);}
    size_t find(const char* str, size_t pos, size_t n) const {return value.find(str, pos, n);}
    size_t find(char c, size_t pos = 0) const {return value.find(c, pos);}
    size_t rfind(const checked_string<T>& str, size_t pos = npos) const {return value.rfind(str.value, pos);}
    size_t rfind(const char* str, size_t pos = npos) const  {return value.rfind(str, pos);}
    size_t rfind(const char* str, size_t pos, size_t n) const {return value.rfind(str, pos, n);}
    size_t rfind(char c, size_t pos = npos) const {return value.rfind(c, pos);}
    size_t find_first_of(const checked_string<T>& str, size_t pos = 0) const {return value.find_first_of(str.value, pos);}
    size_t find_first_of(const char* str, size_t pos = 0) const {return value.find_first_of(str, pos);}
    size_t find_first_of(const char* str, size_t pos, size_t n) const {return value.find_first_of(str, pos, n);}
    size_t find_first_of(char c, size_t pos = 0) const {return value.find_first_of(c, pos);}
    size_t find_last_of(const checked_string<T>& str, size_t pos = npos) const {return value.find_last_of(str.value, pos);}
    size_t find_last_of(const char* str, size_t pos = npos) const  {return value.find_last_of(str, pos);}
    size_t find_last_of(const char* str, size_t pos, size_t n) const {return value.find_last_of(str, pos, n);}
    size_t find_last_of(char c, size_t pos = npos) const {return value.find_last_of(c, pos);}
    size_t find_first_not_of(const checked_string<T>& str, size_t pos = 0) const {return value.find_first_not_of(str.value, pos);}
    size_t find_first_not_of(const char* str, size_t pos = 0) const {return value.find_first_not_of(str, pos);}
    size_t find_first_not_of(const char* str, size_t pos, size_t n) const {return value.find_first_not_of(str, pos, n);}
    size_t find_first_not_of(char c, size_t pos = 0) const {return value.find_first_not_of(c, pos);}
    size_t find_last_not_of(const checked_string<T>& str, size_t pos = npos) const {return value.find_last_not_of(str.value, pos);}
    size_t find_last_not_of(const char* str, size_t pos = npos) const  {return value.find_last_not_of(str, pos);}
    size_t find_last_not_of(const char* str, size_t pos, size_t n) const {return value.find_last_not_of(str, pos, n);}
    size_t find_last_not_of(char c, size_t pos = npos) const {return value.find_last_not_of(c, pos);}
    checked_string substr(size_t pos = 0, size_t len = npos) const {return value.substr(pos, len);}
    int compare(const checked_string<T>& str) const {return value.compare(str.value);}
    int compare(size_t pos, size_t len, const checked_string<T>& str) const {return value.compare(pos, len, str.value);}
    int compare(size_t pos, size_t len, const checked_string<T>& str, size_t subpos, size_t sublen) const {return value.compare(pos, len, str.value, subpos, sublen);}
    int compare(const char* s) const {return value.compare(s);}
    int compare(size_t pos, size_t len, const char* s) const {return value.compare(pos, len, s);}
    int compare(size_t pos, size_t len, const char* s, size_t n) const {return value.compare(pos, len, s, n);}
static const size_t npos = -1;
};
static checked_string<std::string> operator+(const checked_string<std::string>& lhs, const checked_string<std::string>& rhs){return lhs.value + rhs.value;}
static checked_string<std::string> operator+(const checked_string<std::string>& lhs, const char* rhs){return lhs.value + rhs;}
static checked_string<std::string> operator+(const char* lhs, const checked_string<std::string>& rhs){return lhs + rhs.value;}
static checked_string<std::string> operator+(const checked_string<std::string>& lhs, char rhs){return lhs.value + rhs;}
static checked_string<std::string> operator+(char lhs, const checked_string<std::string>& rhs){return lhs + rhs.value;}
static bool operator==(const checked_string<std::string>& lhs, const checked_string<std::string>& rhs){return lhs.value == rhs.value;}
static bool operator==(const char* lhs, const checked_string<std::string>& rhs){return lhs == rhs.value;}
static bool operator==(const checked_string<std::string>& lhs, const char* rhs){return lhs.value == rhs;}
static bool operator!=(const checked_string<std::string>& lhs, const checked_string<std::string>& rhs){return lhs.value != rhs.value;}
static bool operator!=(const char* lhs, const checked_string<std::string>& rhs){return lhs != rhs.value;}
static bool operator!=(const checked_string<std::string>& lhs, const char* rhs){return lhs.value != rhs;}
static bool operator<(const checked_string<std::string>& lhs, const checked_string<std::string>& rhs){return lhs.value < rhs.value;}
static bool operator<(const char* lhs, const checked_string<std::string>& rhs){return lhs < rhs.value;}
static bool operator<(const checked_string<std::string>& lhs, const char* rhs){return lhs.value < rhs;}
static bool operator<=(const checked_string<std::string>& lhs, const checked_string<std::string>& rhs){return lhs.value <= rhs.value;}
static bool operator<=(const char* lhs, const checked_string<std::string>& rhs){return lhs <= rhs.value;}
static bool operator<=(const checked_string<std::string>& lhs, const char* rhs){return lhs.value <= rhs;}
static bool operator>(const checked_string<std::string>& lhs, const checked_string<std::string>& rhs){return lhs.value > rhs.value;}
static bool operator>(const char* lhs, const checked_string<std::string>& rhs){return lhs > rhs.value;}
static bool operator>(const checked_string<std::string>& lhs, const char* rhs){return lhs.value > rhs;}
static bool operator>=(const checked_string<std::string>& lhs, const checked_string<std::string>& rhs){return lhs.value >= rhs.value;}
static bool operator>=(const char* lhs, const checked_string<std::string>& rhs){return lhs >= rhs.value;}
static bool operator>=(const checked_string<std::string>& lhs, const char* rhs){return lhs.value >= rhs;}
static void swap(checked_string<std::string>& lhs, checked_string<std::string>& rhs){lhs.value.swap(rhs.value);}
#define Vx  checked_vector
#define TXT checked_string<std::string>
#define T08 checked_string<std::string>
#define T16 std::wstring





























