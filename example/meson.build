project('discord-rpc-test', 'c',
        default_options: ['default_library=static'])

executable('discord-rpc-test',
           'main.c',
           dependencies: [subproject('discord-rpc').get_variable('discord_rpc_dependency')],
           gui_app: false,
           install: true)
