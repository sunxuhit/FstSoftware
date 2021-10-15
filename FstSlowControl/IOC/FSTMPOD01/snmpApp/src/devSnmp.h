/*
  devSnmp.h

  Original DESY 1.03 version substantially re-written for NSCL
  by J.Priller
*/
#ifndef	DEVSNMP_H
#define	DEVSNMP_H

#include <math.h>
#include <devSup.h>
#include <menuScan.h>
#include <dbScan.h>
#include <epicsTime.h>
#include <epicsThread.h>

#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>

/*
  Uncomment line below for devices to keep and (eventually) send out
  each setting sent to them by record support.

  Leave commented-out to remember only the most recent setting,
  discarding any earlier setting(s) made since we last processed
  settings for a given device.  This improves performance when record
  support sends us a whole batch of settings in a short time (an
  operator is turning a knob, for example), as only the most recent is
  actually sent out.
*/
// #define KEEP_MULTIPLE_SETTINGS

//
// define default maximum for oids per get request
// can be overridden on a per-host basis by IOC shell call to
// epicsSnmpSetMaxOidsPerReq
//
// Wiener/ISEG crates can generally handle 50, but we use 32 as
// a default because not all SNMP devices can handle that many
//
#define DEFAULT_MAX_OIDS_PER_REQ  32

typedef unsigned long SNMP_UINT32;
typedef unsigned long long SNMP_UINT64;
typedef long SNMP_INT32;
typedef long long SNMP_INT64;

struct __oid {
  const char *Name;
  oid Oid[MAX_OID_LEN];
  unsigned int OidLen;
};

typedef struct snmp_session SNMP_SESSION;
typedef struct snmp_pdu SNMP_PDU;
typedef struct __oid OID;

// forward declarations
class devSnmp_manager;
class devSnmp_host;
class devSnmp_group;
class devSnmp_device;
class devSnmp_session;
class devSnmp_getTransaction;
class devSnmp_setTransaction;
class devSnmp_hostversion;

typedef long (*DEVSNMP_DEVFUNC)(devSnmp_device *pDevice);

//----------------------------------------------------------------------
// define a simple class for maintaining a dynamic list of pointers
typedef int (*POINTERLIST_COMPARE_PROC)(void *p1, void *p2);

class pointerList
{
  public:
    pointerList(int initialCount = 1024, int extendCount = 1024);
    virtual ~pointerList(void);
    void append(void *ptr);
    int count(void);
    void *itemAt(int idx);
    void *removeItemAt(int idx);
    void sort(POINTERLIST_COMPARE_PROC proc);

    void *context(void);
    void setContext(void *newcontext);

  protected:
    epicsMutexId  mutex;
    int           delta;
    int           currSize;
    int           currCount;
    void        **pPtrList;
    void         *_context;
};
//----------------------------------------------------------------------
// define a class for storing time info, measuring elapsed time
class timeObject
{
  public:
    timeObject(void);
    virtual ~timeObject(void);
    void start(void);
    void clear(void);
    bool started(void);
    double elapsedSeconds(void);
    double elapsedMilliseconds(void);
    void toDateTimeString(char *str);

