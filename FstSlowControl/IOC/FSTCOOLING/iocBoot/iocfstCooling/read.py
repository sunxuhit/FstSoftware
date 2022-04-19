#!/usr/bin/python

import telnetlib

#_____________________________________________________________________________
def main():

    print "Start"

    com = telnetlib.Telnet()
    com.set_debuglevel(3)
    com.open("130.199.61.23", 2000)

    stx = "*"
    etx = "\r"
    ack = "^"

    #msg = "010000"
    #msg = "590000"
    #msg = "5a0000"
    #msg = "5c0000"
    #msg = "5d0000"
    msg = "5600002B"
    #msg = "500000"
    #msg = "5f0000"

  #  cmd = stx + msg + get_checksum(msg) + etx
    cmd = stx + msg + etx
    print cmd

    #return

    for i in cmd:
        com.write(i)
        #resp = com.read_until(i)
        #print resp

    resp = com.read_until(ack)

    print resp

    com.close()

    print "Done"

#main

#_____________________________________________________________________________
def get_checksum(cmd=None):

    #cmd = "010000"
    #cmd = "1c0064"
    #cmd = "1cfff1"
    #cmd = "570000"

    #print [hex(ord(i)) for i in cmd]

    chsum = 0x0
    for i in cmd:
        #chsum += hex(ord(i))
        chsum += int(hex(ord(i)), 16)

    #print hex(chsum)

    return hex(chsum)[-2:]

    #print bin(chsum)
    #print bin(chsum)[-8:]
    #print hex(bin(chsum)[-8:])

    #print "ddd"

#get_checksum

#_____________________________________________________________________________
if __name__ == "__main__":

    main()

