#include "db.h"

std::unique_ptr<IDatabaseAPI> create_db()
{
        return std::make_unique<MongoDB>();
}


MongoDB::MongoDB()
{
	mongoc_init ();		
	
	init_collection(user_collection_name);
	init_collection(room_collection_name);
}


MongoDB::~MongoDB()
{
	mongoc_cleanup ();
}	


bool MongoDB::insert_new_user(DBUser& user)
{
	std::unique_lock<std::mutex> lk(mx_query);

	mongoc_client_t* client = mongoc_client_new ("mongodb://localhost:27017/?appname=chat_insert_new_user");
	mongoc_collection_t *collection_users = mongoc_client_get_collection (client, database_name.c_str(), user_collection_name.c_str());

	bson_error_t error;
	bson_oid_t oid;
	bson_t *doc;	
	
	doc = BCON_NEW(
		"name", BCON_UTF8(user.name.c_str()),
		"password", BCON_UTF8(user.password.c_str())
		);
				
	bool result = mongoc_collection_insert_one (collection_users, doc, NULL, NULL, &error);
	 
	bson_destroy (doc);
        mongoc_collection_destroy (collection_users);
	mongoc_client_destroy (client);

	if (!result)
	{
    	        std::cout << "ERROR: " << error.message << std::endl;
	}
	return result;
}
	

bool MongoDB::remove_user(const std::string& name)
{
	std::unique_lock<std::mutex> lk(mx_query);

	mongoc_client_t* client = mongoc_client_new ("mongodb://localhost:27017/?appname=chat_remove_user");
	mongoc_collection_t *collection_users = mongoc_client_get_collection (client, database_name.c_str(), user_collection_name.c_str());

	bson_error_t error;
	bson_t filter;
	bson_init(&filter);
	BSON_APPEND_UTF8(&filter, "name", name.c_str());

	bool result = mongoc_collection_delete_one (collection_users, &filter, NULL, NULL, &error);	
	      
        bson_destroy(&filter);
        mongoc_collection_destroy (collection_users);
        mongoc_client_destroy (client);
   
        if (!result)
		std::cout << "ERROR: " << error.message << std::endl;			

        return result;
}


json_string MongoDB::get_user_data(const std::string& user_name)
{
	std::unique_lock<std::mutex> lk(mx_query);	
	
	mongoc_cursor_t *cursor;
	const bson_t *doc;
	bson_t *query;
	char *str;
	std::string str_result;
	
	mongoc_init ();

	mongoc_client_t *client = mongoc_client_new ("mongodb://localhost:27017/?appname=chat_get_user_id");
	mongoc_collection_t *collection_users = mongoc_client_get_collection (client, database_name.c_str(), user_collection_name.c_str());
	query = bson_new ();
	BSON_APPEND_UTF8 (query, "name", user_name.c_str());

	cursor = mongoc_collection_find_with_opts (collection_users, query, NULL, NULL);

	if (mongoc_cursor_next (cursor, &doc)) 
	{
  		str = bson_as_canonical_extended_json (doc, NULL);      		
  		str_result = std::string(str);
  		bson_free (str);
	}

	bson_destroy (query);
	mongoc_cursor_destroy (cursor);
	mongoc_collection_destroy (collection_users);
	mongoc_client_destroy (client);
	mongoc_cleanup ();

	return str_result;

}

//returns all users information including name and password
json_string MongoDB::get_all_users()
{
	std::unique_lock<std::mutex> lk(mx_query);

	std::string str_result;
	mongoc_client_t* client = mongoc_client_new ("mongodb://localhost:27017/?appname=chat_get_all_users");
	mongoc_collection_t *collection_users = mongoc_client_get_collection (client, database_name.c_str(), user_collection_name.c_str());

	mongoc_cursor_t *cursor;
	const bson_t *doc;
	bson_error_t error;
	bson_t query = BSON_INITIALIZER;
	char *str;

	cursor = mongoc_collection_find_with_opts (collection_users, &query, NULL, NULL);

	while (mongoc_cursor_next (cursor, &doc)) 
	{
		str = bson_as_canonical_extended_json (doc, NULL);
		str_result += std::string(str);
		bson_free (str);
	}

	if (mongoc_cursor_error (cursor, &error)) 
	{
		std::cout << "Failed to iterate all documents: " << error.message << std::endl;
	}

	mongoc_cursor_destroy (cursor);

	return str_result;   
}

	
bool MongoDB::init_collection(const std::string& collection_name)
{
	std::unique_lock<std::mutex> lk(mx_query);

	//check if collection exists in database
	mongoc_client_t* client = mongoc_client_new ("mongodb://localhost:27017/?appname=chat_init_collection");
	mongoc_database_t* database = mongoc_client_get_database(client, database_name.c_str());
	bson_error_t error;
	const bson_t *doc;
	bson_iter_t col_iter;
	const char *cur_name;
	bool result = false;		
	bson_t filter = BSON_INITIALIZER;
	mongoc_cursor_t *cursor;
	
	BSON_APPEND_UTF8 (&filter, "name", collection_name.c_str());

	cursor = mongoc_database_find_collections_with_opts (database, &filter);

	if (!cursor) 
	{
  		result =  false;
	}

	while (mongoc_cursor_next (cursor, &doc)) 
	{
  		if (bson_iter_init (&col_iter, doc) &&
      		bson_iter_find (&col_iter, "name") &&
      		BSON_ITER_HOLDS_UTF8 (&col_iter) &&
      		(cur_name = bson_iter_utf8 (&col_iter, NULL))) 
      	{
     		if (!strcmp (cur_name, collection_name.c_str())) 
     		{
        		result =  true;            
     		}
  		}
	}
	
	//if collection does not exist then create collection
	if (!result)
	{
		result = mongoc_database_create_collection (database, collection_name.c_str(), NULL, &error);
		if (!result) 
  			std::cout << "Error: " << error.message << std::endl;         		
	}
	mongoc_cursor_destroy (cursor);  
	mongoc_client_destroy (client);
	return result;
}

