if [[ -z "${CLTJ_INDEX_SOURCE_FILE}" ]]; then
  echo "Some default value because CLTJ_INDEX_SOURCE_FILE is undefined"
  exit 1
else
  SRC_FILE="${CLTJ_INDEX_SOURCE_FILE}"
fi

./build/build_index ${CLTJ_INDEX_SOURCE_FILE}
exit 0