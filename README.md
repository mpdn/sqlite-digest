SQLite-digest
=============
Message digest extension for SQLite3 using OpenSSL. Released under the MIT license (see LICENSE for more information).

SQLite-digest provides functions for computing hashes in SQLite: `md5`, `sha1`, `sha256`, `sha512` and `digest`.

The `digest` function takes a OpenSSL digest name as the first argument and one or more values to digest. You can see *some* of the available digest names by installing the OpenSSL command line tool and executing `openssl list -digest-algorithms`. For OpenSSL 1.1.0f it is `BLAKE2b512`, `BLAKE2s256`, `MD4`, `MD5`, `MD5-SHA1`, `RIPEMD160`, `RSA-SHA1`, `SHA1`, `SHA224`, `SHA256`, `SHA384`, `SHA512`, `whirlpool`.

Functions named `sha1` and `md5` are identical to calling the `digest` function with respectively "sha1" and "md5" as the first argument. See examples below.

The returned digest is always a blob, so it might be necessary to use the `hex` function to get the string representation. Bear in mind that the `hex` function returns hexadecimal in upper case, whereas most hashes are represented with lower case hexadecimal.

All functions are variadic (i.e. they can take any number of arguments). If called with more than one argument, the digest will be calculated as if their binary values were concatenated.

SQL Example
-------
```sql
.load ./digest.so
SELECT hex(digest("sha1", "abc")); -- returns "A9993E364706816ABA3E25717850C26C9CD0D89D"
SELECT hex(sha1("abc"));           -- identical to the above function

SELECT hex(digest("md5", "abc"));  -- returns "900150983CD24FB0D6963F7D28E17F72"
SELECT hex(md5("abc"));            -- identical to the above function
SELECT hex(md5("a", "b", "c"));    -- also identical
```

Tested compatibility
-------------

OpenSSL v1.1

SQLite v3.16

