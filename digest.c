#include <sqlite3ext.h>
SQLITE_EXTENSION_INIT1

#include <openssl/evp.h>

void sqlite3_digest_type(sqlite3_context *context, int argc, sqlite3_value** argv, const EVP_MD *digest_type)
{
	int i;
	EVP_MD_CTX digest;
	unsigned char *result;
	unsigned int result_length;
	
	result = sqlite3_malloc(EVP_MAX_MD_SIZE);
	
	if (result == NULL)
	{
		sqlite3_result_error_nomem(context);
		return;
	}
	
	EVP_MD_CTX_init(&digest);
	
	EVP_DigestInit_ex(&digest, digest_type, NULL);
	
	for (i = 1; i < argc; i++)
	{
		int size = sqlite3_value_bytes(argv[i]);
		EVP_DigestUpdate(&digest, sqlite3_value_blob(argv[i]), size);
	}
	
	EVP_DigestFinal_ex(&digest, result, &result_length);
	EVP_MD_CTX_cleanup(&digest);
	
	sqlite3_result_blob(context, result, result_length, sqlite3_free);
}

void sqlite3_digest(sqlite3_context *context, int argc, sqlite3_value** argv)
{
	const EVP_MD *digest_type;
	
	if (argc < 2)
	{
		sqlite3_result_error(context, "Insufficient arguments to 'digest' function", -1);
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

void sqlite3_digest_md5(sqlite3_context *context, int argc, sqlite3_value** argv)
{
	sqlite3_digest_type(context, argc, argv, EVP_md5());
}

void sqlite3_digest_sha1(sqlite3_context *context, int argc, sqlite3_value** argv)
{
	sqlite3_digest_type(context, argc, argv, EVP_sha1());
}

int sqlite3_digest_init(sqlite3 *db, char **err, const sqlite3_api_routines *api)
{
	SQLITE_EXTENSION_INIT2(api);
	OpenSSL_add_all_digests();
	sqlite3_create_function(db, "digest", -1, SQLITE_ANY, NULL, sqlite3_digest,      NULL, NULL);
	sqlite3_create_function(db, "sha1",   -1, SQLITE_ANY, NULL, sqlite3_digest_sha1, NULL, NULL);
	sqlite3_create_function(db, "md5",    -1, SQLITE_ANY, NULL, sqlite3_digest_md5,  NULL, NULL);
	return SQLITE_OK;
}

int sqlite3_extension_init(sqlite3 *db, char **err, const sqlite3_api_routines *api)
{
	return sqlite3_digest_init(db, err, api);
}
