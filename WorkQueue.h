#ifndef __Dectree__WorkQueue__
#define __Dectree__WorkQueue__

#include <mutex>
#include <condition_variable>
#include <list>
using namespace std;

template <typename T> class WorkQueue {
    list<T>            queue;
    mutex              mut;
    condition_variable condVar;
    unsigned           openItems;
    
public:
    WorkQueue() {
        //mut = std::mutex();
        //condVar = std::condition_variable();
        openItems = 0;
    }
    
    void add(T item) {
        mut.lock();
        queue.push_back(item);
        ++openItems;
        mut.unlock();
        condVar.notify_one();
    }
    
    /* Blocking while queue is empty but new work items can be expected 
       to be added to the queue by the currently open items.
       Unblocks and sets the return value to NULL if no more work is 
       expected. */
    T remove() {
        unique_lock<mutex> lock(mut);
        condVar.wait(lock, [this]{
            return queue.size() > 0 || openItems == 0;
        });
        T item;
        if (queue.size() > 0) {
            item = queue.front();
            queue.pop_front();
        }
        else {
            item = NULL;
        }
        return item;
    }
    
    size_t size() {
        mut.lock();
        size_t size = queue.size();
        mut.unlock();
        return size;
    }
    
    /* Notifies that one of the removed work items can no longer 
       spawn and add new work items to the queue. */
    void closeItem() {
        mut.lock();
        --openItems;
        mut.unlock();
        condVar.notify_all();
    }
};

#endif /* defined(__Dectree__WorkQueue__) */