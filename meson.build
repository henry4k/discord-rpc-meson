project('discord-rpc', 'cpp')

pub_dependencies = [dependency('threads')]
dependencies = [dependency('RapidJSON', version: '>=1.1.0')]

sources = ['include/discord_rpc.h',
           'src/discord_rpc.cpp',
           'include/discord_register.h',
           'src/rpc_connection.h',
           'src/rpc_connection.cpp',
           'src/serialization.h',
           'src/serialization.cpp',
           'src/connection.h',
           'src/backoff.h',
           'src/msg_queue.h']

pub_cpp_args = []
cpp_args = [] #['-std=c++14']
link_args = [] #['-std=c++14']

if not get_option('io_thread')
    pub_cpp_args += '-DDISCORD_DISABLE_IO_THREAD'
endif

if get_option('default_library') == 'shared'
    pub_cpp_args += '-DDISCORD_DYNAMIC_LIB'
    cpp_args += '-DDISCORD_BUILDING_SDK'
endif

host_system = host_machine.system()
compiler = meson.get_compiler('cpp').get_id()

if host_system == 'windows'
    sources += 'src/connection_win.cpp'
    sources += 'src/discord_register_win.cpp'
    pub_cpp_args += '-DDISCORD_WINDOWS'
    dependencies += dependency('psapi')
    dependencies += dependency('advapi32')

    if get_option('default_library') == 'shared'
        sources += 'src/dllmain.cpp'
    endif
else
    sources += 'src/connection_unix.cpp'

    if host_system == 'linux'
        sources += 'src/discord_register_linux.cpp'
        pub_cpp_args += '-DDISCORD_LINUX'
    elif host_system == 'darwin'
        sources += 'src/discord_register_osx.m'
        pub_cpp_args += '-DDISCORD_OSX'
        link_args += '-framework AppKit'
    else
        error('Unsupported host system: '+host_system)
    endif
endif

if compiler == 'msvc'
    cpp_args += ['/EHsc',
                 '/wd4100', # unreferenced formal parameter
                 '/wd4514', # unreferenced inline
                 '/wd4625', # copy constructor deleted
                 '/wd5026', # move constructor deleted
                 '/wd4626', # move assignment operator deleted
                 '/wd4668', # not defined preprocessor macro
                 '/wd4710', # function not inlined
                 '/wd4711', # function was inlined
                 '/wd4820', # structure padding
                 '/wd4946', # reinterpret_cast used between related classes
                 '/wd5027'] # move assignment operator was implicitly defined as deleted
else # GCC-like
    cpp_args += ['-Wno-unknown-pragmas', # pragma push thing doesn't work on clang
                 '-Wno-old-style-cast', # it's fine
                 '-Wno-c++98-compat', # that was almost 2 decades ago
                 '-Wno-c++98-compat-pedantic',
                 '-Wno-missing-noreturn',
                 '-Wno-padded', # structure padding
                 '-Wno-covered-switch-default',
                 '-Wno-exit-time-destructors', # not sure about these
                 '-Wno-global-constructors']
endif


discord_rpc_include = include_directories('include')
discord_rpc_library = library('discord-rpc', sources,
    include_directories: discord_rpc_include,
    dependencies: pub_dependencies + dependencies,
    cpp_args: pub_cpp_args + cpp_args,
    link_args: link_args)
discord_rpc_dependency = declare_dependency(
    link_with: discord_rpc_library,
    include_directories: discord_rpc_include,
    dependencies: pub_dependencies,
    compile_args: pub_cpp_args)
