#include <stdio.h>
#include <time.h>
#include <unistd.h>

#include <iostream>
#include <thread>
#include "common/TBuffer.hpp"
#include "common/TRingBuffer.hpp"
#include "common/Fifo.hpp"

typedef struct
{
    int id;
    char txt[1024];
} TestStruct_t;

void getRandString(char* dst, int max)
{
    static char table[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789~!@#$%^&*()_+-=/\\|'\",./<>?";
    int len = rand()%max + 1;
    for (int i=0; i<len; i++) {
        dst[i] = table[rand()%(sizeof(table)-1)];
    }
    dst[len] = 0;
}

void testFifo()
{
    util::Fifo<TestStruct_t> fifo;
    fifo.init(1024);
    bool exitFlag = false;

    int errput = 0;
    int errget = 0;
    std::thread thread([&exitFlag, &fifo, &errget] {
        TestStruct_t data;
        while (true) {
#if 1
            if (fifo.get(data)) {
                printf("[%d] %s\n", data.id, data.txt);
            } else {
                if (exitFlag) {
                    break;
                }
                errget++;
                usleep(1);
            }
#else
            fifo.getSpinlocked(data);
            printf("[%d] %s\n", data.id, data.txt);
#endif
        }
    });

    TestStruct_t data;

    data.id = 0;
    srand(time(nullptr));
    for (int i=0; i<10000; i++) {
        data.id++;
        getRandString(data.txt, 100);
        while (!fifo.put(data)) {
            errput++;
            usleep(2);
        }
    }
    exitFlag = true;
    thread.join();

    fifo.uninit();

    printf("put failed: %d,  get failed: %d\n", errput, errget);
}

void testTBuffer()
{
    util::TBuffer<TestStruct_t> buffer;
    buffer.init(1024, false);

    bool exitFlag = false;
    int errput = 0;
    int errget = 0;
    std::thread thread([&exitFlag, &buffer, &errget] {
        while (true) {
            TestStruct_t* d = buffer.getForRead();
            if (nullptr != d) {
                printf("[%d] %s\n", d->id, d->txt);
                buffer.readFinished(d);
            } else {
                if (exitFlag) {
                    break;
                }
                errget++;
                usleep(1);
            }
        }
    });

    TestStruct_t *data = nullptr;
    srand(time(nullptr));
    for (int i=0; i<10000; i++) {
        while (true) {
            data = buffer.getForWrite();
            if (nullptr == data) {
                errput++;
                usleep(2);
            } else {
                break;
            }
        };

        data->id = i+1;
        getRandString(data->txt, 100);
        buffer.writeFinished(data);
        data = nullptr;
    }
    exitFlag = true;
    thread.join();

    buffer.uninit();

    printf("put failed: %d,  get failed: %d\n", errput, errget);
}

void testTRingBuffer()
{
    util::TRingBuffer<TestStruct_t> buffer(1024);

    bool exitFlag = false;
    int errput = 0;
    int errget = 0;
    std::thread thread([&exitFlag, &buffer, &errget] {
        while (true) {
            TestStruct_t* d = buffer.getForRead();
            if (nullptr != d) {
                printf("[%d] %s\n", d->id, d->txt);
                buffer.readFinished();
            } else {
                if (exitFlag) {
                    break;
                }
                errget++;
                usleep(1);
            }
        }
    });

    TestStruct_t *data = nullptr;
    srand(time(nullptr));
    for (int i=0; i<10000; i++) {
        while (true) {
            data = buffer.getForWrite();
            if (nullptr == data) {
                errput++;
                usleep(2);
            } else {
                break;
            }
        };

        data->id = i+1;
        getRandString(data->txt, 100);
        buffer.writeFinished();
        data = nullptr;
    }
    exitFlag = true;
    thread.join();

    printf("put failed: %d,  get failed: %d\n", errput, errget);
}


int main()
{
    //testFifo();
    testTBuffer();
    //testTRingBuffer();

    return 0;
}
