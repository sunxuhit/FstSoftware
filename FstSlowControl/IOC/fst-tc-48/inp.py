
#input channel from the unit

from softioc.builder import mbbIn

#_____________________________________________________________________________
class inp():
    #_____________________________________________________________________________
    def __init__(self, nam, cmd, pv_type, com):

        #read command to the unit
        self.cmd = cmd+"0000"

        #process variable for the input
        if pv_type == mbbIn:
            self.pv = mbbIn(nam, ("text0", 0), ("text1", 1), ("text2", 2), ("text3", 3))
        else:
            self.pv = pv_type(nam)

        #telnet to the unit
        self.com = com

        #divide the return value if requested
        self.divide = None

        #self.nam = nam

    #_____________________________________________________________________________
    def update(self):

        resp = self.com.put_cmd_sync(self.cmd)

        if self.divide is not None:
            resp = resp/float(self.divide)

        #if self.nam == "FST:Cool:TC:Temp2Display:read":
            #print resp, type(resp)
            #self.pv.set(2)
            #return

        self.pv.set(resp)

