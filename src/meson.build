project('ade', 'c', default_options : ['c_std=c11'])

shmif = dependency('arcan-shmif')
incdir = include_directories('/usr/local/include/arcan/shmif')

executable('ade', 'main.c', include_directories : incdir, dependencies : shmif)
