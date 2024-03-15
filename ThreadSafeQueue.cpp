#include <iostream>
#include <mutex>
#include <thread>
#include <queue>

template <typename T> class threadSafeQueue {
    std::queue<T> rawQueue{} ;
    std::mutex m{} ;

    public:
        T returnAndDelete() {
            T front_value{};
            m.lock() ;
            if(!rawQueue.empty() ) {
                front_value=rawQueue.front() ;
                rawQueue.pop() ;
            }
            m.unlock() ;
            return front_value ;
        }


    void push(T val) {
        m.lock() ;
        rawQueue.push(val) ;
        m.unlock() ;
    }

    void printAndDelete() {
        m.lock() ;
        while(!rawQueue.empty() ) std::cout << returnAndDelete() << " " ;
        std::cout << "\n" ;
        m.unlock() ;
    }

    bool isEmpty() {
     return rawQueue.empty() ;
    }

} ;

template <typename T> class QueueTest {
    public:

    void operator() (threadSafeQueue<T> &q,int nAdd,int mDelete) {

        for(int i{} ; i!=nAdd ; ++i) {
           std::cout << i << " added\n" ;
           q.push(i) ;
        }

        for(int i{} ; i!=mDelete && !q.isEmpty() ; ++i ) {
                std::cout << q.returnAndDelete() <<" deleted\n" ;
        }
        std::cout << "\n\n" ;
    }
} ;


int main(int argc, char **argv)
{
    threadSafeQueue<int> obj{} ;
    QueueTest<int> test{} ;

    std::thread thread1(test,std::ref(obj),11,3) ;
    std::thread thread2(test,std::ref(obj),13,5 )  ;
    std::thread thread3(test,std::ref(obj),15,4 ) ;

    thread1.join() ;
    thread2.join() ;
    thread3.join() ;

    obj.printAndDelete() ;

    return 0;
}
