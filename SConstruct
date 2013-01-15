# configuring environment
CCFLAGS = '-Wall -pedantic -std=c++0x'
LINKFLAGS = '-lboost_thread -lboost_system -lboost_program_options -pthread'
env = Environment(CCFLAGS = CCFLAGS, LINKFLAGS = LINKFLAGS)

# defining source files list
main = [ 'main.cpp' ]
core = [
  'core/controller.cpp',
  'core/config.cpp',
  'core/eventQueue.cpp',
  'core/console.cpp',
  'core/parser.cpp',
  'core/logger.cpp',
  'common/MessageDictionary.cpp',
  'common/MD5.cpp'
]

connection = [
  'core/connection.cpp'
]

# defining application ingredients
app = core + connection
program = env.Program(target = 'program', source = app + main)
Default(program)
