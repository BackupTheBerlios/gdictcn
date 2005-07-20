#ifndef DICTCN_H
#define DICTCN_H
#define DICT_URL "http://dict.cn/ws.php?key="
#define DICT_URL_WEB "http://dict.cn/search?q="
int u2g(char *inbuf,int inlen,char *outbuf,int outlen);
char * dict_find(char * findstr);
char * parseDoc(char *docname);
#endif
