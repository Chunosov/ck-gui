#
# Collective Knowledge (ck-gui commands)
#
# See CK LICENSE.txt for licensing details
# See CK COPYRIGHT.txt for copyright details
#

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
          }

  Output: {
            return       - return code =  0, if successful
                                       >  0, if error
            (error)      - error text if return > 0
          }
  """
  try:
    r = ck_access({'action': 'search',
                   'module_uoa': 'program',
                   'tags': 'ck-gui'})
    programs = r['lst']

    if not programs:  
     CKException.throw('No suitable programs found')
     
    if len(programs) > 1:
      # TODO: select a program to run
      CKException.throw('More than one program found. Can not choose a better one')
      
    program = programs[0]

    ck_access({'action': 'compile',
               'module_uoa': 'program',
               'data_uoa': program['data_uoa']})
    
    ck_access({'action': 'run',
               'module_uoa': 'program',
               'data_uoa': program['data_uoa']})

  except CKException as e:
    return e.ck_result 

  return {'return': 0}
