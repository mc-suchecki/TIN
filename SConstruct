# configuring environment
CCFLAGS = '-Wall -pedantic'
LINKFLAGS = '-lboost_thread -lboost_system'
env = Environment(CCFLAGS = CCFLAGS, LINKFLAGS = LINKFLAGS)

# defining source files list
main = [ 'main.cpp' ]
core = [
  'core/controller.cpp',
  'core/eventQueue.cpp',
  'core/console.cpp',
  'core/parser.cpp',
  'core/logger.cpp'
]

connection = [
  'core/connection.cpp'
]

# defining application ingredients
app = core + connection
program = env.Program(target = 'program', source = app + main)
Default(program)
