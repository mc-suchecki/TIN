# configuring environment
CCFLAGS = '-Wall -pedantic'
LINKFLAGS = '-lboost_thread'
env = Environment(CCFLAGS = CCFLAGS, LINKFLAGS = LINKFLAGS)

# defining source files list
main = [ 'main.cpp' ]
core = [
  'core/controller.cpp',
  'core/eventQueue.cpp',
  'core/console.cpp',
  'core/parser.cpp'
]
connection = [
  'core/clientsConnection.cpp',
  'core/serversConnection.cpp',
  'core/message.cpp'
]
events = [
  'events/event.cpp',
  'events/consoleEvent.cpp',
  'events/connectionEvent.cpp'
]

# defining application ingredients
app = core + ui + connection + events
cabaj = env.Program(target = 'cabaj', source = app + main)
Default(cabaj)
