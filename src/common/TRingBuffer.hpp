#ifndef TRINGBUFFER_H
#define TRINGBUFFER_H

#include <stdint.h>
#include <vector>
#include <atomic>

namespace util {

template<typename T>
class TRingBuffer
{
    // disabled
    TRingBuffer();
    void operator = (const TRingBuffer& a);

public:
    explicit TRingBuffer(uint32_t num = 1024) {
        uint32_t mask = 0;
        for (int i=0; i<32; i++) {
            mask = mask<<1;
            mask |= 1;
            if (0 == ((~mask) & num)) break;
        }
        m_mask = mask;
        m_vct.resize(mask+1);
    }

    ~TRingBuffer() {
        m_vct.clear();
    }

    T* getForWrite() {
        if (m_isWriting) return nullptr;
        if (m_wIndex - m_rIndex == m_mask + 1) return nullptr;
        m_isWriting = true;
        return &m_vct[m_wIndex & m_mask];
    }

    void writeFinished() {
        if (!m_isWriting) return;
        m_wIndex++;
        m_isWriting = false;
    }

    T* getForRead() {
        if (m_isReading) return nullptr;
        if (m_wIndex == m_rIndex) return nullptr;
        m_isReading = true;
        return &m_vct[m_rIndex & m_mask];
    }

    void readFinished() {
        if (!m_isReading) return;
        m_rIndex++;
        m_isReading = false;
    }

    bool isEmpty() const { return (m_wIndex == m_rIndex); }
    uint32_t size() const { return m_wIndex - m_rIndex; }
    uint32_t capacity() const { return m_mask+1; }

private:
    uint32_t m_mask; // 2^n-1

    std::vector<T> m_vct;
    uint32_t m_wIndex = 0; // 入队位置
    uint32_t m_rIndex = 0; // 出队位置

    bool m_isWriting = false;
    bool m_isReading = false;
};

} // namespace util

#endif // TRINGBUFFER_H
