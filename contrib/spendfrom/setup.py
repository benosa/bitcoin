from distutils.core import setup
setup(name='tlrspendfrom',
      version='1.0',
      description='Command-line utility for thalercoin "coin control"',
      author='Gavin Andresen',
      author_email='gavin@thalercoinfoundation.org',
      requires=['jsonrpc'],
      scripts=['spendfrom.py'],
      )
