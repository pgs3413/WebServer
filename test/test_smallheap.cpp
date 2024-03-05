#include "../src/queue/smallheap.h"
#include<iostream>


struct Item {

    int id_;
    int priority_;

    int id(){
        return id_;
    }

    int & priority(){
        return priority_;
    }

};

int main(){

    SmallHeap<Item> heap;

    heap.add({1,1});
    heap.add({2,2});
    heap.add({100,100});
    heap.add({3,3});
    heap.add({4,4});
    heap.add({101,101});
    heap.add({102,102});
    heap.add({5,5});
    heap.add({6,6});
    heap.add({7,7});
    heap.add({8,8});
    heap.add({103,103});
    heap.add({104,104});
    heap.add({105,105});
    heap.add({106,106});
    heap.add({9,100});

    heap.adjust(8, 105);

    heap.del(101);

    int size = heap.size();

    for(int i = 0; i < size; i++){
        std::cout << heap.front().id_ << std::endl;
        heap.pop();
    }

    return 0;

}