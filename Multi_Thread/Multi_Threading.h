#pragma once
#include <bits/stdc++.h>

using namespace std;

class Thread
{
public:
    Thread() {}
    virtual ~Thread() { CloseHandle(thread_handle); }
    virtual void start();
    virtual void join();
    virtual DWORD get_id() const { return tid; }
protected:
    virtual void run() = 0;
    friend DWORD thread_ftn(LPVOID T);
    HANDLE thread_handle;
    DWORD tid;
private:
    Thread(const Thread& src);
    Thread& operator=(const Thread& rhs);
};

class MutexClass
{
public:
    MutexClass();
    virtual ~MutexClass();
    virtual void lock_mutex();
    virtual void unlock_mutex();
protected:
    HANDLE mutex;
};

class Producer : public Thread
{
protected:
    virtual void run();
};

class Consumer : public Thread
{
protected:
    virtual void run();
};