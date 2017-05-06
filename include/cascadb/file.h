// Copyright (c) 2013 The CascaDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#ifndef CASCADB_FILE_H_
#define CASCADB_FILE_H_

#include "slice.h"

namespace cascadb {

class SequenceFileReader {
public:
    SequenceFileReader() {}
    virtual ~SequenceFileReader() {}

    // read a number of bytes from the end of file into buffer,
    // The function will block until data is ready,
    // Return number of bytes read.
    virtual size_t read(Slice buf) = 0;

    // skip a number of bytes
    virtual bool skip(size_t n) = 0;

    virtual void close() = 0;

private:
    SequenceFileReader(const SequenceFileReader&);
    void operator=(const SequenceFileReader&);   
};

class SequenceFileWriter {
public:
    SequenceFileWriter() {}
    virtual ~SequenceFileWriter() {}

    // append buffer to the end of file,
    // The function will block until data is ready,
    // Return true if written successfully
    virtual bool append(Slice buf) = 0;
    virtual bool flush() = 0;
    virtual void close() = 0;

private:
    SequenceFileWriter(const SequenceFileWriter&);
    void operator=(const SequenceFileWriter&);   
};

class RandomAccessFile {
public:
	RandomAccessFile() {}
    virtual ~RandomAccessFile() {}

    // blocking wrapper to async read
    virtual int read(uint64_t offset, Slice buf) = 0;

    // blocking wrapper to async write
    virtual int write(uint64_t offset, Slice buf) = 0;

	virtual void truncate(uint64_t offset) { }

    virtual void close() = 0;

private:
	RandomAccessFile(const RandomAccessFile&);
    void operator=(const RandomAccessFile&);
};

}

#endif