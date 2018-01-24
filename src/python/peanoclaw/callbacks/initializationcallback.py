'''
Created on Jan 29, 2013

@author: kristof
'''

from ctypes import c_bool
from ctypes import c_double
from ctypes import c_int
from ctypes import CFUNCTYPE
from ctypes import py_object

from peanoclaw.converter import get_number_of_dimensions

class InitializationCallback(object):
  '''
  This class encapsulates the callback for initializing a single subgrid.
  '''

  #Callback definition
  CALLBACK_INITIALIZATION = CFUNCTYPE(c_double, 
                                      py_object, #q
                                      py_object, #qbc
                                      py_object, #aux
                                      c_int,     #subdivision factor X0
                                      c_int,     #subdivision factor X1
                                      c_int,     #subdivision factor X2
                                      c_int,     #unknowns per cell
                                      c_int,     #aux fields per cell
                                      c_double, c_double, c_double, #size
                                      c_double, c_double, c_double, #position
                                      c_bool)    #skip q initialization


  def __init__(self, solver, refinement_criterion, q_initialization, aux_initialization, initial_minimal_mesh_width):
    '''
    Constructor
    '''
    self.solver = solver
    self.refinement_criterion = refinement_criterion
    self.q_initialization = q_initialization
    self.aux_initialization = aux_initialization
    self.initial_minimal_mesh_width = initial_minimal_mesh_width
    self.callback = None

  def get_initialization_callback(self):
    r"""
    Creates a closure for initializing the grid
    """
    def callback_initialization(q, qbc, aux, subdivision_factor_x0, subdivision_factor_x1, subdivision_factor_x2, unknowns_per_subcell, aux_fields_per_subcell, size_x, size_y, size_z, position_x, position_y, position_z, skip_q_initialization):
        import clawpack.pyclaw as pyclaw
        
        dim = get_number_of_dimensions(q)
        if dim is 2:
            self.dim_x = pyclaw.Dimension(position_x,position_x + size_x,subdivision_factor_x0, name='x')
            self.dim_y = pyclaw.Dimension(position_y,position_y + size_y,subdivision_factor_x1, name='y')
            domain = pyclaw.Domain([self.dim_x,self.dim_y])

        elif dim is 3:
            self.dim_x = pyclaw.Dimension(position_x,position_x + size_x,subdivision_factor_x0, name='x')
            self.dim_y = pyclaw.Dimension(position_y,position_y + size_y,subdivision_factor_x1, name='y')
            self.dim_z = pyclaw.Dimension(position_z,position_z + size_z,subdivision_factor_x2, name='z')
            domain = pyclaw.Domain([self.dim_x,self.dim_y,self.dim_z])
                
        subgrid_state = pyclaw.State(domain, unknowns_per_subcell, aux_fields_per_subcell)
        subgrid_state.q = q
        if(aux_fields_per_subcell > 0):
          subgrid_state.aux = aux
        subgrid_state.problem_data = self.solver.solution.state.problem_data
        
        if not skip_q_initialization:
          self.q_initialization(subgrid_state)

        if(self.aux_initialization != None and aux_fields_per_subcell > 0):
          self.aux_initialization(subgrid_state)
          
        #Steer refinement
        if self.refinement_criterion != None:
          return self.refinement_criterion(subgrid_state)
        else:
          return self.initial_minimal_mesh_width
   
    if not self.callback:
        self.callback = self.CALLBACK_INITIALIZATION(callback_initialization)

    return self.callback
