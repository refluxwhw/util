#ifndef TBUFFER_H
#define TBUFFER_H

#include <list>
#include <map>
#include <mutex>

namespace util {

template<typename T>
class TBuffer {
public:
    TBuffer() {}
    bool init(unsigned int num, bool multiThread = false) {
        uninit();
        m_multiThread = multiThread;
        m_size = num;
        m_d = new T[num];
        for (unsigned int i=0; i<m_size; i++) {
            m_write.push_back(&m_d[i]);
        }
        return true;
    }
    void uninit() {
        if (m_multiThread) {
            m_wLock.lock();
            m_rLock.lock();
        }
        if (nullptr != m_d) {
            delete [] m_d;
            m_d = nullptr;
        }
        m_write.clear();
        m_read.clear();
        m_writing.clear();
        m_reading.clear();
        m_size = 0;
        if (m_multiThread) {
            m_rLock.unlock();
            m_wLock.unlock();
        }
    }
    void clear() {
        if (m_multiThread) {
            m_wLock.lock();
            m_rLock.lock();
        }
        m_write.clear();
        m_read.clear();
        m_writing.clear();
        m_reading.clear();
        for (unsigned int i=0; i<m_size; i++) {
            m_write.push_back(&m_d[i]);
        }
        if (m_multiThread) {
            m_rLock.unlock();
            m_wLock.unlock();
        }
    }

    T* getForWrite() {
        if (m_multiThread) {
            m_wLock.lock();
        }
        T* d = nullptr;
        if (!m_write.empty()) {
            d = m_write.front();
            m_writing.insert(std::make_pair(d,0));
            m_write.pop_front();
        }
        if (m_multiThread) {
            m_wLock.unlock();
        }
        return d;
    }
    bool writeFinished(T* d) {
        if (m_multiThread) {
            m_wLock.lock();
            m_rLock.lock();
        }
        bool ret = false;
        auto itr = m_writing.find(d);
        if (itr != m_writing.end()) {
            m_writing.erase(itr);
            m_read.push_back(d);
            ret = true;
        }
        if (m_multiThread) {
            m_rLock.unlock();
            m_wLock.unlock();
        }
        return ret;
    }

    T* getForRead() {
        if (m_multiThread) {
            m_rLock.lock();
        }
        T* d = nullptr;
        if (!m_read.empty()) {
            d = m_read.front();
            m_reading.insert(std::make_pair(d,0));
            m_read.pop_front();
        }
        if (m_multiThread) {
            m_rLock.unlock();
        }
        return d;
    }
    bool readFinished(T* d) {
        if (m_multiThread) {
            m_wLock.lock();
            m_rLock.lock();
        }
        bool ret = false;
        auto itr = m_reading.find(d);
        if (itr != m_reading.end()) {
            m_reading.erase(itr);
            m_write.push_back(d);
            ret = true;
        }
        if (m_multiThread) {
            m_rLock.unlock();
            m_wLock.unlock();
        }
        return ret;
    }


private:
    T* m_d = nullptr;
    unsigned int m_size = 0;
    std::list<T*> m_write;
    std::map<T*,int> m_writing;
    std::list<T*> m_read;
    std::map<T*,int> m_reading;

    bool m_multiThread = false;
    std::mutex m_rLock;
    std::mutex m_wLock;
};

} // namespace util

#endif // TBUFFER_H
