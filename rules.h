#ifdef __cplusplus
extern "C" {
#endif

typedef struct _Dependencies Dependencies;
typedef struct _RuleHeader RuleHeader;
typedef struct _Rule Rule;
typedef struct _StringList StringList;
typedef struct _String String;

void print_rule(Rule *);
Rule *make_rule(RuleHeader *, StringList *);
RuleHeader *make_rule_header(StringList *, Dependencies *);
Dependencies *make_dependencies(StringList *, StringList *);
StringList *new_stringlist();
StringList *add_stringlist(StringList *slist, String *string);

#ifdef __cplusplus
}
#endif