  protected:
    epicsTimeStamp lastStarted;
};
//----------------------------------------------------------------------
class devSnmp_group
/* one of these structures exists for each host/community
   (things for which we can pack GET requests into one session) */
{
  public:
    devSnmp_group(devSnmp_manager *pMgr, devSnmp_host *host, char *community, bool *okay);
    virtual ~devSnmp_group(void);

    void addDevice(devSnmp_device *pDevice);

    void processing(void);

    const char *hostName(void);
    const char *communityName(void);
    int snmpVersion(void);

    void setMaxOidsPerReq(int maxoids);
    int getMaxOidsPerReq(void);

    void zeroCounters(void);
    void report(int level, char *match);

    void sessionSent(int state);
    void sessionGotReply(devSnmp_session *pSession);
    void sessionGotError(devSnmp_session *pSession);

    SNMP_SESSION *getBaseSession(void);

  protected:
    devSnmp_manager *pOurMgr;
    devSnmp_host    *pOurHost;
    SNMP_SESSION    *base_session;
    pointerList     *deviceList;
    pointerList     *weightList;
    double           bestReplyMsec;
    double           worstReplyMsec;
    double           avgReplyMsec;
    long             sendCount;
    long             replyCount;
    long             errorCount;
    int              maxOidsPerReq;
};
//----------------------------------------------------------------------
class devSnmp_session
/* one of these structures exists for each active session, GET or SET,
   kept track of in DEVSNMP_GROUP item's activeSessionList list */
{
  public:
    devSnmp_session(devSnmp_manager *pMgr, devSnmp_group *pGroup, bool is_set);
    virtual ~devSnmp_session(void);
    bool open(SNMP_SESSION *psess);
    bool send(void);
    void close(void);
    int itemCount(void);
    bool isCompleted(void);
    bool wasSent(void);
    bool isSetting(void);
    double secondsSinceCreated(void);
    double secondsSinceSent(void);

    void addReading(devSnmp_device *pDevice, const oid *Oid, size_t size);
    void addSetting(devSnmp_device *pDevice, const oid *Oid, size_t size, char type, const char *value);

    int replyProcessing(int op, SNMP_SESSION *sp, int reqId, SNMP_PDU *pdu);

    SNMP_SESSION *getSession(void);

  protected:
    devSnmp_manager *pOurMgr;
    devSnmp_group   *pOurGroup;
    SNMP_SESSION    *session;
    SNMP_PDU        *pdu;
    timeObject       timeStarted;
    timeObject       timeSent;
    pointerList     *deviceList;
    bool             is_setting;
    bool             completed;
    bool             sent;
    bool             tried_send;
};
//----------------------------------------------------------------------
class devSnmp_device
// one of these structures exists for each PV
{
  public:
    // how many milliseconds since last call to declareValueValid() before
    // validTimeout() returns TRUE
    static const int ValidTimeoutMSec = 10000;

    // millisecond readback poll gap for Passive records (such as output records)
    static const int PassivePollGapMSec = 2000;

    // how long to skip readback/record->val compare for output records after
    // a setting to the record has been made (prevents value 'flicker')
    static const int SetSkipReadbackMSec = 4000;

    // maximum poll weight of lowest-weight device in a group to indicate
    // a poll of devices in the group should be made
    static const int MaxTopPollWeight = 10;

    // poll weight that flags device should NOT be polled
    static const int DoNotPollWeight = 1000;

    devSnmp_device(devSnmp_manager *pMgr,
                   devSnmp_group   *pGroup,
                   struct dbCommon *pRec,
                   struct link     *pLink,
                   char            *type,
                   bool            *okay);
    virtual ~devSnmp_device(void);

    bool getValueString(char *str, int maxsize);
    bool getRawValueString(char *str, int maxsize);

    int getDataLength(void);
    void setDataLength(int length);

    void declareValueValid(void);
    bool validTimeout(void);

    int getPollGapMSec(void);
    void setPollGapMSec(int msec);

    double pollsPerSecond(void);

    void processRecord(void);
    bool doingProcess(void);

    void set(char *str);
    bool wasSetRecently(void);
    bool needsSet(void);
    char *getNextSetting(void);
    char getSetType(void);

    void recalcPollWeight(void);
    int getPollWeight(void);

    void setGetQueued(bool state);

    OID *getOid(void);

    void setPeriodicCallback(DEVSNMP_DEVFUNC procFunc, double seconds);

    devSnmp_manager *getManager(void);
    devSnmp_group *getGroup(void);
    void setGroup(devSnmp_group *pGroup);

    void settingSendStatus(bool state);
    void readingSendStatus(bool state);

    void periodicProcessing(void);
    void replyProcessing(devSnmp_session       *pSession,
                         int                    op,
                         long                   errCode,
                         netsnmp_variable_list *var);

    const char *hostName(void);
    const char *communityName(void);
    const char *recordName(void);
    struct dbCommon *record(void);

    void zeroCounters(void);
    void report(int level);

  protected:
    devSnmp_manager  *pOurMgr;
    devSnmp_group    *pOurGroup;
    OID               oid;
    char             *host;
    char             *community;
    char             *read_data;
    char             *our_type;
    int               data_len;
    epicsMutexId      valMutex;
    char             *mask;
    bool              validFlag;
    struct dbCommon  *pRecord;
    DEVSNMP_DEVFUNC   pPeriodicFunction;
    double            periodicSeconds;
    timeObject        lastPeriodicFuncCall;
    bool              has_reading;
    char              set_type;
#ifdef KEEP_MULTIPLE_SETTINGS
    pointerList      *settingsQueue;
#else
    epicsMutexId      setMutex;
    char             *setting_to_send;
#endif
    int               flagged_read_bad;
    bool              in_rec_process;
    int               pollGapMSec;
    double            desiredPollsPerSec;
    timeObject        lastValidFlagSet;
    timeObject        pollStart;
    timeObject        lastSetSent;
    timeObject        lastPollSent;
    timeObject        lastPollReply;
    unsigned long     pollSendCount;
    unsigned long     pollReplyCount;
    unsigned long     errorCount;
    bool              queued_for_get;
    int               pollWeight;

    void setReplyProcessing(devSnmp_session       *pSession,
                            int                    op,
                            long                   errCode,
                            netsnmp_variable_list *var);

    void readReplyProcessing(devSnmp_session       *pSession,
                             int                    op,
                             long                   errCode,
                             netsnmp_variable_list *var);

};
//----------------------------------------------------------------------
class devSnmp_transaction
// base class for SNMP transactions we'll perform (get and set)
{
  public:
    devSnmp_transaction(bool isSet);
    virtual ~devSnmp_transaction(void);
    bool isSetting(void);
    double millisecondsSinceCreated(void);
    virtual devSnmp_session *createSession(void) = 0;

  protected:
    bool       is_set;
    timeObject createTime;
};
//----------------------------------------------------------------------
class devSnmp_getTransaction : public devSnmp_transaction
// class to hold data for an SNMP 'get' transaction'
// allows multiple elements per request, up to MaxItems
{
  public:
    devSnmp_getTransaction(int MaxItems);
    virtual ~devSnmp_getTransaction(void);
    void addDevice(devSnmp_device *pDevice);
    bool isFull(void);
    int count(void);
    virtual devSnmp_session *createSession(void);

  protected:
    pointerList *deviceList;
    int ourMaxItems;
};
//----------------------------------------------------------------------
class devSnmp_setTransaction : public devSnmp_transaction
/*
   class to hold data for an SNMP 'set' transaction'
   allows only one element per request, as SNMP set replies don't return
   which element triggered the error if one of them causes an error
*/
{
  public:
    devSnmp_setTransaction(devSnmp_device *pDevice);
    virtual ~devSnmp_setTransaction(void);
    virtual devSnmp_session *createSession(void);

  protected:
    devSnmp_device *ourDevice;
    char           *ourDeviceSetting;
};
//----------------------------------------------------------------------
class devSnmp_host
// one of these exists for every SNMP remote host
{
  public:
    // how old in milliseconds the oldest 'get' transaction queued for a
    // host can be before 'set' transactions no longer have priority over it
    static const int GetWaitedTooLongMSec = 1000;

    devSnmp_host(devSnmp_manager *pMgr, char *host, bool *okay);
    virtual ~devSnmp_host(void);
    char *hostName(void);
    void addDevice(devSnmp_device *pDevice);
    void processing(void);
    void zeroCounters(void);
    void report(int level, char *match);
    void queueGetTransaction(devSnmp_getTransaction *pTrans);
    void queueSetTransaction(devSnmp_setTransaction *pTrans);
    void setMaxOidsPerReq(int maxoids);
    int getMaxOidsPerReq(void);

  protected:
    devSnmp_manager *pOurMgr;
    char            *hostname;
    pointerList     *snmpGroupList;
    pointerList     *getQueue;
    pointerList     *setQueue;
    pointerList     *activeSessionList;
    int              maxOidsPerReq;

    devSnmp_group *createGroup(char *community);
};
//----------------------------------------------------------------------
class devSnmp_hostdata
// one of these structures is created when the IOC shell calls
// epicsSnmpSetSnmpVersion or epicsSnmpSetMaxOidsPerReq to override the
// default SNMP version for the given host (SNMP_VERSION_2c) or the default
// maximum number of oids per request (devSnmp_getTransaction::MaxItems)
{
  public:
    devSnmp_hostdata(char *host);
    virtual ~devSnmp_hostdata(void);

    const char *hostName(void);

    void setSnmpVersion(int version);
    int snmpVersion(void);

    void setMaxOidsPerReq(int maxoids);
    int maxOidsPerReq(void);

  protected:
    char *ourHostName;
    int   ourSnmpVersion;
    int   ourMaxOidsPerReq;
};
//----------------------------------------------------------------------
class devSnmp_manager
// object to manage other devSnmp_xxx objects
// devSnmp has one and only one of these
{
  public:
    // sleep time for read and send tasks
    static const int ThreadSleepMsec = 10;

    devSnmp_manager(void);
    virtual ~devSnmp_manager(void);
    void setHostSnmpVersion(char *host, char *versionStr);
    int getHostSnmpVersion(char *host);
    int getHostMaxOidsPerReq(char *host);
    void setMaxOidsPerReq(char *host, int maxoids);
    devSnmp_device *createDevice(struct dbCommon *pRec, struct link *pLink, char *type);
    void processing(void);
    void zeroCounters(void);
    void report(int level, char *match);

    int start(void);
    int readTask(void);
    int sendTask(void);

    void sessionMutexLock(void);
    void sessionMutexUnlock(void);

    void incActiveRequests(void);
    void decActiveRequests(void);

  protected:
    pointerList   *snmpHostList;
    pointerList   *snmpHostDataList;
    bool           started;
    bool           snmpSendTaskAbort;
    bool           snmpReadTaskAbort;
    epicsMutexId   sessionMutex;
    epicsThreadId  readTaskId;
    epicsThreadId  sendTaskId;
    int            activeRequests;
    timeObject     readTaskStart;
    timeObject     sendTaskStart;
    unsigned long  readTaskLoops;
    unsigned long  sendTaskLoops;

    devSnmp_host *createHost(char *host);
    devSnmp_host *findHost(char *host);
    devSnmp_hostdata *findHostData(char *host);
};
//----------------------------------------------------------------------
class pollAggregate
// class to generate average poll times report for a number of devices
{
  public:
    pollAggregate(void);
    virtual ~pollAggregate(void);
    void addDevice(devSnmp_device *pDevice);
    void report(void);

  protected:
    pointerList *lists;

    pointerList *findList(unsigned long msec);
};
//----------------------------------------------------------------------

