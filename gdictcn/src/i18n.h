#ifndef I18N_H 
#define I18N_H
#include <libintl.h>
#include <locale.h>
#define _(text) gettext(text)
#endif
