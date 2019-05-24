{
  'targets': [
    {
      'target_name': 'node-serialize-lib',
      'include_dirs': [
        "<!@(node -p \"require('node-addon-api').include\")",
        "<!@(node -p \"require('nan')\")",
        "vendor/flatbuffers/include"
      ],
      'dependencies': [
        "<!(node -p \"require('node-addon-api').gyp\")"
      ],
      'defines': [
        'USE_NAN'
      ],
      'cflags!': [ '-fno-exceptions' ],
      'cflags_cc!': [ '-fno-exceptions' ],
      'sources': [
        'src/binding.cc',
        'src/flexbuffer.cc',
        'vendor/flatbuffers/src/util.cpp'
      ],
      'conditions': [
        ['OS=="win"', {
          'msvs_settings': {
            'VCCLCompilerTool': { 'ExceptionHandling': 1 }
          }
        }],
        ['OS=="mac"', {
          'cflags+': ['-fvisibility=hidden'],
          'xcode_settings': {
            'GCC_SYMBOLS_PRIVATE_EXTERN': 'YES',
            'GCC_ENABLE_CPP_EXCEPTIONS': 'YES',
            'OTHER_CFLAGS': [
              '-ObjC++',
              '-Wno-format-security'
            ],
            'CLANG_CXX_LIBRARY': 'libc++',
            'MACOSX_DEPLOYMENT_TARGET': '10.7'
          },
        }]
      ]
    }
  ]
}