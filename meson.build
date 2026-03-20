project(
    'hgdler',
    'cpp',

    default_options: [
        # 'buildtype=debugoptimized',
        'buildtype=debug',
        'warning_level=2',
        'cpp_std=c++20',
    ],
    license: 'Public Domain',
    meson_version: '>= 0.54.0',
)

add_global_arguments('-fsanitize=address', language: 'c')

dl_include_directories = include_directories(['src'])

dl_exec_src = [
    './src/main.cpp',
    './src/filedl/filedl.cpp',
    './src/data_queue/data_queue.cpp',
    './src/jobs_main/download_main.cpp',
    './src/md5calc/md5calc.cpp',
    './src/zipfile/zipfile.cpp',
    './src/jobs_main/extract_main.cpp',

]

depcurl = dependency('libcurl')
depopenssl = dependency('openssl')
depzlib = dependency('zlib-ng')
depspdlog = dependency('spdlog')

executable(
    'hgdler',
    sources: [dl_exec_src],
    include_directories: [dl_include_directories],
    dependencies: [depcurl, depopenssl, depzlib, depspdlog],
)