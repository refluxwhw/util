#include "CharFifo.h"

#include <string.h>
#include <iostream>

namespace util {

CharFifo::CharFifo(int nSize)
{
    nSize = roundup_power_of_two(nSize);
    m_mask = nSize - 1;
    pthread_spin_init(&m_inLock, 0);
    pthread_spin_init(&m_outLock, 0);
}

CharFifo::~CharFifo()
{
    if(nullptr != m_buffer) {
        delete[] m_buffer;
        m_buffer = nullptr;
    }
    pthread_spin_destroy(&m_inLock);
    pthread_spin_destroy(&m_outLock);
}

bool CharFifo::init()
{
    m_buffer = new char[m_mask+1];
    if (!m_buffer) {
        return false;
    }

    m_in = m_out = 0;

    return true;
}

unsigned long CharFifo::roundup_power_of_two(unsigned long val)
{
    if((val & (val-1)) == 0) return val;

    unsigned long maxulong = (unsigned long)((unsigned long)~0);
    unsigned long andv = ~(maxulong&(maxulong>>1));

    while((andv & val) == 0) andv = andv>>1;

    return andv<<1;
}

size_t CharFifo::put(const char *buffer, size_t len, bool full)
{
    size_t left = leftSize();
    if (full && left < len) {
        return 0;
    }

    size_t l;
    len = std::min(len, left);

    /* first put the data starting from fifo->in to buffer end */
    l = std::min(len, m_mask + 1 - (m_in  & m_mask));
    memcpy(m_buffer + (m_in & m_mask), buffer, l);

    /* then put the rest (if any) at the beginning of the buffer */
    memcpy(m_buffer, buffer + l, len - l);

    __sync_synchronize();

    m_in += len;

    return len;
}

size_t CharFifo::putSpinlocked(const char *buffer, size_t len, bool full)
{
    pthread_spin_lock(&m_inLock);
    len = put(buffer, len, full);
    pthread_spin_unlock(&m_inLock);
    return len;
}

size_t CharFifo::get(char *buffer, size_t len, bool full)
{
    size_t l_ = getDataLen();
    if (full && l_ < len) {
        return 0;
    }

    size_t l;
    len = std::min(len, l_);

    /* first get the data from fifo->out until the end of the buffer */
    l = std::min(len, m_mask + 1 - (m_out & m_mask));
    memcpy(buffer, m_buffer + (m_out & m_mask), l);

    /* then get the rest (if any) from the beginning of the buffer */
    memcpy(buffer + l, m_buffer, len - l);

    __sync_synchronize();

    m_out += len;

    return len;
}

size_t CharFifo::getSpinlocked(char *buffer, size_t len, bool full)
{
    pthread_spin_lock(&m_outLock);
    len = get(buffer, len, full);
    pthread_spin_unlock(&m_outLock);
    return len;
}

} // namespace util
