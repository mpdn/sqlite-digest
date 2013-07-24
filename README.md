SQLite-digest
=============
Message digest extension for SQLite3 using OpenSSL. Released under the MIT license (see LICENSE for more information).

SQLite-digest provides three functions for computing hashes in sqlite: `digest`, `sha1` and `md5`.

The `digest` function takes a OpenSSL digest name as the first argument and one or more values to digest. You can see *some* of the available digest names by installing the OpenSSL command line tool and executing `openssl list-message-digest-commands`. `sha1` and `md5` are identical to calling the `digest` function with respectively "sha1" and "md5" as the first argument.

The returned digest is always a blob, so it might be neccesary to use the `hex` function to get the string representation. Bear in mind that the `hex` function returns hexdecimal in upper case, whereas most hashes are represented with lower case hexdecimal.

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
