#include <openssl/sha.h>
#include <time.h>
#include <string>
#include <list>
#include <iostream>
#include <stdexcept>

#include "file.h"
#include "rules.h"
#include "build.h"
#include "debug.h"
#include "subprocess.h"
#include "dependencies.h"
#include "match.h"
#include "plotter.h"

using namespace std;

// This is just for controlling the indentation of debug prints
int indentation = 0;

// List of all active rules
list<Rule *> Rule::rules;
// What has been build already
std::set<std::string> Rule::buildCache;

Plotter *Rule::plotter = NULL;

Subprocess::~Subprocess( )
{
}

void Rule::print()
{
    cout << "Rule:" << endl;

    if( targets != NULL ) {
        cout << "Targets:" << endl;
        for(list<string>::iterator i = targets->begin();
                i != targets->end();
                i ++)
        {
            cout << (*i);
        }
        cout << endl;
    }
    if( commands != NULL ) {
        cout << "Commands:" << endl;
        for(list<string>::iterator i = commands->begin();
                i != commands->end();
                i ++ )
        {
            cout << (*i) << endl;
        }
        cout << endl;
    }
}

void indent()
{
    int i;
    for( i = 0; i < indentation; i ++ ) {
        cout << " ";
    }
}

void print(std::string filename)
{
    indent();
    cout << "Depends on " << filename << endl;
}

void print(std::string filename, int status)
{
    indent();
    cout << "Depends on " << filename << "(" << status << ")" << endl;
}

void Rule::callback_entry(std::string filename)
{
    pair<Rule *, Match *> r;

    string canon = fileCanonicalize( filename );
    if( get_debug_level( DEBUG_DEPENDENCIES ) ) {
        ::print(canon);
    }

    r = Rule::find(canon);
    if( r.first ) {
        r.first->execute( canon, r.second );
        dependencies.insert( pair<string,bool>(canon, true) );
    }
}

void Rule::callback_exit(std::string filename, bool success)
{
    string canon = fileCanonicalize( filename );
    dependencies.insert( pair<string,bool>(canon, success) );

    if( get_debug_level( DEBUG_DEPENDENCIES ) ) {
        ::print(canon, success);
    }
}

bool Rule::build(const std::string &target, bool *updated)
{
    pair<Rule *, Match *> r;
    
    r = find( fileCanonicalize( target ) );
    if( r.first ) {
        *updated = r.first->execute( target, r.second );
        delete r.second;
        return true;
    } else {
        // No rule to build the target. But if it exists, that's still okay
        if( fileExists( target ) ) {
            *updated = false;
            return true;
        } else {
            return false;
        }
    }
}

bool Rule::execute(const string &target, Match *m)
{
    unsigned char hash[SHA256_DIGEST_LENGTH];
    bool needsRebuild = false;
    list<string>::iterator targeti;
    list<pair<string, bool> > *deps;
    time_t targetTime;
    string targetName;

    // See if it's already being built
    if( built( target ) ) return false;

    // Insert the name in the build cache
    for(targeti = targets->begin(); targeti != targets->end(); targeti ++ ) {
        targetName = m->substitute( *targeti );
        buildCache.insert( targetName );
    }

    if( targets == NULL || commands == NULL ) return false;

    if( fileTime( target, &targetTime ) ) {
        // No target time, so definitely rebuild
        targetTime = 0;
    }

    if( get_debug_level( DEBUG_DEPENDENCIES ) ) {
        indent();
        cout << "Try to build: " << *targets->begin() << "(" << target << "," << targetTime << ")" << endl;
    }
    indentation ++;

    recalcHash( target, hash );
    // See if we have dependencies in the database
    deps = retrieve_dependencies( hash );
    // If we know the dependencies, we may be able to avoid building. If we
    // don't know the dependencies, we definitely have to rebuild.
    if( deps != NULL ) {
        // Find which target from the list of targets in the rule is used to build this target
        // Check for any listed dependencies
        for( list<pair<string,bool> >::iterator i = declaredDeps->begin(); i != declaredDeps->end(); i ++ ) {
            string depName;
            depName = m->substitute( i->first );
            needsRebuild |= checkDep( target, depName, i->second, targetTime );
            if( plotter != NULL ) {
                plotter->output( target, depName );
            }
        }
        // And check for any dependencies we find
        for( list<pair<string, bool> >::iterator i = deps->begin(); i != deps->end(); i ++ ) {
            needsRebuild |= checkDep( target, i->first, i->second, targetTime );
            if( plotter != NULL ) {
                plotter->output( target, i->first );
            }
        }
        delete deps;

        if( !needsRebuild ) {
            indentation --;
            return false;
        }
        if( get_debug_level( DEBUG_DEPENDENCIES ) ) {
            cout << "Dependency updated, must build" << endl;
        }
    } else {
        // We don't know the dependencies, have to
        // build
        if( get_debug_level( DEBUG_REASON ) ) {
            cout << "Dependencies unknown, must build \"" << target << "\"" << endl;
        }

        // Even though we don't know the auto-generated dependencies, there
        // may be explicit dependencies, so build those
        for( list<pair<string, bool> >::iterator i = declaredDeps->begin();
                                    i != declaredDeps->end();
                                    i ++ ) {
            string s;
            time_t t;
            bool updated;
            s = m->substitute( i->first );
            if( get_debug_level( DEBUG_DEPENDENCIES ) ) {
                cout << "Building explicit dependency `" << s << "'" << endl;
            }
            if( !fileTime( s, &t ) || t <= targetTime ) {
                if( !build( s, &updated ) ) {
                    if( get_debug_level( DEBUG_REASON ) ) {
                        cout << "Cannot build explicit dep `" << s << "'" << endl;
                    }
                    indentation --;
                    return false;
                }
            }
        }
    }
    
    clear_dependencies( hash );
    for(list<string >::iterator i = commands->begin(); i != commands->end(); i ++ ) {
        trace( expand_command( *i, target, m ) );

        // Touch the targets in case something else updated last in the build process 
    }
    add_dependencies( hash, dependencies );
    dependencies.clear();
    indentation --;
    if( get_debug_level( DEBUG_DEPENDENCIES ) ) {
        indent();
        cout << "Done trying to build: " << *targets->begin() << "(" << target << "," << targetTime << ")" << endl;
    }
    if( plotter != NULL ) {
        for( set<pair<string, bool> >::iterator j = dependencies.begin();
                                                j != dependencies.end();
                                                j ++ ) {
            plotter->output( target, j->first );
        }
    }

    return true;
}

