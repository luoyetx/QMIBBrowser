#ifndef SNMPMANAGER_H
#define SNMPMANAGER_H

#include <snmp_pp/snmp_pp.h>

#include "common.h"

class SnmpManager {

public:
    /*Handle Request Operation 'GetRequest', 'GetNextRequest', 'SetRequest'*/
    static Status handleOperation(Request *requestt);
    /*Load MIB file*/
    //Status loadMIB(QString fileName, QTreeWidget *MIBTableWidget);

private:
    /*Operation GetRequest*/
    static Status handleOperationGet(Request *request);
    /*Operation GetNextRequest*/
    static Status handleOperationGetNext(Request *request);
    /*Operation SetRequest*/
    static Status handleOperationSet(Request *request);

};
#endif // SNMPMANAGER_H
