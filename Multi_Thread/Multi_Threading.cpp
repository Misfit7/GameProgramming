#include "Multi_Threading.h"

using namespace std;

DWORD thread_ftn(LPVOID T) {
    Thread* t = static_cast<Thread*>(T);
    t->run();
    return NULL;
}

void Thread::start() {
    thread_handle = CreateThread(
        NULL, // default security
        0, // default stack size
        (LPTHREAD_START_ROUTINE)&thread_ftn, // thread function name
        (LPVOID)this, // argument to thread function
        0, // use default creation flags
        &tid);
}

void Thread::join() {
    WaitForSingleObject(thread_handle, INFINITE);
}

MutexClass::MutexClass() {
    mutex = CreateMutex(
        NULL, // default security
        FALSE, // initially not owned
        NULL); // unamed mutex
}

MutexClass::~MutexClass() {
    CloseHandle(mutex);
}

void MutexClass::lock_mutex() {
    DWORD wait_result;
    wait_result = WaitForSingleObject(
        mutex, // handle to mutex
        INFINITE); // no time-out interval
}

void MutexClass::unlock_mutex() {
    ReleaseMutex(mutex);
}

MutexClass mut;
vector<string> buffer;
BOOL done = FALSE;

void Producer::run() {
    mut.lock_mutex();
    Sleep(5000);
    buffer.push_back("Hello from Producer\n");
    mut.unlock_mutex();
}

void Consumer::run() {

    while (!done) {
        mut.lock_mutex();
        if (buffer.size() > 0) {
            std::cout << "got msg: " << buffer.front() << "\n";
            done = TRUE;
            buffer.pop_back();
        }
        mut.unlock_mutex();
    }
}