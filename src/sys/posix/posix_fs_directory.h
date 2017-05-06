// Copyright (c) 2013 The CascaDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#ifndef CASCADB_SYS_POSIX_POSIX_FS_DIRECTORY_H_
#define CASCADB_SYS_POSIX_POSIX_FS_DIRECTORY_H_

#include "store/fs_directory.h"

namespace cascadb {

class PosixFSDirectory : public FSDirectory {
public:
    PosixFSDirectory(const std::string& path) : FSDirectory(path) {}

    virtual ~PosixFSDirectory();

    virtual bool file_exists(const std::string& filename);

    virtual SequenceFileReader* open_sequence_file_reader(const std::string& filename);

    virtual SequenceFileWriter* open_sequence_file_writer(const std::string& filename);

    virtual RandomAccessFile* open_random_access_file(const std::string& filename);

    virtual size_t file_length(const std::string& filename);

protected:
    virtual const std::string fullpath(const std::string& filename);
};

}

#endif