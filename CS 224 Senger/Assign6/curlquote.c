#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

#define MASHAPE_KEY "X-Mashape-Key: ZrgAv0tZExmsh2UT0rGEd4zRjaqgp1Ms5H3jsndqKLiYb4bpSq"

struct buffer {
  size_t size;
  char * contents;
};

void
initBuffer(struct buffer * buffer) {
  buffer->size = 0;
  buffer->contents = malloc(1);
  buffer->contents[0] = '\0';
}

size_t
appendData(void * ptr, size_t size, size_t nmemb, struct buffer * buffer)
{
  printf("appendData size %lu nmemb %lu\n",size,nmemb);
  
  size_t newSize = buffer->size + size * nmemb;
  buffer->contents = realloc(buffer->contents, newSize+1);
  strncpy(buffer->contents + buffer->size, ptr, size * nmemb);
  buffer->contents[newSize] = '\0';
  buffer->size = newSize;
  return size * nmemb;
}

int
main(int argc, char * argv[])
{
  struct buffer buffer;
  
  initBuffer(&buffer);
  
  curl_global_init(CURL_GLOBAL_DEFAULT);
  
  CURL * curl = curl_easy_init();
  
  struct curl_slist * list = NULL;
  list = curl_slist_append(list,MASHAPE_KEY);
  list = curl_slist_append(list,"Content-Type: application/x-www-form-urlencoded");
  list = curl_slist_append(list,"Accept: application/json");
  curl_easy_setopt(curl,CURLOPT_HTTPHEADER,list);
  curl_easy_setopt(curl,CURLOPT_URL,"https://andruxnet-random-famous-quotes.p.mashape.com/?cat=movies");
  curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION, appendData);
  curl_easy_setopt(curl,CURLOPT_WRITEDATA, &buffer);
  
  CURLcode res = curl_easy_perform(curl);
  
  printf("%s\n",buffer.contents);
}
