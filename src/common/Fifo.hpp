#ifndef TEMPLATE_FIFO_H
#define TEMPLATE_FIFO_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

namespace util {

// 内存屏障
#if 0
# define MEMORY_BARRIER __asm__ __volatile__("": : :"memory")
#else
# define MEMORY_BARRIER __sync_synchronize()
#endif

template<typename T>
class Fifo
{
public:
    Fifo() {
        m_esize = sizeof(T);
        pthread_spin_init(&m_inLock, 0);
        pthread_spin_init(&m_outLock, 0);
    }
    ~Fifo() {
        uninit();
        pthread_spin_destroy(&m_inLock);
        pthread_spin_destroy(&m_outLock);
    }

    bool init(size_t n) {
        uninit();

        n = roundup_pow_of_two(n);
        m_mask = n - 1;
        m_data = malloc(n * m_esize);
        if (nullptr == m_data) {
            return false;
        }

        return true;
    }

    void uninit() {
        if (nullptr == m_data) {
            return;
        }
        free(m_data);
        m_esize = sizeof(T);
        m_data  = nullptr;
        m_mask  = 0;
        m_in    = 0;
        m_out   = 0;
    }

    bool put(const T& data) {
        if (is_full()) {
            return false;
        }

        T* ptr = (T*)m_data;
        ptr += m_in & m_mask;
        memcpy(ptr, &data, sizeof(data));

        MEMORY_BARRIER;

        m_in++;

        return true;
    }

    bool putSpinlocked(const T& data) {
        pthread_spin_lock(&m_inLock);
        bool ret = put(data);
        pthread_spin_unlock(&m_inLock);
        return ret;
    }

    bool get(T& data) {
        if (is_empty()) {
            return false;
        }

        T* ptr = (T*)m_data;
        ptr += m_out & m_mask;
        memcpy(&data, ptr, sizeof(data));

        MEMORY_BARRIER;
        m_out++;

        return true;
    }

    bool getSpinlocked(T& data) {
        pthread_spin_lock(&m_outLock);
        bool ret = get(data);
        pthread_spin_unlock(&m_outLock);
        return ret;
    }

    size_t length() const { return m_in - m_out; }
    size_t capacity() const { return m_mask + 1; }

private:
    unsigned long roundup_pow_of_two(unsigned long val) {
        if((val & (val-1)) == 0) return val;

        unsigned long maxulong = (unsigned long)((unsigned long)~0);
        unsigned long andv = ~(maxulong&(maxulong>>1));
        while((andv & val) == 0) andv = andv>>1;

        return andv<<1;
    }

    inline bool is_full() {
        return (m_in - m_out) == (m_mask + 1);
    }

    inline bool is_empty() {
        return m_in == m_out;
    }

private:
    void*  m_data  = nullptr;
    size_t m_esize = 0;
    size_t m_mask  = 0;
    size_t m_in    = 0;
    size_t m_out   = 0;

    pthread_spinlock_t m_inLock;
    pthread_spinlock_t m_outLock;
};

#undef MEMORY_BARRIER

}// namespace util

#endif // TEMPLATE_FIFO_H
