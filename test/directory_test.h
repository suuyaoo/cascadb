#ifndef CASCADB_TEST_DIRECTORY_TEST_H_
#define CASCADB_TEST_DIRECTORY_TEST_H_

#include <malloc.h>
#include <map>
#include <gtest/gtest.h>

#include "cascadb/directory.h"
#include "sys/sys.h"

using namespace cascadb;
using namespace std;

class SequenceFileTest : public testing::Test {
public:
    virtual ~SequenceFileTest() {
        delete dir;
    }

    virtual void SetUp() {
        dir->delete_file("sequence_file_test");
    }

    virtual void TearDown() {
        dir->delete_file("sequence_file_test");
    }

    void TestReadAndWrite() {
        char buf[4096];

        SequenceFileWriter* writer = dir->open_sequence_file_writer("sequence_file_test");
        for(int i = 0; i < 1000; i ++) {
            memset(buf, i&0xFF, 4096);
            EXPECT_TRUE(writer->append(Slice(buf, 4096)));
        }
        delete writer;

        SequenceFileReader* reader = dir->open_sequence_file_reader("sequence_file_test");
        for (int i = 0; i < 1000; i++ ) {
            EXPECT_EQ(4096U, reader->read(Slice(buf, 4096)));
            char shouldbe[4096];
            memset(shouldbe, i&0xFF, 4096);
            EXPECT_TRUE(memcmp(buf, shouldbe, 4096) == 0);
        }
        delete reader;       
    }

    Directory                   *dir;
};


class RandomAccessFileTest : public testing::Test {
public:
    virtual ~RandomAccessFileTest() {
        delete dir;
    }

    virtual void SetUp()
    {
        dir->delete_file("aio_file_test");
        file = dir->open_random_access_file("aio_file_test");
    }

    virtual void TearDown()
    {
        delete file;
        dir->delete_file("aio_file_test");
    }

    void TestBlockingReadAndWrite()
    {
        Slice buf = allocbuf(4096);

        for(int i = 0; i < 1000; i ++) {
            memset((void*)buf.data(), i&0xFF, 4096);
            int writelen = file->write(i*4096, buf);
            EXPECT_TRUE(writelen == 4096);
        }

        for (int i = 0; i < 1000; i++ ) {
            int readlen = file->read(i*4096, buf);
            EXPECT_TRUE(readlen == 4096);

            char shouldbe[4096];
            memset(shouldbe, i&0xFF, 4096);
            EXPECT_TRUE(memcmp(buf.data(), shouldbe, 4096) == 0);
        }

        buf.destroy();
    }

    void TestReadAndWrite()
    {
        int     id[1000];
        Slice  buf[1000];

        for (int i = 0; i < 1000; i++ ) {
            id[i] = i;
            buf[i] = allocbuf(4096);
            memset((void*)buf[i].data(), i&0xFF, 4096);
            int writelen = file->write(i * 4096, buf[i]);
			result[i] = writelen;
        }

        while(result.size() != 1000) cascadb::usleep(10000); // 10ms
        for (int i = 0; i < 1000; i++ ) {
            EXPECT_EQ(4096, result[i]);
            buf[i].destroy();
        }
        result.clear();

        for (int i = 0; i < 1000; i++ ) {
            id[i] = i;
            buf[i] = allocbuf(4096);
            int readlen = file->read(i * 4096, buf[i]);
			result[i] = readlen;
        }

        while(result.size() != 1000) cascadb::usleep(10000); // 10ms
        for (int i = 0; i < 1000; i++ ) {
            EXPECT_EQ(4096, result[i]);
            char shouldbe[4096];
            memset(shouldbe, i&0xFF, 4096);
            EXPECT_TRUE(memcmp(buf[i].data(), shouldbe, 4096) == 0);
            buf[i].destroy();
        }
    }

    void TestReadPartial()
    {
        int id = 0;
        Slice buf = allocbuf(4096);
        memset((void*)buf.data(), 0, 4096);
        int writelen = file->write(0, buf);
		result[0] = writelen;
        cascadb::usleep(100000); // wait 100 ms
        EXPECT_EQ(4096, result[0]);
        buf.destroy();
        result.clear();

        buf = allocbuf(8192);
        int readlen = file->read(0, buf);
		result[0] = readlen;

        cascadb::usleep(100000); // wait 100 ms
        EXPECT_EQ(4096, result[0]);
        buf.destroy();
    }

    Slice allocbuf(size_t size)
    {
        void *ptr = memalign(4096, size);
        return Slice((char*)ptr, size);
    }

    Directory                   *dir;
	RandomAccessFile*           file;
    static Mutex                mtx;
    static map<int, int>  result;
};



#endif