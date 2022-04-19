
#input channel from the unit for use with builder.records pvs

from epics import PV

#_____________________________________________________________________________
class inp_rec():
    #_____________________________________________________________________________
    def __init__(self, nam, cmd, pv_type, com):

        #read command to the unit
        self.cmd = cmd+"0000"

        #process variable for the input
        self.pv = pv_type(nam)
        self._pv = PV(nam)
        #self._pv_sevr = PV(nam+".SEVR")

        #telnet to the unit
        self.com = com

        #divide the return value if requested
        self.divide = None

        self.nam = nam

    #_____________________________________________________________________________
    def update(self):

        resp = self.com.put_cmd_sync(self.cmd)

        if self.divide is not None:
            resp = resp/float(self.divide)

        #if self.nam == "FST:Cool:TC:AlarmStatus":
            #print resp, type(resp)
            #self.pv.set(2)
            #return

        self._pv.put(resp)



