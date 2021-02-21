project('ptmake', 'cpp')
ssldep = dependency('libssl')
cryptodep = dependency('libcrypto')
sources = ['argpc.cc', 'argpcoption.cc', 'build.cc', 'debug.cc', 'dependencies.cc', 'exception.cc', 'file_unix.cc', 'find.cc', 'lex.cc', 'main.cc', 'make.cc', 'make_match.cc', 'make_rules.cc', 'match.cc', 'plotter.cc', 'rules.cc', 'utilities.cc', 'variables.cc']

warning(host_machine.system())
if host_machine.system() == 'linux'
  sources += [ 'subprocess_unix.cc' ]
elif host_machine.system() == 'darwin'
  sources += [ 'subprocess_mach.cc' ]
elif host_machine.system() == 'windows'
  sources += [ 'subprocess_win.cc' ]
endif

# Yacc support
yacc = find_program('yacc')

parsers = custom_target('make_parser',
  output  : ['parser.hh', 'parser.cc'],
  input   : 'make_parse.y',
  command : [yacc, '-d', '-o', 'parser.cc', '@INPUT@'],
  install : false)

# Berkeley DB
lbdb_found = false
lbdb_cargs = []

compiler = meson.get_compiler('cpp')

bdb_lib = compiler.find_library('db', required: true)

  
executable('ptmake', sources: sources + [parsers], dependencies: [ssldep, cryptodep, bdb_lib])
