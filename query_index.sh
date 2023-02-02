if [[ -z "${CLTJ_INDEX}" ]]; then
  echo "Some default value because CLTJ_INDEX is undefined"
  exit 1
else
  SRC_FILE="${CLTJ_INDEX}"
fi

if [[ -z "${CLTJ_QUERIES}" ]]; then
  echo "Some default value because CLTJ_QUERIES is undefined"
  exit 1
else
  QUERIES_FILE="${CLTJ_QUERIES}"
fi
./build/query_index ${QUERIES_FILE} ${CLTJ_INDEX} 1000
exit 0