extern "C" {

  typedef struct snmpTypeConvStruct
  {
     char *str;
     int (*func)(void*, char*);
     size_t size;
  } SNMP_TYPE_CONV;

  static epicsStatus snmpWfCntr32Convert(void *rval, char *sval);
  static epicsStatus snmpWfCntr64Convert(void *rval, char *sval);
  static epicsStatus snmpWfGauge32Convert(void *rval, char *sval);
  static epicsStatus snmpWfGauge64Convert(void *rval, char *sval);
  static epicsStatus snmpWfTimeTicksConvert(void *rval, char *sval);
  static epicsStatus snmpWfHexStrConvert(void *rval, char *sval);
  static epicsStatus snmpWfNetAddrConvert(void *rval, char *sval);
  static epicsStatus snmpWfIntConvert(void *rval, char *sval);
  static epicsStatus snmpWfStrConvert(void *rval, char *sval);
  static epicsStatus snmpWfIpAddrConvert(void *rval, char *sval);

  SNMP_TYPE_CONV wfTypes[] =
  {
     { "Counter32",       snmpWfCntr32Convert,    sizeof(unsigned int)  },
     { "Counter64",       snmpWfCntr64Convert,    sizeof(SNMP_UINT64)   },
     { "Gauge32",         snmpWfGauge32Convert,   sizeof(unsigned int)  },
     { "Gauge64",         snmpWfGauge64Convert,   sizeof(SNMP_UINT64)   },
     { "Timeticks",       snmpWfTimeTicksConvert, sizeof(unsigned long) },
     { "Hex-STRING",      snmpWfHexStrConvert,    (size_t) 0            },
     { "Network Address", snmpWfNetAddrConvert,   (size_t) 0            },
     { "INTEGER",         snmpWfIntConvert,       sizeof(int)           },
     { "STRING",          snmpWfStrConvert,       (size_t) 0            },
     { "IpAddress",       snmpWfIpAddrConvert,    (size_t) 0            },
     { NULL,              NULL,                   (size_t) 0            }
  };

}  // extern "C"

#endif  // DEVSNMP_H





