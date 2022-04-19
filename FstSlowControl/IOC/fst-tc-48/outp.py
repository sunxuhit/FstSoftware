
#output channel to the unit

from softioc.builder import mbbOut

#_____________________________________________________________________________
class outp():
    #_____________________________________________________________________________
    def __init__(self, nam, cmd, pv_type, com, multiply=1, init_cmd=None):

        #write command to the unit
        self.cmd = cmd

        #telnet to the unit
        self.com = com

        #multiply the output value if requested
        self.multiply = multiply

        #initial value for the pv
        ival = 0
        if init_cmd is not None:
            resp = self.com.put_cmd_sync(init_cmd+"0000")
            ival = resp
            if multiply != 1:
                ival = float(resp)/self.multiply

        #process variable for the output
        if pv_type == mbbOut:
            self.pv = pv_type(nam, ("text0", 0), ("text1", 1), ("text2", 2), ("text3", 3), on_update=self.on_update, initial_value=ival)
        else:
            self.pv = pv_type(nam, on_update=self.on_update, initial_value=ival)

    #_____________________________________________________________________________
    def on_update(self, val):

        val = val*self.multiply

        msg = "{0:04x}".format(int(val))

        self.com.put_cmd(self.cmd+msg)