Rule::Rule( )
{
    rules.push_back(this);
    targets = NULL;
    declaredDeps = new list<pair<string,bool> >;
    commands = NULL;
}

pair<Rule *,Match *>Rule::find(const string &target)
{
    bool depsFound;
    Match *m, *oldm;
    Rule *r = NULL;
    
    for(list<Rule *>::iterator i = rules.begin(); i != rules.end(); i ++ )
    {
        if( (*i)->match( target, &m ) ) {
            if( r ) {
                // We have multiple rules to build the target
                delete oldm;
                delete m;
                return pair<Rule *, Match *>(NULL, NULL);
            }
            // Check that we have all the explicit dependencies, or it's not worth even trying
            depsFound = true;
            for( list<pair<string,bool> >::iterator j = (*i)->declaredDeps->begin(); j != (*i)->declaredDeps->end(); j ++ ) {
                if( j->second && !canBeBuilt( m->substitute( j->first ) ) ) {
                    // Cannot build this file
                    depsFound = false;
                    break;
                }
            }
            if( depsFound ) {
                r = *i;
                oldm = m;
            } else {
                delete m;
            }
        }
    }

    return pair<Rule *, Match *>(r,m);
}

bool Rule::match(const std::string &target, Match **match)
{
    std::list<std::string>::iterator b, e, te;

    if( targets == NULL ) return false;

    *match = new Match;

    b = targets->begin();
    e = targets->end();
    for( te = b; te != e; te ++ ) {
        if( *te == target ) {
            return true;
        }
    }
    return false;
}

bool Rule::canBeBuilt(const std::string &file)
{
    pair<Rule *,Match *> r;
    // Check if the file already exists
    if( fileExists( file ) ) return true;

    // If the file doesn't exist, see if we can build it
    r = Rule::find( file );
    if( r.first ) {
        delete r.second;
    }
    return r.first != NULL;
}

void Rule::addTarget(const std::string &target)
{
    if( targets == NULL ) {
        targets = new list<string>;
    }
    targets->push_back(fileCanonicalize(target));
}

void Rule::addTargetList(std::list<std::string> *targetList)
{
    if( targets == NULL ) {
        targets = new list<string>;
    }

    for( list<string>::iterator i = targetList->begin(); i != targetList->end(); i ++ ) {
        targets->push_back(fileCanonicalize(*i));
    }
}

void Rule::addDependency(const std::string &dependency, bool exists)
{
    declaredDeps->push_back(pair<string,bool>(fileCanonicalize(dependency), exists));
}

void Rule::addDependencyList(list<pair<string,bool> > *dependencyList)
{
    for( list<pair<string,bool> >::iterator i = dependencyList->begin(); i != dependencyList->end(); i ++ ) {
        declaredDeps->push_back(pair<string,bool>(fileCanonicalize(i->first),i->second));
    }
}

