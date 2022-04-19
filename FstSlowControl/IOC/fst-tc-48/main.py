#!/usr/local/epics/modules/pythonIoc/pythonIoc

from softioc import softioc, builder
from softioc.builder import aIn, boolIn, mbbIn, longIn, records, PP
from softioc.builder import aOut, boolOut, mbbOut, longOut

from tc48 import tc48

#tc48 instance
tc = tc48("130.199.61.23")

#read parameters
#name, command, type
#TC-48 setting, read
c = tc.add_read("FST:Cool:TC:Alarm1:DB:read", "61", aIn)
c.pv.PREC = 1
c.pv.LINR = "LINEAR"
c.divide = 10

c = tc.add_read("FST:Cool:TC:Alarm2:DB:read", "62", aIn)
c.pv.PREC = 1
c.pv.LINR = "LINEAR"
c.divide = 10

c = tc.add_read("FST:Cool:TC:AnalogOutMlt:read", "63", aIn)
c.pv.PREC = 2
c.pv.LINR = "LINEAR"
c.divide = 100
#c.pv.ESLO = 0.01

c = tc.add_read("FST:Cool:TC:PropBW:read", "61", aIn)
c.pv.PREC = 1
c.pv.LINR = "LINEAR"
c.divide = 10

c = tc.add_read("FST:Cool:TC:IGain:read", "52", aIn)
c.divide = 100
c.pv.PREC = 2
c.pv.LINR = "LINEAR"
#c.pv.ESLO = 0.01

c = tc.add_read("FST:Cool:TC:DGain:read", "53", aIn)
c.pv.PREC = 2
c.pv.LINR = "LINEAR"
c.divide = 100

c = tc.add_read("FST:Cool:TC:Alarm1:Type:read", "5b", boolIn)
c.pv.ZNAM = "No Effect"
c.pv.ONAM = "Load Off"

c = tc.add_read("FST:Cool:TC:Alarm2:Type:read", "5e", boolIn)
c.pv.ZNAM = "No Effect"
c.pv.ONAM = "Load Off"

c = tc.add_read("FST:Cool:TC:EEpromWrite:read", "65", boolIn)
c.pv.ZNAM = "OFF"
c.pv.ONAM = "ON"

c = tc.add_read("FST:Cool:TC:Mode:read", "55", boolIn)
c.pv.ZNAM = "Cooling"
c.pv.ONAM = "Heating"

c = tc.add_read("FST:Cool:TC:OutputEnable:read", "64", boolIn)
c.pv.ZNAM = "OFF"
c.pv.ONAM = "ON"

c = tc.add_read("FST:Cool:TC:SensorChoice:read", "54", boolIn)
c.pv.ZNAM = "15K"
c.pv.ONAM = "10K"

c = tc.add_read("FST:Cool:TC:Temp2Display:read", "60", mbbIn)
c.pv.ZRST = "Off"
c.pv.ONST = "Auto"
c.pv.TWST = "On"

c = tc.add_read("FST:Cool:TC:AlarmLatch:read", "5f", mbbIn)
c.pv.ZRST = "No Latches"
c.pv.ONST = "Alarm 1 Latch"
c.pv.TWST = "Alarm 2 Latch"
c.pv.THST = "Alarm 1+2 Latch"

## tc-48-temps read
c = tc.add_read("FST:Cool:TC:SetTemp:read", "50", aIn)
c.pv.PREC = 2
c.pv.LINR = "LINEAR"
c.divide = 10

c = tc.add_read("FST:Cool:TC:SensorOffset:read", "58", aIn)
c.pv.PREC = 2
c.pv.LINR = "LINEAR"
c.divide = 10
#c.pv.ESLO = 0.1
c.pv.MDEL = 0.1

c = tc.add_read("FST:Cool:TC:SecSensTemp", "04", aIn)
c.pv.PREC = 2
c.pv.LINR = "LINEAR"
c.divide = 10
#c.pv.ESLO = 0.1
c.pv.MDEL = 0.1

c = tc.add_read("FST:Cool:TC:LowSet:read", "56", aIn)
c.pv.PREC = 0

c = tc.add_read("FST:Cool:TC:HighSet:read", "57", aIn)
c.pv.PREC = 0

c = tc.add_read("FST:Cool:TC:ConSensTemp", "01", aIn)
c.pv.PREC = 2
c.pv.LINR = "LINEAR"
#contepm_pv = c.pv
c.divide = 10
#c.pv.ESLO = 0.1
c.pv.MDEL = 0.1

c = tc.add_read("FST:Cool:TC:Alarm2:Low:read", "5c", aIn)

c = tc.add_read("FST:Cool:TC:Alarm2:High:read", "5d", aIn)

c = tc.add_read("FST:Cool:TC:Alarm1:Low:read", "59", aIn)

c = tc.add_read("FST:Cool:TC:Alarm1:High:read", "5a", aIn)

