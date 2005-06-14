#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iconv.h>

#include <curl/curl.h>
#include <curl/types.h>
#include <curl/easy.h>
#include "dictcn.h"

struct MemoryStruct {
  char *memory;
  size_t size;
};

size_t
WriteMemoryCallback(void *ptr, size_t size, size_t nmemb, void *data)
{
  register int realsize = size * nmemb;
  struct MemoryStruct *mem = (struct MemoryStruct *)data;
  
  mem->memory = (char *)realloc(mem->memory, mem->size + realsize + 1);
  if (mem->memory) {
    memcpy(&(mem->memory[mem->size]), ptr, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;
  }
  return realsize;
}

int code_convert(char *from_charset,char *to_charset,char *inbuf,int inlen,char *outbuf,int outlen)
{
 	iconv_t cd;
	char **pin = &inbuf;
	char **pout = &outbuf;

	cd = iconv_open(to_charset,from_charset);
	if (cd==0) return -1;
	memset(outbuf,0,outlen);
	if (iconv(cd,pin,&inlen,pout,&outlen)==-1) return -1;
	iconv_close(cd);
	return 0;
}
//UNICODEGB2312
int u2g(char *inbuf,int inlen,char *outbuf,int outlen)
{
	return code_convert("utf-8","gb2312",inbuf,inlen,outbuf,outlen);
}
//GB2312UNICODE
int g2u(char *inbuf,size_t inlen,char *outbuf,size_t outlen)
{
	return code_convert("gb2312","utf-8",inbuf,inlen,outbuf,outlen);
}
char * dict_find(char * findstr)
{
  CURL *curl_handle;
  char dicturl[256]={0,};
  char strdest[200]={0,};
  char * esc;
  iconv_t it;
  int aa;

  struct MemoryStruct chunk;

  chunk.memory=NULL; 
  chunk.size = 0;   
  if(strlen(findstr) >150)
	*(findstr+150)=0;

  it=iconv_open("gb2312","utf-8");
  aa=u2g(findstr,strlen(findstr),strdest,150);
  esc=curl_escape(strdest,strlen(strdest));
  sprintf(dicturl,"%s%s",DICT_URL,esc);
  
  iconv_close(it);
  curl_global_init(CURL_GLOBAL_ALL);
  curl_handle = curl_easy_init();

  curl_easy_setopt(curl_handle, CURLOPT_URL, dicturl);

  curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);

  curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&chunk);

  curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");

  curl_easy_perform(curl_handle);

  curl_easy_cleanup(curl_handle);

    return parseDoc(chunk.memory);

}

/*
int main()
{
	dict_find("love");
	return 0;
}
*/
