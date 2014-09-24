#!/usr/bin/python

import os, time
from subprocess import call


# pixel analysis input
pix_dir    = '/home/diamond/Testbeam_September2014/DHidasPLT-devel/'
pix_exe    = './TestPSIBinaryFileReader'
pix_rundir = '/home/diamond/Testbeam_September2014/runs-september/'


# pad analysis input
pad_exe    = '/home/diamond/Testbeam_September2014/Dmitry-devel/padreadout-devel/decode'
pad_rundir = '/home/diamond/Testbeam_September2014/drs4_data/'



def age(file):
  return time.time() - os.path.getmtime(file)

analyzedRuns = []

pix_runls = os.listdir(pix_rundir)

while True:

  for run in pix_runls:
    num = run[-6:]
    if run in analyzedRuns: continue ## don't recheck all the done ones
    print '^^^^^^^^^^^^^^ at run %s ^^^^^^^^^^^^^^^^\n\n' %(num)
    analyzepix = True
    if os.path.isdir(pix_dir+'plots/'+num):
      print 'pixel run already analyzed!'
      analyzepix = False
    else:
      pix_file = pix_rundir+run+'/mtb.bin'
      if age(pix_file) < 3000.:
        print 'pixel file is too young, skipping this for now...'
        analyzepix = False
  
  
    pad_file = pad_rundir+'run_2014_09r'+num+'.dat'
    analyzepad = True
    if not os.path.isfile(pad_file):
      print 'could not find pad run number:', num
      analyzepad = False
    else:
      print 'found pad file for run', num, 'named', pad_file
      if os.path.isfile(pad_file.replace('.dat','.root')):
        print 'pad run already analyzed, file', pad_file.replace('.dat','.root'), 'exists'
        analyzepad = False
      if age(pad_file) < 3000.:
        print 'pad file is too young, skipping this for now...'
        analyzepad = False
  
    ## pixel analysis
  
    if analyzepix and analyzepad: # analyze only runs for which we have both pad and pixel data
      print '\n\n'
      print 'starting pixel analysis of run number', num
      print '-------------------------------------'
  
      pix_opt = pix_file+' 0 5'
      print 'executing pixel analysis command:', pix_exe, pix_opt
      #go to the pixel directory
      curr_path = os.getcwd()
      os.chdir(pix_dir)
      try:
        call(pix_exe+' '+pix_opt, shell=True)
      except OSError as e:
        print 'ERROR!!!!', e.strerror, 'moving on'
      # go back to the script directory
      os.chdir(curr_path)
    
    ## if analyzepad:
      print '\n\n'
      print 'starting pad analysis of run number', num
      print '-------------------------------------'
  
      pad_opt = ' -i '+ pad_file
      print 'executing pad analysis command:', pad_exe, pad_opt
      try:
        call(pad_exe+pad_opt, shell=True)
      except OSError as e:
        print 'ERROR!!!!', e.strerror, 'moving on'
  analyzedRuns.append(run)
  time.sleep(10)
    
  
