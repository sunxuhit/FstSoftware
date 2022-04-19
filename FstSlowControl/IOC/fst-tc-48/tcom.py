
#telnet connection to the unit

import telnetlib
import time

class tcom:
  #_____________________________________________________________________________
  def __init__(self, ip):
    #commands queue
    self.queue = []
    #busy flags
    self.busy = False
    self.asyn = False
    self.in_put_cmd = False
    #open the telnet connection
    self.com = telnetlib.Telnet()
    self.com.open(ip, 2000)
    #protocol constants
    self.stx = "*"
    self.etx = "\r"
    self.ack = "^"

  #_____________________________________________________________________________
  def put_cmd(self, cmd=""):
    #asynchronous command
    if cmd != "": self.queue.append(cmd)
    #only one instance since now
    if self.in_put_cmd == True: return
    #lock the instance
    self.in_put_cmd = True
    #wait for synchronous command to finish
    while self.busy == True: time.sleep(0.1)
    #process the command queue
    while len(self.queue) > 0:
      #skip if synchronous command was issued
      if self.busy == True:
        break
      #raise asyn bysy flag
      self.asyn = True
      self.run_cmd(self.queue.pop(0))
      self.asyn = False
    #unlock the instance
    self.in_put_cmd = False

  #_____________________________________________________________________________
  def put_cmd_sync(self, cmd):
    #synchronous command, expected to run at regular intervals
    self.busy = True
    #wait for asynchronous command in proggress
    while self.asyn == True: time.sleep(0.1)
    #process the command, capture the response
    resp = self.run_cmd(cmd)
    #print resp
    self.busy = False
    #finish any asynchronous commands
    self.put_cmd()
    return int(resp[1:5], 16)

  #_____________________________________________________________________________
  def run_cmd(self, msg):
    #put command to telnet
    cmd = self.stx + msg + self.get_checksum(msg) + self.etx
    #print cmd

    for i in cmd:
        self.com.write(i)

    return self.com.read_until(self.ack)

  #_____________________________________________________________________________
  def get_checksum(self, msg):

    chsum = 0x0
    for i in msg:
        chsum += int(hex(ord(i)), 16)

    return hex(chsum)[-2:]

