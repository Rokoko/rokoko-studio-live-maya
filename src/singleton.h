#ifndef SINGLETON_H
#define SINGLETON_H

template<class T>
class Singleton
{
public:
    static T* getInstance() {
        static T instance;
        return &instance;
    }
private:
    Singleton()=default;
    Singleton(const Singleton&)=delete;
    ~Singleton()=default;
    Singleton&operator=(const Singleton&)=delete;
};

#endif // SINGLETON_H
