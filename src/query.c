#include "query.h"

static FILE *fp;
static char *file_name = "response.txt";

/**
 * @brief Handles entire query process
 * 
 */
void query() {
    USER user;
    CITY city;
    QUERY query;
    query.user = &user;
    query.city = &city;

    init_query(query);
    set_query_URL(query);
    send_query_request(fp, fpname, query);
}

/**
 * @brief Initializes query. Asks for the city name and app id. 
 * 
 */
void init_query(QUERY query) {

    char *temp;

    // request app id
    temp = malloc(sizeof(char)*(MAX_APP_ID_LEN));
    if (temp != NULL) {
        printf("APP ID: ");
        fgets(temp, MAX_APP_ID_LEN, stdin);
        (query.user)->app_id_len = strlen(temp); // get size of app_id
        strcpy(query.user->app_id, temp); // set user app id

    } else {
        printf("Failed to allocate memory while requesting app ID");
    }
    free(temp);
    
    // request name of city to query 
    temp = malloc(sizeof(char)*(MAX_CITY_NAME_LEN));
    if (temp != NULL) {
        printf("CITY TO QUERY: ");
        fgets(temp, MAX_CITY_NAME_LEN, stdin);
        query.city->city_name_len = strlen(temp); // get size of city
        strcpy(query.city->city_name, temp); // set city to query 
    } else {
        printf("Failed to allocate memory while requesting city");
    }
    free(temp);
}

/**
 * @brief Create a query URL by appending city_name and app id to the request url
 * 
 */
void set_query_URL(QUERY query) {
    strcpy(query.request_url, url);// set base url
    strcat(query.request_url, "q="); // add city parameter to the url
    strncat(query.request_url, query.city->city_name, query.city->city_name_len-1);
    strcat(query.request_url, "&appid="); // add the app id to the url
    strncat(query.request_url, query.user->app_id, strlen(query.request_url) + strlen(query.user->app_id));
    *(query.request_url + sizeof(query.request_url) + 1) = '\0'; //null-terminate the string 
}

struct MemoryStruct {
  char *memory;
  size_t size;
};
 
static size_t
WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
  size_t realsize = size * nmemb;
  struct MemoryStruct *mem = (struct MemoryStruct *)userp;
 
  char *ptr = realloc(mem->memory, mem->size + realsize + 1);
  if(ptr == NULL) {
    /* out of memory! */ 
    printf("not enough memory (realloc returned NULL)\n");
    return 0;
  }
 
  mem->memory = ptr;
  memcpy(&(mem->memory[mem->size]), contents, realsize);
  mem->size += realsize;
  mem->memory[mem->size] = 0;
 
  return realsize;
}

/**
 * @brief Creates a query requests, and creates a file that stores the resonse from the query
 * 
 */
void send_query_request(FILE *fp, char *fpname, QUERY query) {
    fp = fopen(fpname, "wb");
    if (fp != NULL) {
        CURL *curl;
        struct MemoryStruct chunk;
        chunk.memory = malloc(1);  /* will be grown as needed by the realloc above */ 
        chunk.size = 0;
        CURLcode res;

        curl = curl_easy_init();
        if (curl) {
            curl_easy_setopt(curl, CURLOPT_URL, query.request_url);
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);
            curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");
            res = curl_easy_perform(curl);
            if(res != CURLE_OK) {
                fprintf(stderr, "curl_easy_perform() failed: %s\n",
                curl_easy_strerror(res));
            }
        } else {
            printf("Curl init failed.");
        }
    } else {
        printf("File open not opened.");
    }
}

/**
 * @brief read file <fp> for the temperature value 
 * 
 * @return temperature (in degrees celsius)
 */
int read_file_for_temp(FILE *fp);

/**
 * @brief Return kelvin temperature to celsius temperature
 * 
 */
void convert_to_celsius(int temp) {
    temp = temp - 273.15;
}

