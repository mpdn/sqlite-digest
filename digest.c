#include <openssl/evp.h>
#include <sqlite3ext.h>
SQLITE_EXTENSION_INIT1

/*
Uses the message digest type specified by digest_type to create a digest of any
sqlite3 values specified by argc and argv
*/
void sqlite3_digest_type(sqlite3_context *context, int argc, sqlite3_value** argv, const EVP_MD *digest_type)
{
	int i;
	EVP_MD_CTX *digest;
	unsigned char *result;
	unsigned int result_length;
	
	if (argc < 1)
	{
		sqlite3_result_error(context, "Insufficient arguments to digest function", -1);
		return;
	}
	
	result = sqlite3_malloc(EVP_MAX_MD_SIZE);
	
	if (result == NULL)
	{
		sqlite3_result_error_nomem(context);
		return;
	}

	digest = EVP_MD_CTX_new();
	EVP_MD_CTX_init(digest);
	
	if (!EVP_DigestInit_ex(digest, digest_type, NULL))
	{
		sqlite3_result_error(context, "Failed intializing digest", -1);
		return;
	}
	
	for (i = 0; i < argc; i++)
	{
		int size = sqlite3_value_bytes(argv[i]);
		EVP_DigestUpdate(digest, sqlite3_value_blob(argv[i]), size);
	}
	
	if (!EVP_DigestFinal_ex(digest, result, &result_length))
	{
		sqlite3_result_error(context, "Failed finilazing digest", -1);
		return;
	}
	
	EVP_MD_CTX_free(digest);
	
	sqlite3_result_blob(context, result, result_length, sqlite3_free);
}

/*
Interprets the first parameter as the name of the digest type, and digests the
remaining parameters using that digest type.
*/
void sqlite3_digest(sqlite3_context *context, int argc, sqlite3_value** argv)
{
	const EVP_MD *digest_type;
	
	if (argc < 1)
	{
		sqlite3_result_error(context, "Insufficient arguments to digest function", -1);
		return;
	}
	
	digest_type = EVP_get_digestbyname((const char*)sqlite3_value_text(argv[0]));
	
	if (digest_type == NULL)
	{
		sqlite3_result_error(context, "Unrecognized digest type", -1);
		return;
	}
	
	sqlite3_digest_type(context, argc - 1, argv + 1, digest_type);
}

/*
For list of EVP_* see OpenSSL docs: https://www.openssl.org/docs/man1.1.0/man3/EVP_DigestInit.html
*/

/*
Digests a series of sqlite3 values using MD5
*/
void sqlite3_digest_md5(sqlite3_context *context, int argc, sqlite3_value** argv)
{
	sqlite3_digest_type(context, argc, argv, EVP_md5());
}

/*
Digests a series of sqlite3 values using SHA-1
*/
void sqlite3_digest_sha1(sqlite3_context *context, int argc, sqlite3_value** argv)
{
	sqlite3_digest_type(context, argc, argv, EVP_sha1());
}

/*
Digests a series of sqlite3 values using SHA-256
*/
void sqlite3_digest_sha256(sqlite3_context *context, int argc, sqlite3_value** argv)
{
        sqlite3_digest_type(context, argc, argv, EVP_sha256());
}

/*
Digests a series of sqlite3 values using SHA-512
*/
void sqlite3_digest_sha512(sqlite3_context *context, int argc, sqlite3_value** argv)
{
        sqlite3_digest_type(context, argc, argv, EVP_sha512());
}

/*
Initializes the digest functions
*/
int sqlite3_digest_init(sqlite3 *db, char **err, const sqlite3_api_routines *api)
{
	SQLITE_EXTENSION_INIT2(api);
	OpenSSL_add_all_digests();
	sqlite3_create_function(db, "digest", -1, SQLITE_ANY, NULL, sqlite3_digest,      NULL, NULL);
	sqlite3_create_function(db, "sha1",   -1, SQLITE_ANY, NULL, sqlite3_digest_sha1, NULL, NULL);
	sqlite3_create_function(db, "sha256", -1, SQLITE_ANY, NULL, sqlite3_digest_sha256,  NULL, NULL);
	sqlite3_create_function(db, "sha512", -1, SQLITE_ANY, NULL, sqlite3_digest_sha512,  NULL, NULL);
	sqlite3_create_function(db, "md5",    -1, SQLITE_ANY, NULL, sqlite3_digest_md5,  NULL, NULL);
	return SQLITE_OK;
}

#ifdef SQLITE_DIGEST_STANDALONE
int sqlite3_extension_init(sqlite3 *db, char **err, const sqlite3_api_routines *api)
{
	return sqlite3_digest_init(db, err, api);
}
#endif
