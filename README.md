sqlite-digest
=============
Message digest extension for SQLite3 using OpenSSL.

The `digest` function takes a OpenSSL digest name as the first parameter and one or more values to digest. You can see *some* of the available digest names by installing the OpenSSL command line tool and executing `openssl list-message-digest-commands`.

Two other functions are also available: `sha1` and `md5`. These are identical to calling the `digest` function with respectively "sha1" and "md5" as the first parameter.

The returned digest is always a blob, so it might be neccesary to use the `hex` function to get the string representation. Bear in mind that the `hex` function returns hexdecimal in upper case, whereas most hashes are represented with lower case hexdecimal.

All functions are variadic (i.e. they can take any number of parameters). If called with more than one argument, the digest will be calculated as if their binary values were concatenated.

Example
-------
```sql
SELECT hex(digest("sha1", "abc")); -- returns "A9993E364706816ABA3E25717850C26C9CD0D89D"
SELECT hex(sha1("abc"));           -- identical to the above function

SELECT hex(digest("md5", "abc"));  -- returns "900150983CD24FB0D6963F7D28E17F72"
SELECT hex(md5("abc"));            -- identical to the above function
SELECT hex(md5("a", "b", "c"));    -- also identical
```
