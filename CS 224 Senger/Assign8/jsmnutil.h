#define JSMN_PARENT_LINKS
#include "jsmn/jsmn.h"

// A wrapper for the JSON string data and the JSMN parsed tokens
typedef struct {
  char * json;
  size_t jsonSize;
  size_t jsonAllocatedSize;
  jsmntok_t * tokens;
  unsigned int tokensSize;
  unsigned int tokensAllocatedSize;
} jsmndata_t;

// References the JSON entity at index in the context of the JSMN data
typedef struct {
  jsmndata_t * data;
  int index;
} jsmnmarker_t;

// Macros to extract token fields given marker struct
// Assumes that m is an initialized marker struct
#define TOKEN_LENGTH(m) (m.data->tokens[m.index].end - m.data->tokens[m.index].start)
#define TOKEN_PARENT(m) (m.data->tokens[m.index].parent)
#define TOKEN_TYPE(m) (m.data->tokens[m.index].type)
#define TOKEN_SIZE(m) (m.data->tokens[m.index].size)
#define TOKEN_ADDR(m) (&(m.data->json[m.data->tokens[m.index].start]))

// An array of strings names for the various JSMN symbolic constants
extern char * jsmnTypes[];

// An initialized marker struct representing an error condition
// Failed dereferencing attempts will result in this token
extern const jsmnmarker_t jsmnErrorMarker;

// test marker structs for equality
int             areEqualMarkers(jsmnmarker_t marker1, jsmnmarker_t marker2);

// tests if the marker is equal to the error marker
int             isErrorMarker(jsmnmarker_t marker);

// return an initialized marker struct for the root entity in the context data
jsmnmarker_t    makeMarker(jsmndata_t * data);

// return a marker struct for the entity corresponding to key in object referenced by marker
jsmnmarker_t    objectDeref(jsmnmarker_t marker, char * key);

// return a marker struct for the entity at index in object referenced by marker
jsmnmarker_t    arrayDeref(jsmnmarker_t marker, int index);

// perform a series of dereference operations on the entry referenced by marker
// fmt is a string of 's' and 'd' characters denoting whether the remaining
// arguments should be interpreted as object key strings or array indices

jsmnmarker_t    derefBy(jsmnmarker_t marker, char * fmt, ...);

// return marker for next key of object, when objMarker == keyMarker, returns
// marker for first key, when not equal it is next key after keyMarker, when no
// more keys returns error marker
jsmnmarker_t    nextKeyForObject(jsmnmarker_t objMarker, jsmnmarker_t keyMarker);

// return a marker for the value corresponding to the object key marker
jsmnmarker_t    valueForKey(jsmnmarker_t keyMarker);

// return a string corresponding to the json data referenced by marker or NULL
// the string is created with strndup(), the caller is responsible for deallocation
char *          tokenText(jsmnmarker_t marker);

// prints the string corresponding to the json data referenced by marker to fh
// if marker does not refer to a token the string "unresolved reference" is printed
void            printToken(FILE * fh, jsmnmarker_t marker);
void            dprintToken(int fd, jsmnmarker_t marker);
