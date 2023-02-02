# LTJ-CompactTries
A compact implementation of the Leapfrog Triejoin algorithm using compact Tries 

Build process
===================
`run release_build.sh`

or rather run :
`cmake -B build/ -D CMAKE_BUILD_TYPE=Release && cmake --build build/`

Index construction
===================
In Linux, define the CLTJ_INDEX_SOURCE_FILE path as follows:

`export CLTJ_INDEX_SOURCE_FILE=<src_file_path>`

and then run 
`./build_index.sh`

Querying the index
==================


In Linux, define the CLTJ_INDEX and CLTJ_QUERIES environment variables.

and then run 
`./build_query.sh`

Important: query ?v1 ?v1 ?v0 is removed from the full query set (queries_full.txt)