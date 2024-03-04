#ifndef SMALL_HEAP_H
#define SMALL_HEAP_H

#include<vector>
#include<unordered_map>
#include<assert.h>
#include<algorithm>
#include<memory>

/**
 *  小顶堆
 *  T 需要实现 id() 代表标记， priority() 代表优先级, 返回引用
 */
template<typename T>
class SmallHeap {

private:
    std::vector<T> heap; // 用数组存储一个完全二叉树(堆)
    std::unordered_map<int, size_t> map; // 用map记录index, (key,value) = (id,index)

    void swap(size_t index1, size_t index2); //用于交换
    void up(size_t index); // 用于up堆化
    void down(size_t index); // 用于down堆化
    void adjust(size_t index);

public:

    SmallHeap(){}
    ~SmallHeap(){}

    void adjust(int id, int priority);
    void add(const T &t);
    void del(int id);
    T front();
    void pop();
    size_t size();

};

template<typename T>
void SmallHeap<T>::swap(size_t index1, size_t index2){
    assert(index1 >= 0 && index1 < heap.size());
    assert(index2 >= 0 && index2 < heap.size());
    std::swap(heap[index1], heap[index2]);
    map[heap[index1].id()] = index1;
    map[heap[index2].id()] = index2;
}

template<typename T>
void SmallHeap<T>::up(size_t index){
    assert(index >= 0 && index < heap.size());
    size_t i = index;
    size_t j = (i - 1) / 2; // 父节点为（i - 1) / 2
    while (j >= 0)
    {
        if(heap[i].priority() < heap[j].priority()){  // 比父节点小，需要上移
            swap(i, j);
            i = j;
            j = (i - 1) / 2;
        }else {
            break;
        } 
    }
}

template<typename T>
void SmallHeap<T>::down(size_t index){
    assert(index >= 0 && index < heap.size());
    size_t i = index;
    size_t j = 2 * i + 1; // 左子节点为 2i + 1
    size_t n = heap.size();
    while (j < n)
    {
        // 如果右节点比左节点小，需要与右节点交换
        if(j + 1 < n && heap[j + 1].priority() < heap[j].priority()) {
            j += 1; 
        }
        //如果比子节点大，需要下行
        if(heap[j].priority() < heap[i].priority()){
            swap(i, j);
            i = j;
            j = 2 * i + 1;
        }else{
            break;
        }
    }
    
}

template<typename T>
void SmallHeap<T>::adjust(size_t i){
    assert(i >= 0 && i < heap.size());
    int j = (i - 1) / 2; // 父节点
    //如果比父节点小，上移，否则下行
    if(j >= 0 && heap[i].priority() < heap[j].priority()){
        up(i);
    } else{
        down(i);
    }
}

template<typename T>
 void SmallHeap<T>::adjust(int id, int priority){
    assert(map.count(id) == 1);
    size_t index = map[id];
    heap[index].priority() = priority;
    adjust(index);
 }

template<typename T>
void SmallHeap<T>::add(const T &t){
    if(map.count(t.id()) == 0){
        heap.push_back(t);
        int index = heap.size() - 1;
        map[t.id()] = index;
        up(index);
    }else{
        adjust(t.id(), t.priority());
    }
}

template<typename T>
void SmallHeap<T>::del(int id){
    assert(map.count(id) == 1);
    int lastIndex = heap.size() - 1;
    int index == map[id];

    if(index == lastIndex){
        heap.pop_back();
        map.erase(id);
        return;
    }

    //用最后一个替代，然后进行调整
    T t = heap[lastIndex];
    heap.pop_back();
    map.erase(id);
    heap[index] = t;
    map[t.id()] = index;

    adjust(index);

}

template<typename T>
size_t SmallHeap<T>::size(){
    return heap.size();
}

template<typename T>
T SmallHeap<T>::front(){
    assert(heap.size() > 0);
    return heap.front();
}

template<typename T>
void SmallHeap<T>::pop(){
    del(front().id());
}

#endif