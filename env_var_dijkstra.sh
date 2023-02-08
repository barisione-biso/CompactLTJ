(TODO: get rid of this env variable horrible idea.)
Index construction
==================
FILTERED:
export CLTJ_INDEX_SOURCE_FILE=/data/graphs/wikidata/wikidata-filtered.nt.sorted.enumerated-cltj
FULL:
export CLTJ_INDEX_SOURCE_FILE=/data/graphs/wikidata-whole-dump/wikidata.nt.enumerated-daniela * (confirm)

./build/query_index queries_filtered.txt /data/graphs/wikidata/wikidata-filtered.nt.sorted.enumerated-/ 1000 > index_query_wikidata_filtered.txt 

Quering the Index
=================
FILTERED: 
export CLTJ_INDEX=/data/graphs/wikidata/wikidata-filtered.nt.sorted.enumerated-/
export CLTJ_QUERIES=queries_filtered.txt

Running it: 
./build/query_index ${QUERIES_FILE} ${CLTJ_INDEX} 1000 > index_query_wikidata_full.txt 
equivalent to:
./build/query_index queries_full.txt /data/graphs/wikidata/wikidata-filtered.nt.sorted.enumerated-/ 1000 > index_query_wikidata_full.txt 
FULL:
CLTJ_INDEX=/data/graphs/wikidata-whole-dump/wikidata.nt.enumerated-ltj-dan/