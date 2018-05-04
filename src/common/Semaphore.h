#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include <mutex>
#include <condition_variable>

namespace util {

class Semaphore {
public:
    Semaphore(int value=0): m_count{value} {}

    void wait()
    {
        std::unique_lock<std::mutex> lock{m_mutex};
        if (--m_count < 0) // count is not enough ?
        {
            m_condition.wait(lock); // suspend and wait...
        }
    }

    bool tryWait()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (m_count > 0) // count is not enough ?
        {
            m_count--;
            return true;
        }
        else
        {
            return false;
        }
    }

    void post(unsigned int n = 1)
    {
        std::lock_guard<std::mutex> lock{m_mutex};
        while (n-- > 0)
        {
            if(++m_count <= 0) // have some thread suspended ?
            {
                m_condition.notify_one(); // notify one !
            }
        }
    }

private:
    int m_count;
    std::mutex m_mutex;
    std::condition_variable m_condition;
};

} // namespace util

#endif // SEMAPHORE_H
