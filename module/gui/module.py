#
# Collective Knowledge (ck-gui commands)
#
# See CK LICENSE.txt for licensing details
# See CK COPYRIGHT.txt for copyright details
#

import os
import subprocess

cfg = {}  # Will be updated by CK (meta description of this module)
work = {} # Will be updated by CK (temporal data)
ck = None # Will be updated by CK (initialized CK kernel) 

class CKException(Exception):
  def __init__(self, ck_result):
    self.ck_result = ck_result

  @staticmethod
  def throw(message, code=1):
    raise CKException({'return': code, 'error': message})  


def ck_access(params_json, skip_error_codes = []):
  '''
  Performs call to ck-kernel and raises an exception when an error.
  Returns the result of ck-operation.
  '''
  r = ck.access(params_json)
  error_code = r['return']
  if error_code > 0 and not (error_code in skip_error_codes):
    ck.out('CK error details:')
    ck.out('    action: ' + params_json.get('action',''))
    ck.out('    param: module_uoa=' + params_json.get('module_uoa',''))
    ck.out('    param: data_uoa=' + params_json.get('data_uoa',''))
    import traceback
    stack_lines = traceback.format_stack()
    if len(stack_lines) >= 2:
      # The last line of the stack is the current line (`traceback.format_stack`),
      # so we need a line before the last - location of call of `ck_access` function.
      location_and_code = stack_lines[-2].strip().split('\n')
      ck.out('    location: ' + location_and_code[0].strip())
      if len(location_and_code) > 1:
        ck.out('    code: ' + location_and_code[1].strip())
    raise CKException(r)
  return r   


##############################################################################
# Initialize module

def init(i):
  """
  Input:  {}

  Output: {
            return       - return code =  0, if successful
                                       >  0, if error
            (error)      - error text if return > 0
          }
  """
  return {'return': 0}


##############################################################################
# Run ck-gui program

def run(i):
  """
  Input:  {
    recompile - compile program before run even though it is already compiled
    prebuilt  - try to run prebuilt package, install package if env is not found
          }
  Output: {
            return       - return code =  0, if successful
                                       >  0, if error
            (error)      - error text if return > 0
          }
  """
  try:
    if i.get('prebuilt') == 'yes':
      ck.out('Try to run prebuilt package ...')
      _run_prebuilt()
      
    else:
      ck.out('Try to compile and run program ...')
      _run_program(i)

  except CKException as e:
    return e.ck_result

  return {'return': 0}


def _run_program(i):
  r = ck_access({'action': 'search',
                 'module_uoa': 'program',
                 'add_meta': 'yes',
                 'tags': 'ck-gui'})
  programs = r['lst']

  if not programs:
   CKException.throw('No suitable programs found')

  if len(programs) > 1:
    # TODO: select a program to run
    CKException.throw('More than one program found. Can not choose a better one')

  program = programs[0]

  def is_program_compiled():
    program_file = program.get('meta',{}).get('target_file')
    program_path = os.path.join(program['path'], 'tmp', program_file)
    return os.path.isfile(program_path)

  if i.get('recompile') == 'yes' or not is_program_compiled():
    # TODO: need to clean tmp dir, otherwise make says: Nothing to be done for 'first'.
    ck_access({'action': 'compile',
               'module_uoa': 'program',
               'data_uoa': program['data_uoa']})

  ck_access({'action': 'run',
             'module_uoa': 'program',
             'data_uoa': program['data_uoa']})


def _run_prebuilt():
  # TODO: need to account platform when searching for env
  r = ck_access({'action': 'search',
                 'module_uoa': 'env',
                 'tags': 'tool,ck-gui,vprebuilt'})
  envs = r['lst']

  if not envs:
    ck.out('No suitable packages found')
    _install_prebuilt()
    _run_prebuilt()
    return

  if len(envs) > 1:
    # TODO: select a envs to load
    CKException.throw('More than one env found. Can not choose a better one')

  env = envs[0]

  ck.out('Run prebuilt from env {} ...'.format(env['data_uoa']))

  r = ck_access({'action': 'load',
                 'module_uoa': 'env',
                 'data_uoa': env['data_uoa']})

  tool_path = r.get('dict',{}).get('customize',{}).get('full_path')
  if not tool_path:
    CKException.throw('full_path to executable is not set in env {}'.format(env['data_uoa']))

  subprocess.call([tool_path])


def _install_prebuilt():
  ck.out('Try to install prebuilt package ...')

  r = ck_access({'action': 'search',
                 'module_uoa': 'package',
                 'tags': 'tool,ck-gui,vprebuilt'})
  packages = r['lst']

  if not packages:
   CKException.throw('No suitable packages found')

  if len(packages) > 1:
    # TODO: select a package to install
    CKException.throw('More than one package found. Can not choose a better one')
    
  package = packages[0]

  ck.out('Install package {} ...'.format(package['data_uoa']))

  ck_access({'action': 'install',
             'module_uoa': 'package',
             'data_uoa': package['data_uoa']})
