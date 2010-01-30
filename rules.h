#ifdef __cplusplus
extern "C" {
#endif

void print_rule(void *);
void *make_rule(void *, void *);
void *make_rule_header(void *, void *);
void *make_dependencies(void *, void *);
void *new_stringlist();
void *add_stringlist(void *slist, void *string);

#ifdef __cplusplus
}
#endif
