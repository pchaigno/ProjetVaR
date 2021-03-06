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
#include "TestRInterface.h"

/**
 * @brief Initializes a portfolio for the tests.
 */
TestRInterface::TestRInterface() {
	QString assetFolder = "../Examples/";

	// FIRST ASSET DEFINITION
	QDate startDate(2014, 1, 2);
	QDate endDate(2014, 3, 11);
	Asset* asset1 = new Asset("asset1", assetFolder+"dax.csv", "YAHOO", startDate, endDate);

	// TEST PORTFOLIO DEFINITION
	QMap<Asset*, int> assets;
	assets.insert(asset1, 1);
	QList<Report*> reports;
	this->father = new Portfolio("Father", assets, reports);
}

/**
 * @brief Tests the correct computation of a Garch model through R.
 */
void TestRInterface::testComputeGarchModel() {
	QPair<QDate, QDate> period;
	period.first = QDate(2014, 2, 25);
	period.second = this->father->retrieveEndDate();
	GarchModel model = RInterface::computeGarchModel(*father, period);

	QCOMPARE(model.getOmega(), 0.0002504062);
	QCOMPARE(model.getAlpha(), 0.0500141719);
	QCOMPARE(model.getBeta(), 0.0499956054);
	QCOMPARE(model.getResiduals().size(), 9);
	QCOMPARE(model.getResiduals().at(0), -0.468397283);
	QCOMPARE(model.getResiduals().at(1), 0.659176463);
	QCOMPARE(model.getResiduals().at(2), -2.130830688);
	QCOMPARE(model.getResiduals().at(3), 1.348051460);
	QCOMPARE(model.getResiduals().at(4), -0.286279545);
	QCOMPARE(model.getResiduals().at(5), 0.005457179);
	QCOMPARE(model.getResiduals().at(6), -1.252346813);
	QCOMPARE(model.getResiduals().at(7), -0.543204489);
	QCOMPARE(model.getResiduals().at(8), 0.277749688);
}

/**
 * @brief Tests that a Garch model cannot be computed if the period parameter is too
 * short and thus incompatible with R Garch model computation.
 */
void TestRInterface::testComputeGarchModelIncorrectParameter() {
	QPair<QDate, QDate> period;
	period.first = QDate(2014, 2, 25);
	period.second = QDate(2014, 2, 27);
	try {
		GarchModel model = RInterface::computeGarchModel(*father, period);
		QFAIL("computeGarchModel() should not have succeeded in computing a Garch model");
	} catch(std::invalid_argument& e) {

	}
}

/**
 * @brief Tests the method checkCorrelation that computes the correlation of portfolio returns
 * in normal cases.
 */
void TestRInterface::testCheckCorrelation() {
	int timeLag, period;
	QDate date = this->father->retrieveEndDate();
	QPair<double, double> result;

	// Normal cases
	// Comparing results to a separate R execution
	timeLag = 1;
	period = 10;
	try {
		result = RInterface::checkCorrelation(*father, timeLag, date, period);
	} catch(std::invalid_argument& e) {
		QFAIL("checkCorrelation failed at computing a valid parameters test");
	}
	// The toDouble() method in checkCorrelation seems to slightly
	// modify the precision, that is why the following test is performed
	QCOMPARE(result.first, 2.633048);
	QCOMPARE(result.second, 0.1046607);

	timeLag = 3;
	period = 20;
	try {
		result = RInterface::checkCorrelation(*father, timeLag, date, period);
	} catch(std::invalid_argument& e) {
		QFAIL("checkCorrelation failed at computing a valid parameters test");
	}
	// The toDouble() method in checkCorrelation seems to slightly
	// modify the precision, that is why the following test is performed
	QCOMPARE(result.first, 6.277748);
	QCOMPARE(result.second, 0.09885197);
}

/**
 * @brief Tests the method checkCorrelation that computes the correlation of portfolio returns
 * with an incorrect time lag parameter
 */
void TestRInterface::testCheckCorrelationIncorrectTimeLag() {
	QPair<double, double> result;
	QDate date = this->father->retrieveEndDate();
	int timeLag = -1;
	int period = 10;
	try {
		result = RInterface::checkCorrelation(*father, timeLag, date, period);
		QFAIL("RInterface::checkCorrelation should have not executed");
	} catch(std::invalid_argument& e) {}
}

/**
 * @brief Tests the method checkCorrelation that computes the correlation of portfolio returns
 * with an incorrect period parameter
 */
void TestRInterface::testCheckCorrelationIncorrectPeriod() {
	QPair<double, double> result;
	QDate date = this->father->retrieveEndDate();
	int timeLag = 1;
	int period = -1;
	try {
		result = RInterface::checkCorrelation(*father, timeLag, date, period);
		QFAIL("RInterface::checkCorrelation should have not successfully executed");
	} catch(std::invalid_argument& e) {}
}

/**
 * @brief Tests the method checkCorrelation that computes the correlation of portfolio returns
 * with incompatibles time lag and period parameters
 */
void TestRInterface::testCheckCorrelationIncompatibleParameters() {
	QPair<double, double> result;
	QDate date = this->father->retrieveEndDate();
	int timeLag = 9;
	int period = 10;
	try {
		result = RInterface::checkCorrelation(*father, timeLag, date, period);
		QFAIL("RInterface::checkCorrelation should have not executed");
	} catch(std::invalid_argument& e) {}
}

/**
 * @brief Tests the method checkSquareCorrelation that computes the square correlation of portfolio returns
 * in normal cases
 */
void TestRInterface::testCheckSquareCorrelation() {
	int timeLag, period;
	QDate date = this->father->retrieveEndDate();
	QPair<double, double> result;

	// Normal cases
	timeLag = 1;
	period = 10;
	try {
		result = RInterface::checkSquareCorrelation(*father, timeLag, date, period);
	} catch(std::invalid_argument& e) {
		QFAIL("checkSquareCorrelation failed at computing a valid parameters test");
	}
	QCOMPARE(result.first, 0.3349454);
	QCOMPARE(result.second, 0.5627615);

	timeLag = 3;
	period = 20;
	try {
		result = RInterface::checkSquareCorrelation(*father, timeLag, date, period);
	} catch(std::invalid_argument& e) {
		QFAIL("checkSquareCorrelation failed at computing a valid parameters test");
	}
	QCOMPARE(result.first, 2.762662);
	QCOMPARE(result.second, 0.4296833);
}

/**
 * @brief Tests the method checkSquareCorrelation that computes the square correlation of portfolio returns
 * with incompatibles time lag and period parameters
 */
void TestRInterface::testCheckSquareCorrelationIncompatibleParameters() {
	QPair<double, double> result;
	QDate date = this->father->retrieveEndDate();
	int timeLag = 10;
	int period = 10;
	try {
		result = RInterface::checkSquareCorrelation(*father, timeLag, date, period);
		QFAIL("RInterface::checkSquareCorrelation should have not successfully executed");
	} catch(std::invalid_argument& e) {}
}
