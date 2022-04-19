
#TC-48 unit

from threading import Thread, Timer
from time import sleep
import atexit

from softioc import alarm
#from epics import caput

from tcom import tcom
from inp import inp
from inp_rec import inp_rec
from outp import outp

#_____________________________________________________________________________
class tc48(Thread):
    #_____________________________________________________________________________
    def __init__(self, ip):
        Thread.__init__(self)

        #update periodicity
        self.update_period = 3 # sec

        #input and output channels
        self.inps = []
        self.outs = []

        #telnet connection to the unit
        self.com = tcom(ip)
        atexit.register(self.close_com)

        #timer to detect no updates from the unit
        self.timeout = 60 # sec
        self.timer = Timer(self.timeout, self.set_invalid)
        self.timer.start()

    #_____________________________________________________________________________
    def add_read(self, nam, cmd, pv_type):

        #add input channel
        self.inps.append(inp(nam, cmd, pv_type, self.com))

        return self.inps[-1]

    #_____________________________________________________________________________
    def add_read_rec(self, nam, cmd, pv_type):

        #add input channel
        self.inps.append(inp_rec(nam, cmd, pv_type, self.com))

        return self.inps[-1]

    #_____________________________________________________________________________
    def add_write(self, nam, cmd, pv_type, multiply=1, init_cmd=None):

        #add output channel
        self.outs.append(outp(nam, cmd, pv_type, self.com, multiply, init_cmd))

        return self.outs[-1]

    #_____________________________________________________________________________
    def update(self):

        #update the input channels
        for i in self.inps:
            i.update()

        self.reset_timer()

    #_____________________________________________________________________________
    def run(self):

        #periodically run the update via the Thread
        while True:
            self.update()
            sleep(self.update_period)

    #_____________________________________________________________________________
    def close_com(self):

        print "Telnet close"

        #close the telnet
        self.com.com.close()

    #_____________________________________________________________________________
    def set_invalid(self):

        #mark all PVs as invalid
        for i in self.inps:

            if hasattr(i.pv, "set_alarm"):
                i.pv.set_alarm(alarm.INVALID_ALARM, alarm.TIMEOUT_ALARM)

           # else:
                #caput(i.nam+".SEVR", 3)
                #print "Alarm for: ", i.nam
                #i._pv_sevr.put(3, use_complete=True)


    #_____________________________________________________________________________
    def reset_timer(self):

        self.timer.cancel()
        self.timer = Timer(self.timeout, self.set_invalid)
        self.timer.start()
        

