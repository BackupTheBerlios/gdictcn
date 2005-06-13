#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

xmlChar *
getReference (xmlDocPtr doc, xmlNodePtr cur) {

	xmlChar *def = NULL;
	cur = cur->xmlChildrenNode;
	while (cur != NULL) {
	    if ((!xmlStrcmp(cur->name, (const xmlChar *)"def"))) {
		    def = xmlNodeListGetString(doc,cur->xmlChildrenNode,1);
		//	printf("def : %s\n",def);
		    //xmlFree(def);
			break;
	    }
	    cur = cur->next;
	}
	return def;
}


char *
parseDoc(char *docname) {

	xmlDocPtr doc;
	xmlNodePtr cur;
	char * content_tmp;

	doc = xmlParseMemory(docname,strlen(docname));
	
	if (doc == NULL ) {
		fprintf(stderr,"Document not parsed successfully. \n");
		return NULL;
	}
	
	cur = xmlDocGetRootElement(doc);
	
	if (cur == NULL) {
		fprintf(stderr,"empty document\n");
		xmlFreeDoc(doc);
		return NULL;
	}
	
	if (xmlStrcmp(cur->name, (const xmlChar *) "dict")) {
		fprintf(stderr,"document of the wrong type, root node != dict");
		xmlFreeDoc(doc);
		return NULL;
	}
	
	if(!(content_tmp = getReference (doc, cur)))
		return NULL;
	else{
		xmlFreeDoc(doc);
		return strdup(content_tmp);
	}

	return NULL;
}

/*
int
main(int argc, char **argv) {

	char *docname;

	if (argc <= 1) {
		printf("Usage: %s docname\n", argv[0]);
		return(0);
	}

	docname = argv[1];
	parseDoc (docname);
	
	return 0;
}
*/
