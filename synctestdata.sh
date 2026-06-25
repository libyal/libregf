#!/bin/sh
# Script that synchronizes the local test data

TESTS_INPUT_DIRECTORY="tests/input"
TEST_SET="public"
TEST_FILES="NTUSER.DAT SOFTWARE SYSTEM"

mkdir -p "${TESTS_INPUT_DIRECTORY}/.regfinfo_hierarchy"
echo "-H" > "${TESTS_INPUT_DIRECTORY}/.regfinfo_hierarchy/options"

mkdir -p "${TESTS_INPUT_DIRECTORY}/${TEST_SET}"

for TEST_FILE in ${TEST_FILES}
do
	URL="https://raw.githubusercontent.com/log2timeline/dfwinreg/refs/heads/main/test_data/${TEST_FILE}"

	curl -L -o "${TESTS_INPUT_DIRECTORY}/${TEST_SET}/${TEST_FILE}" ${URL}
done
