digest.so: digest.c
	$(CC) digest.c -shared -o digest.so -fPIC -pedantic -Wall -lcrypto -DSQLITE_DIGEST_STANDALONE $(CFLAGS) $(CPPFLAGS) $(LDFLAGS)

tests: digest.so
	sqlite3 -line :memory: '.read tests.sql'

clean:
	rm -f digest.so

.PHONY: tests clean
