/*
 * This file is part of ofono-qt
 *
 * Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
 *
 * Contact: Alexander Kanavin <alexander.kanavin@nokia.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * version 2.1 as published by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
 *
 */
 
#include <QtDBus/QtDBus>
#include <QtCore/QObject>
#include <QtCore/QDebug>

#include "ofonomodeminterface.h"
#include "ofonomodem.h"

OfonoModemInterface::OfonoModemInterface(OfonoModem::SelectionSetting modemSetting, const QString& modemPath, const QString& ifname, GetPropertySetting propertySetting, QObject *parent)
    : OfonoInterface("/", ifname, propertySetting, parent)
{

    m_m = new OfonoModem(modemSetting, modemPath, this);
    connect(m_m, SIGNAL(validityChanged(bool)), this, SLOT(modemValidityChanged(bool)));
    connect(m_m, SIGNAL(interfacesChanged(QStringList)), this, SLOT(interfacesChanged(QStringList)));
    connect(m_m, SIGNAL(pathChanged(QString)), this, SLOT(setPath(QString)));

    setPath(m_m->path());
    m_isValid = checkValidity();
}

OfonoModemInterface::~OfonoModemInterface()
{
}

bool OfonoModemInterface::isValid() const
{
    return m_isValid;
}

OfonoModem* OfonoModemInterface::modem() const
{
    return m_m;
}

bool OfonoModemInterface::checkValidity()
{
    return (m_m->isValid() && m_m->interfaces().contains(ifname()));
}

void OfonoModemInterface::updateValidity()
{
    if (isValid() != checkValidity()) {
        m_isValid = checkValidity();
        emit validityChanged(isValid());
    }
}

void OfonoModemInterface::modemValidityChanged(bool /*validity*/)
{
    updateValidity();
}

void OfonoModemInterface::interfacesChanged(const QStringList& /*interfaces*/)
{
    updateValidity();
}

