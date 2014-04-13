/**
 * Copyright (C) 2013 Benjamin Bouguet, Damien Carduner, Paul Chaignon,
 * Eric Chauty, Xavier Fraboulet, Clement Gautrais, Ulysse Goarant.

 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
#pragma once

#include "ReportFactory.h"
#include <QString>
#include "Portfolio.h"
#include "StatisticsReport.h"
#include <qmath.h>
#include "qcustomplot.h"

class MODELVARSHARED_EXPORT StatisticsReportFactory: public ReportFactory {
public:
    StatisticsReportFactory(Portfolio *portfolio);

protected:

    virtual Report * createReport();
    virtual ReportDataJson* createJson();

    QPixmap generateChart1(QMap<QDateTime, double> values);
    QPixmap generateChart2(QList<Asset *> compo, QDateTime start,QDateTime end);
    double getAverage(QVector<double> values);
    double getVariance(QVector<double> values);
    double getMax(QVector<double> values);
    double getMin(QVector<double> values);
    double getKurtosis(QVector<double> values);

    Portfolio * portfolio;


};
