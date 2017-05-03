// Copyright (c) 2011 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

// This file is copied from LevelDB and modifed a little 
// to add LevelDB style benchmark

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>

#include "cascadb/db.h"
#include "sys/sys.h"
#include "util/logger.h"

#include "random.h"
#include "testutil.h"
#include "histogram.h"

using namespace std;
using namespace cascadb;

template<typename T>
string to_string(T val)
{
    stringstream ss;
    ss << val;
    return ss.str();
}

int main(int argc, char** argv)
{
    Options opts;
    opts.dir = create_fs_directory(".");
    opts.comparator = new LexicalComparator();
    //opts.compress = kSnappyCompress;

    const char *dbname = "example.db";
    DB *db = DB::open(dbname, opts);
    if (!db) {
      fprintf(stderr, "open error %s\n", dbname);
      exit(1);
    }
    
    size_t count = 1000000;
    
    for (size_t i = 0; i < count; ++i) {
        if (!db->put("This is the key" + to_string(i), "This is the value" + to_string(i))) {
            fprintf(stderr, "put error\n");
        }
    }
    
    for (size_t i = 0; i < count; ++i) {
        Slice value;
        if (!db->get("This is the key" + to_string(i), value)) {
            fprintf(stderr, "get error\n");
        }
        if (value.to_string() != "This is the value" + to_string(i)) {
            fprintf(stderr, "error: the value is %s\n", value.to_string().c_str());
        }
    }
    
    for (size_t i = 0; i < count; ++i) {
        if (!db->del("This is the key" + to_string(i))) {
            fprintf(stderr, "del error\n");
        }
    }
    
    delete db;
    delete opts.comparator;
    delete opts.dir;
    
    return 0;
}
