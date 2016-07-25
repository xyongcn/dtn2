#include <pthread.h>
#include <list>
using namespace std;
template <typename T>



class BlockingQueue
{
public:
    BlockingQueue ( )
    {
        pthread_mutexattr_init(&_attr);
        // set lock recursive
        pthread_mutexattr_settype(&_attr,PTHREAD_MUTEX_RECURSIVE_NP);
        pthread_mutex_init(&_lock,&_attr);
        pthread_cond_init(&_cond, NULL);
    }
    ~BlockingQueue ( )
    {
        pthread_mutex_destroy(&_lock);
        pthread_cond_destroy(&_cond);
    }
    void push(const T& data);
    bool push(const T& data, const int seconds); //time-out push
    T pop( );
    T pop(const int seconds); // time-out pop
private:
    list<T> _list;
    pthread_mutex_t _lock;
    pthread_mutexattr_t _attr;
    pthread_cond_t _cond;
};
template <typename T>
T BlockingQueue<T>::pop( )
{
    pthread_mutex_lock(&_lock);
    while (_list.empty( ))
    {
        pthread_cond_wait(&_cond, &_lock) ;
    }
    T _temp = _list.front( );
    _list.pop_front( );
    pthread_mutex_unlock(&_lock);
    return _temp;
}
template <typename T>
void BlockingQueue <T>::push(const T& value )
{
    pthread_mutex_lock(&_lock);
    const bool was_empty = _list.empty( );
    _list.push_back(value);
    pthread_mutex_unlock(&_lock);
    if (was_empty)
        pthread_cond_broadcast(&_cond);
}
