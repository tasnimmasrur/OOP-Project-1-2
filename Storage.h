#ifndef STORAGE_H
#define STORAGE_H
#include <vector>

template<class T>
class Storage {
private:
    std::vector<T> data;
public:
    void add(T item) { data.push_back(item); }
    const std::vector<T>& getAll() const { return data; }
    std::vector<T> getAllCopy() const { return data; }
    void clear() { data.clear(); }


    void remove(int index) {
        if(index >= 0 && index < data.size()) {
            data.erase(data.begin() + index);
        }
    }

    void set(int index, T item) {
        if(index >= 0 && index < data.size()) {
            data[index] = item;
        }
    }
};
#endif
