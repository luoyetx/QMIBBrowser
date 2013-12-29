#include <QDebug>
#include <QMessageBox>

#include "snmpmanager.h"

Status SnmpManager::handleOperation(Request *request)
{
    Status status;
    switch (request->operation) {
    case OperationGet:
        status = handleOperationGet(request);
        break;
    case OperationGetNext:
        status = handleOperationGetNext(request);
        break;
    case OperationSet:
        status = handleOperationSet(request);
        break;
    default:
        status = Status_FAILED;
        break;
    }
    return status;
}

Status SnmpManager::handleOperationGet(Request *request)
{
    Snmp::socket_startup();
    int status;
    Snmp snmp(status, 0, false);
    if (status != SNMP_CLASS_SUCCESS) {
        /*Setup SNMP FAILED*/
        Helper::log(0, "RequestGet Failed");
        Helper::log(1, snmp.error_msg(status));
        Helper::pop("Error", snmp.error_msg(status));
        Snmp::socket_cleanup();
        return Status_FAILED;
    }
    Pdu pdu;
    pdu += request->data;
    CTarget ctarget(request->address);
    ctarget.set_retry(request->retry);
    ctarget.set_timeout(request->timeout);
    ctarget.set_version(request->version);
    ctarget.set_readcommunity(request->community.c_str());
    SnmpTarget *target = &ctarget;
    status = snmp.get(pdu, *target);
    if (status == SNMP_CLASS_SUCCESS) {
        /*GetRequest SUCCESS*/
        pdu.get_vb(request->data, 0);
        Helper::log(1, "RequestGet SUCCESS");
        QString replayOid = request->data.get_printable_oid();
        QString replayValue = request->data.get_printable_value();
    }
    else {
        /*GetRequest FAILED*/
        Helper::log(0, "RequestGet Failed");
        Helper::log(1, snmp.error_msg(status));
        Helper::pop("Error", snmp.error_msg(status));
        Snmp::socket_cleanup();
        return Status_FAILED;
    }
    Snmp::socket_cleanup();
    qDebug() << "########################################################";
    qDebug() << "Handle RequestGet Successfully";
    qDebug() << "Oid:       " << request->data.get_printable_oid();
    qDebug() << "Value:     " << request->data.get_printable_value();
    qDebug() << "########################################################";
    return Status_SUCCESS;
}

Status SnmpManager::handleOperationGetNext(Request *request)
{
    Snmp::socket_startup();
    int status;
    Snmp snmp(status, 0, false);
    if (status != SNMP_CLASS_SUCCESS) {
        /*Setup SNMP FAILED*/
        Helper::log(0, "RequestGetNext Failed");
        Helper::log(1, snmp.error_msg(status));
        Helper::pop("Error", snmp.error_msg(status));
        Snmp::socket_cleanup();
        return Status_FAILED;
    }
    Pdu pdu;
    pdu += request->data;
    CTarget ctarget(request->address);
    ctarget.set_retry(request->retry);
    ctarget.set_timeout(request->timeout);
    ctarget.set_version(request->version);
    ctarget.set_readcommunity(request->community.c_str());
    SnmpTarget *target = &ctarget;
    status = snmp.get_next(pdu, *target);
    if (status == SNMP_CLASS_SUCCESS) {
        /*GetNextRequest SUCCESS*/
        Helper::log(1, "RequestGetNext SUCCESS");
        pdu.get_vb(request->data, 0);
        QString replayOid = request->data.get_printable_oid();
        QString replayValue = request->data.get_printable_value();
    }
    else {
        /*GetNextRequest FAILED*/
        Helper::log(0, "RequestGetNext Failed");
        Helper::log(1, snmp.error_msg(status));
        Helper::pop("Error", snmp.error_msg(status));
        Snmp::socket_cleanup();
        return Status_FAILED;
    }
    Snmp::socket_cleanup();
    qDebug() << "########################################################";
    qDebug() << "Handle RequestGetNext Successfully";
    qDebug() << "Oid:       " << request->data.get_printable_oid();
    qDebug() << "Value:     " << request->data.get_printable_value();
    qDebug() << "########################################################";
    return Status_SUCCESS;
}

Status SnmpManager::handleOperationSet(Request *request)
{
    Snmp::socket_startup();
    int status;
    Snmp snmp(status, 0, false);
    if (status != SNMP_CLASS_SUCCESS) {
        /*Setup SNMP FAILED*/
        Helper::log(0, "RequestSet Failed");
        Helper::log(1, snmp.error_msg(status));
        Helper::pop("Error", snmp.error_msg(status));
        Snmp::socket_cleanup();
        return Status_FAILED;
    }
    Pdu pdu;
    pdu += request->data;
    CTarget ctarget(request->address);
    ctarget.set_retry(request->retry);
    ctarget.set_timeout(request->timeout);
    ctarget.set_version(request->version);
    ctarget.set_writecommunity(request->community.c_str());
    SnmpTarget *target = &ctarget;
    status = snmp.set(pdu, *target);
    if (status == SNMP_CLASS_SUCCESS) {
        /*SetRequest SUCCESS*/
        Helper::log(1, "RequestSet Success");
        Helper::pop("Info", "RequestSet Success");
    }
    else {
        /*SetRequest FAILED*/
        Helper::log(0, "RequestSet Failed");
        Helper::log(1, snmp.error_msg(status));
        Helper::pop("Error", snmp.error_msg(status));
        Snmp::socket_cleanup();
        return Status_FAILED;
    }
    Snmp::socket_cleanup();
    qDebug() << "########################################################";
    qDebug() << "Handle RequestSet Successfully";
    qDebug() << "Oid:       " << request->data.get_printable_oid();
    qDebug() << "Value:     " << request->data.get_printable_value();
    qDebug() << "########################################################";
    return Status_SUCCESS;
}
