#include "sys/posix/posix_fs_directory.h"

#include "directory_test.h"

class PosixSequenceFileTest : public SequenceFileTest {
public:
    PosixSequenceFileTest()
    {
        dir = new PosixFSDirectory("/tmp");
    }
};

TEST_F(PosixSequenceFileTest, read_and_write) {
    TestReadAndWrite();
}

class PosixRandomAccessFileTest : public RandomAccessFileTest {
public:
	PosixRandomAccessFileTest()
    {
        dir = new PosixFSDirectory("/tmp");
    }
};

TEST_F(PosixRandomAccessFileTest, blocking_read_and_write)
{
    TestBlockingReadAndWrite();
}

TEST_F(PosixRandomAccessFileTest, read_and_write)
{
    TestReadAndWrite();
}

TEST_F(PosixRandomAccessFileTest, read_partial)
{
    TestReadPartial();
}
