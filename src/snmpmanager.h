#ifndef SNMPMANAGER_H
#define SNMPMANAGER_H

#include <snmp_pp/snmp_pp.h>
#include <QTableWidget>

#include "common.h"

class SnmpManager {

public:
    SnmpManager();
    ~SnmpManager();
    /*Handle Request Operation 'GetRequest', 'GetNextRequest', 'SetRequest'*/
    Status handleOperation(Request *requestt);

private:
    /*Operation GetRequest*/
    Status handleOperationGet(Request *request);
    /*Operation GetNextRequest*/
    Status handleOperationGetNext(Request *request);
    /*Operation SetRequest*/
    Status handleOperationSet(Request *request);

};
#endif // SNMPMANAGER_H