#tc-48 misc
c = tc.add_read("FST:Cool:TC:ChillerPower", "02", aIn)
c.pv.PREC = 1
c.pv.LINR = "LINEAR"
c.pv.EGUF = 100
c.pv.EGU  = "Percent"
c.pv.HOPR = 100
#c.pv.ESLO = .195694
c.divide = 5.1100187
c.pv.ADEL = 0.1
c.pv.MDEL = 0.1

c = tc.add_read("FST:Cool:TC:revision","05", longIn)

c = tc.add_read("FST:Cool:TC:model","00",longIn)

# record(mbbiDirect,"FST:Cool:TC:AlarmStatus") {
# 	field(SCAN,".2 second")
# 	field(DTYP,"stream")
# 	field(NOBT,"8")
# 	field(INP,"@tc-48.proto AlarmStatus TC-48")
# }
c = tc.add_read_rec("FST:Cool:TC:AlarmStatus","03",records.mbbiDirect)
c.pv.NOBT = 8


#write parameters
#name, command, type, multiply, init_command
#TC-48 setting, write
c = tc.add_write("FST:Cool:TC:Alarm1:DB:write", "2d", aOut, 10, "61")
c.pv.PREC = 1
c.pv.LINR = "LINEAR"
c.pv.DRVH = 20
c.pv.DRVL = 0

c = tc.add_write("FST:Cool:TC:Alarm2:DB:write", "2e", aOut, 10, "62")
c.pv.PREC = 1
c.pv.LINR = "LINEAR"
c.pv.DRVH = 20
c.pv.DRVL = 0

c = tc.add_write("FST:Cool:TC:AnalogOutMlt:write", "2f", aOut, 100, "63")
c.pv.PREC = 2
c.pv.LINR = "LINEAR"
c.pv.DRVH = 1.0
c.pv.DRVL = 0

c = tc.add_write("FST:Cool:TC:PropBW:write", "1d", aOut, 10, "51")
c.pv.PREC = 1
c.pv.LINR = "LINEAR"
c.pv.DRVH = 100.0
c.pv.DRVL = 0.5

c = tc.add_write("FST:Cool:TC:IGain:write", "1e", aOut, 100, "52")
c.pv.PREC = 2
c.pv.LINR = "LINEAR"
c.pv.DRVH = 10.00
c.pv.DRVL = 0

c = tc.add_write("FST:Cool:TC:DGain:write", "1f", aOut, 100, "53")
c.pv.PREC = 2
c.pv.LINR = "LINEAR"
c.pv.DRVH = 10.00
c.pv.DRVL = 0.0

c = tc.add_write("FST:Cool:TC:Alarm1:Type:write", "27", boolOut, 1, "5b")
c.pv.ZNAM = "No Effect"
c.pv.ONAM = "Load Off"

c = tc.add_write("FST:Cool:TC:Alarm2:Type:write", "2a", boolOut, 1, "5e")
c.pv.ZNAM = "No Effect"
c.pv.ONAM = "Load Off"

c = tc.add_write("FST:Cool:TC:EEpromWrite:write", "31", boolOut, 1, "65")
c.pv.ZNAM = "OFF"
c.pv.ONAM = "ON"

c = tc.add_write("FST:Cool:TC:Mode:write", "21", boolOut, 1, "55")
c.pv.ZNAM = "Cooling"
c.pv.ONAM = "Heating"

c = tc.add_write("FST:Cool:TC:OutputEnable:write", "30", boolOut, 1, "64")
c.pv.ZNAM = "OFF"
c.pv.ONAM = "ON"

c = tc.add_write("FST:Cool:TC:SensorChoice:write", "20", boolOut, 1, "54")
c.pv.ZNAM = "15K"
c.pv.ONAM = "10K"

c = tc.add_write("FST:Cool:TC:AlarmLatch:write", "2b", mbbOut, 1, "5f")
c.pv.ZRST = "No Latches"
c.pv.ONST = "Alarm 1 Latch"
c.pv.TWST = "Alarm 2 Latch"
c.pv.THST = "Alarm 1+2 Latch"

c = tc.add_write("FST:Cool:TC:Temp2Display:write", "2c", mbbOut, 1, "60")
c.pv.ZRST = "Off"
c.pv.ONST = "Auto"
c.pv.TWST = "On"

#tc-48-temps write
c = tc.add_write("FST:Cool:TC:SetTemp:write", "1c", aOut, 10, "50")
c.pv.PREC = 1
c.pv.LINR = "LINEAR"
#c.pv.ESLO = 0.1
c.pv.DRVH = 80
c.pv.DRVL = 10

c = tc.add_write("FST:Cool:TC:SensorOffset:write", "24", aOut, 10, "58")
c.pv.PREC = 1
c.pv.LINR = "LINEAR"
#c.pv.ESLO = 0.1
c.pv.DRVH = 20
c.pv.DRVL = 0

c = tc.add_write("FST:Cool:TC:LowSet:write", "22", aOut, 1, "56")
c.pv.PREC = 0
c.pv.DRVH = 100
c.pv.DRVL = 0

