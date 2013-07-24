digest.so: digest.c
	gcc digest.c -shared -o digest.so -fPIC -pedantic -Wall -lcrypto -DSQLITE_DIGEST_STANDALONE

tests: digest.so
	sqlite3 -line :memory: '.read tests.sql'

.PHONY: tests
