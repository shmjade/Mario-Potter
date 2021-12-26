#ifndef _LOAD_MEDIA_H_
#define _LOAD_MEDIA_H_
#include "all_headers.h"
void load_media(SRC *media);
PHASE load_phase(char fullTitle[MAX_CHAR_NAME], SRC media);
void load_map(SRC media);
void load_journey(PHASE journey[3], SRC media);
#endif // _LOAD_MEDIA_H_
