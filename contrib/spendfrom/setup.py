from distutils.core import setup
setup(name='sysspendfrom',
      version='1.0',
      description='Command-line utility for billiecoin "coin control"',
      author='Gavin Andresen',
      author_email='gavin@billiecoinfoundation.org',
      requires=['jsonrpc'],
      scripts=['spendfrom.py'],
      )
