#include "eurotherm.h"

#include <errno.h>

#ifndef NULL
#define NULL ((0))
#endif

Eurotherm::Eurotherm() :
    dev(NULL),
    errNo(0)
{
}

Eurotherm::~Eurotherm()
{
    if (isOpen())
        close();
}

void Eurotherm::close()
{
    modbus_free(dev);
    dev = NULL;
}

bool Eurotherm::currentT(int *T)
{
    *T = 0;
    if (modbus_read_registers(dev, REG_PV_IN, 1, (uint16_t*)T) != 1) {
        errNo = errno;
        return false;
    }
    return true;
}

int Eurotherm::error() const
{
    return errNo;
}

const char* Eurotherm::errorString() const
{
    return modbus_strerror(errNo);
}

bool Eurotherm::isOpen() const
{
    return (dev != NULL);
}

bool Eurotherm::mode(bool *)
{
    return false;
}

bool Eurotherm::open(const char* port, int slave)
{
    dev = modbus_new_rtu(port, 9600, 'N', 8, 1);
    if (dev == NULL) {
        errNo = errno;
        return false;
    }

    if (modbus_connect(dev) == -1)
    {
        errNo = errno;
        return false;
    }

    if (modbus_set_slave(dev, slave) == -1) {
        errNo = errno;
        return false;
    }

    return true;
}

bool Eurotherm::setEnabled(bool enabled)
{
    /* In manual mode output value should be set to 0 (safe value) TODO: check. */
    if (modbus_write_register(dev, REG_IM, enabled ? REG_IM_AUTO : REG_IM_MANUAL) != 1) {
        errNo = errno;
        return false;
    }

    return true;
}

bool Eurotherm::setTarget(int T)
{
    if (modbus_write_register(dev, REG_SP1, T) != 1) {
        errNo = errno;
        return false;
    }

    return true;
}

bool Eurotherm::targetT(int *T)
{
    *T = 0;

    if (modbus_read_registers(dev, REG_TG_SP, 1, (uint16_t*)T) != 1) {
        errNo = errno;
        return false;
    }

    return true;
}

bool Eurotherm::targetTRange(int *Tmin, int *Tmax)
{
    *Tmin = *Tmax = 0;

    if (modbus_read_registers(dev, REG_SP_LO, 1, (uint16_t*)Tmin) != 1) {
        errNo = errno;
        return false;
    }

    if (modbus_read_registers(dev, REG_SP_HI, 1, (uint16_t*)Tmax) != 1) {
        errNo = errno;
        return false;
    }

    return true;
}