void Rule::addCommand(const std::string &command)
{
    if( commands == NULL ) {
        commands = new list<string>;
    }
    commands->push_back(command);
}

void Rule::addCommandList(std::list<std::string> *commandList)
{
    if( commands != NULL ) {
        delete commands;
    }

    commands = commandList;
}

void Rule::setDefaultTargets(void)
{
    if( !rules.empty() ) {
        Rule *r = *rules.begin();
        if( r->targets == NULL ) throw runtime_error("First rule has no targets");
        for(list<string>::iterator i = r->targets->begin(); i != r->targets->end(); i ++ ) {
            set_target(*i);
        }
    }
}

void Rule::setPlotter( Plotter *p )
{
    plotter = p;
}

string Rule::expand_command( const string &command, const string &target, Match *m )
{
    return command;
}

void Rule::recalcHash(string target, unsigned char *hash)
{
    SHA256_CTX sha256;
    SHA256_Init(&sha256);

    if( targets != NULL ) {
        for(list<string>::iterator i = targets->begin(); i != targets->end(); i ++ ) {
          SHA256_Update(&sha256, i->c_str(), i->size() );
        }
    }

    if (commands != NULL ) {
        for(list<string>::iterator i = commands->begin(); i != commands->end(); i ++ ) {
            SHA256_Update(&sha256, i->c_str(), i->size() );
        }
    }
    SHA256_Update(&sha256, target.c_str(), target.size());
    SHA256_Final(hash, &sha256);
}

bool Rule::built( const string &target )
{
    return buildCache.find( target ) != buildCache.end();
}

bool Rule::checkDep( const string &ruleTarget, const string &target, bool exists, time_t targetTime )
{
    pair<Rule *,Match *>r;
    // If the file has a rule, we need to try to rebuild it, and rebuild if that
    // succeeds.
    // If the file doesn't have a rule, then:
    // If the file didn't exist before, and it still doesn't, we don't need to
    // rebuild.
    // If the file didn't exist before, and it does now, we need to rebuild.
    // If the file existed before and doesn't exist now, we need to rebuild.
    // If the file existed before and still exists, we need to rebuild if the
    // file is newer
    if( get_debug_level( DEBUG_DEPENDENCIES ) ) {
        indent();
        cout << "Dependency " << target << "(" << exists << ")" << endl;
    }
    r = Rule::find(target);
    if( r.first ) {
        // Use a rule to rebuild
        if( r.first->execute( target, r.second ) ) {
            delete r.second;
            // It was rebuilt, so we need to rebuild the primary target
            if( get_debug_level( DEBUG_REASON ) ) {
                cout << "Dependency \"" << target << "\" rebuilt, need to rebuild \"" << ruleTarget << "\"" << endl;
            }
            return true;
        } else {
            bool status;
            time_t t;
            bool isDir;

            delete r.second;
            // It was rebuilt, so we need to rebuild the primary target
            // If it wasn't rebuilt, but is already newer, we still
            // have to rebuild.
            status = !fileTime(target, &t, &isDir);
            if( !status || (t > targetTime && !isDir) ) {
                if( get_debug_level( DEBUG_REASON ) ) {
                    indent();
                    string t1 = ctime(&t);
                    string t2 = ctime(&targetTime);
                    t1 = t1.substr(0, t1.length() - 1);
                    t2 = t2.substr(0, t2.length() - 1);
                    if( !status ) {
                        cout << "Dependency \"" << ruleTarget << "\" missing, need to rebuild \"" << ruleTarget << "\"" << endl;
                    } else {
                        cout << ruleTarget << "(" << t2 << ") is newer than target (" << t1 << "), build needed" << endl;
                    }
                }
                return true;
            }
        }
    } else {
        bool status;
        time_t t;
        bool isDir;

        status = !fileTime(target, &t, &isDir);
        if( (status ^ exists) || (status && t > targetTime && !isDir) ) {
            if( get_debug_level( DEBUG_REASON ) ) {
                indent();
                if( status && !exists ) {
                    cout << "No rule to rebuild \"" << target << "\" and it has been created, must rebuild \"" << ruleTarget << "\"" << endl;
                } else if( !status && exists ) {
                    cout << "No rule to rebuild \"" << target << "\" and it has been deleted, must rebuild \"" << ruleTarget << "\"" << endl;
                } else {
                    string t1 = ctime(&t);
                    string t2 = ctime(&targetTime);
                    t1 = t1.substr(0, t1.length() - 1);
                    t2 = t2.substr(0, t2.length() - 1);
                    cout << "No rule to rebuild \"" << target << "\"(" << t1 << ") and it is newer than \"" << ruleTarget << "\"(" << t2 << "), must rebuild \"" << ruleTarget << "\"" << endl;
                }
            }
            return true;
        }
    }

    return false;
}
