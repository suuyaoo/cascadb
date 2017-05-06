#include "store/ram_directory.h"

#include "directory_test.h"

class RAMSequenceFileTest : public SequenceFileTest {
public:
    RAMSequenceFileTest()
    {
        dir = new RAMDirectory();
    }
};

TEST_F(RAMSequenceFileTest, read_and_write) {
    TestReadAndWrite();
}

class RAMRandomAccessFileTest : public RandomAccessFileTest {
public:
	RAMRandomAccessFileTest()
    {
        dir = new RAMDirectory();
    }
};

TEST_F(RAMRandomAccessFileTest, blocking_read_and_write)
{
    TestBlockingReadAndWrite();
}

TEST_F(RAMRandomAccessFileTest, read_and_write)
{
    TestReadAndWrite();
}

TEST_F(RAMRandomAccessFileTest, read_partial)
{
    TestReadPartial();
}