c = tc.add_write("FST:Cool:TC:HighSet:write", "23", aOut, 1, "57")
c.pv.PREC = 0
c.pv.DRVH = 100
c.pv.DRVL = 0

c = tc.add_write("FST:Cool:TC:Alarm2:Low:write", "28", aOut, 1, "5c")
c.pv.DRVH = 100
c.pv.DRVL = 0

c = tc.add_write("FST:Cool:TC:Alarm2:High:write", "29", aOut, 1, "5d")
c.pv.DRVH = 100
c.pv.DRVL = 0

c = tc.add_write("FST:Cool:TC:Alarm1:Low:write", "25", aOut, 1, "59")
c.pv.DRVH = 100
c.pv.DRVL = 0

c = tc.add_write("FST:Cool:TC:Alarm1:High:write", "26", aOut, 1, "5a")
c.pv.DRVH = 100
c.pv.DRVL = 0

#c = tc.add_write(mbbo,"FST:Cool:HeatCapacitySelect",,mbbOut, 1)
pv = mbbOut("FST:Cool:HeatCapacitySelect", ("text0", 0), ("text1", 1), ("text2", 2), ("text3", 3))
pv.PINI = "YES"
pv.DTYP = "Soft Channel"
pv.DOL = 0
pv.ZRVL = 0
pv.ONVL = 1
pv.ZRST = "Water"
pv.ONST = "Novek"

c = tc.add_write("FST:Cool:TC:AlarmLatch:Clear", "33", boolOut, 1)

#_________________________________
# record(calc,"FST:Cool:TC:HeatLoad") {
# 	field(SCAN,"1 second")
# 	field(CALC,"((C+D)/2)*(B-A)*(E?F:G)")
# 	field(INPA,"FST:Cool:TC:ConSensTemp.VAL NPP MS")
# 	field(INPB,"FST:Cool:TC:SecSensTemp.VAL NPP MS")
# 	field(INPC,"FST:Cool:Flow:supply.VAL NPP MS")
# 	field(INPD,"FST:Cool:Flow:return.VAL NPP MS")
# 	field(INPE,"FST:Cool:HeatCapacitySelect.VAL NPP MS")
# 	field(INPF,"28.918")
# 	field(INPG,"69.783")
# 	field(EGU,"Watts")
# 	field(PREC,"0")
# 	field(HOPR,"400")
# 	field(ADEL,"1")
# 	field(MDEL,"1")
# }

def make_heatload_calc(name,inpa,inpb,inpc,inpd,inpe,cexp):
    cx = records.calc(name, CALC=cexp, PREC=0, HOPR=400)
    cx.INPA = inpa.name + " MS"
    cx.INPB = inpb.name + " MS"
    cx.INPC = inpc.name + " MS"
    cx.INPD = inpd.name + " MS"
    cx.INPE = inpe.name + " MS"
    cx.INPF = "28.918"
    cx.INPG = "69.783"
    inpa.FLNK = PP(cx)
    inpb.FLNK = PP(cx)
    inpc.FLNK = PP(cx)
    inpd.FLNK = PP(cx)
    inpe.FLNK = PP(cx)
#    inpf.FLNK = PP(cx)
#    inpg.FLNK = PP(cx)
    cx.EGU = "Watts"
    cx.ADEL = 1

def make_heatload_calc_names(name,anam,bnam,cnam,dnam,enam,cexp):
    cx = records.calc(name, CALC=cexp, PREC=0, HOPR=400, SCAN = "1 second")
    # cx = records.calc(name, CALC=cexp)
    # cx.pv.PREC = 0
    # cx.pv.HOPR = 400
    # cx.pv.SCAN = "1 second"
    cx.INPA = anam + " NPP MS"
    cx.INPB = bnam + " NPP MS"
    cx.INPC = cnam + " NPP MS"
    cx.INPD = dnam + " NPP MS"
    cx.INPE = enam + " NPP MS"
    cx.INPF = "28.918"
    cx.INPG = "69.783"
    cx.EGU = "Watts"
    cx.ADEL = 1
    return cx

c = make_heatload_calc_names("FST:Cool:TC:HeatLoad","FST:Cool:TC:ConSensTemp","FST:Cool:TC:SecSensTemp","FST:Cool:Flow:supply","FST:Cool:Flow:return","FST:Cool:HeatCapacitySelect","((C+D)/2)*(B-A)*(E?F:G)")


def make_blink_calc(name):
    cx = records.calc(name, CALC="!A", SCAN = "1 second")
    cx.INPA = name + " NPP NMS"
c = make_blink_calc("FST:Cool:TC:AlarmStatus:blinker")

#load the ioc
builder.LoadDatabase()
softioc.iocInit()

#tc.update()

#start the monitoring thread
tc.daemon = True
tc.start()

#open the ioc shell
softioc.interactive_ioc(globals())


