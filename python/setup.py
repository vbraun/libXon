from distutils.core import setup
from distutils.extension import Extension
from Cython.Distutils import build_ext

setup(
    cmdclass = {'build_ext': build_ext},
    ext_modules = [
        Extension('message_passing.message', 
                  language='c++',
                  sources=['message_passing/message.pyx'],
                  libraries=['xon-objectxx']
                  )
        ]
    )

