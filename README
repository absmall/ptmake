![build workflow](https://github.com/absmall/ptmake/actions/workflows/build.yml/badge.svg)

What is ptmake?

ptmake is a make tool which will parse standard makefiles, but ignore the
listed dependenices, and instead, determine dependencies itself.  By ptracing
every command run as part of a build process, ptmake knows exactly what files
the command depends on. It will rebuild what is necessary, and only what is
necessary.

Portability:

As ptrace is used to track all filesystem accesses while running, and many
operating systems do not have either ptrace or a ptrace-like API, ptmake is
not, and never will be, very portable. The intended use of ptmake is to
dramatically speed up builds on platforms where ptmake is supported, by parsing
standard (portable) gnumakefiles. This allows you to speed up your builds by using
ptmake, while your neighbour, who uses an operating system without a suitable
kernel interface to trap filesystem accesses, can still build. They will have
imperfect dependency information, so may have to do more clean builds, but can
still build something.

Design:

The concept of ptmake is to start at the top when building a target whose
dependencies are unknown, and at the bottom when building a target whose
dependencies are known. For example, say B can be built from A using rule 1,
and C can be built from B using rule 2. On the first attempt to build C, ptmake will do the following:

1) Execute rule 2 to build C
2) Trap the kernel entry to access file B.
3) Suspend the execution of rule 2 while searching for a rule to build B
4) Finds rule 1
5) Record that rule 2 depends on B
6) Executes rule 1
7) Trap the kernel entry to access file A.
8) Look for a rule to build A.
9) Record that rule 1 depends on A
10) No rule found, rule 1 continues.
11) Rule 1 completes, continue rule 2.
12) Rule 2 completes.

On a second attempt to build C, the process will be somewhat different, as the dependency information is cached from the first build.

1) See that C can be built using rule 2
2) See that rule 2 depends on B
3) See that B can be built using rule 1
4) See that rule 1 depends on A
5) See that A has an older timestamp than B, no need to execute rule 1.
6) See that B has an older timestamp than C, no need to execute rule 2.
