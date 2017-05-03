add_executable(
    db_sample
    sample.cpp
    testutil.cpp
    histogram.cpp)
target_link_libraries(
    db_sample
    cascadbShared
    ${CASCADB_LIBS}
    )

