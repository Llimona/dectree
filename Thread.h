#ifndef __Dectree__Thread__
#define __Dectree__Thread__

#include <thread>

class Thread {
public:
    Thread();
    virtual ~Thread();
    
    void start();
    void join();
    
    virtual void run() = 0;
    
private:
    std::thread  t;
};

#endif /* defined(__Dectree__Thread__) */