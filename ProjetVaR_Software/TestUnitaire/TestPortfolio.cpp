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
#include "TestPortfolio.h"

/**
 * @brief Initializes a few portfolio for the tests.
 */
TestPortfolio::TestPortfolio() {
	QString assetFolder = "../../CSV_examples/";

	// FIRST ASSET DEFINITION
	QDate startDate1(2014, 1, 1);
	QDate endDate1(2014, 1, 6);
	Asset* asset1 = new Asset("asset1", assetFolder+"asset1.txt", "YAHOO", startDate1, endDate1);

	// SECOND ASSET DEFINITION
	QDate startDate2(2014, 1, 2);
	QDate endDate2(2014, 1, 7);
	Asset* asset2 = new Asset("asset2", assetFolder+"asset2.txt", "YAHOO", startDate2, endDate2);

	// THIRD ASSET DEFINITION
	QDate startDate3(QDate(2014, 1, 3));
	QDate endDate3(QDate(2014, 1, 8));
	Asset* asset3 = new Asset("asset3", assetFolder+"asset3.txt", "YAHOO", startDate3, endDate3);

	// FOURTH ASSET DEFINITION
	QDate startDate4(QDate(2014, 1, 1));
	QDate endDate4(QDate(2014, 1, 4));
	Asset* asset4 = new Asset("asset4", assetFolder+"asset4.txt", "YAHOO", startDate4, endDate4);

	// FIFTH ASSET DEFINITION
	QDate startDate5(QDate(2014, 1, 1));
	QDate endDate5(QDate(2014, 1, 4));
	Asset* asset5 = new Asset("asset5", assetFolder+"asset5.txt", "YAHOO", startDate5, endDate5);

	// TEST PORTFOLIO DEFINITION
	QMap<Asset*, int> assets;
	assets.insert(asset1, 1);
	assets.insert(asset2, 2);
	assets.insert(asset3, 3);

	QVector<Report*> reports;
	this->father = Portfolio("Father", assets, reports);
	this->son = Portfolio(&this->father, "Son", assets, reports);

	// SECOND PORTFOLIO DEFINITION
	QMap<Asset*, int> assets2;
	assets2.insert(asset4, 1);
	assets2.insert(asset5, 2);
	QVector<Report*> reports2;
	this->uncle = Portfolio("uncle", assets2, reports2);
}

/**
 * @brief Checks that the id can only be set once.
 */
void TestPortfolio::testId() {
	QCOMPARE(this->father.getId(), -1);
	QCOMPARE(this->son.getParentId(), -1);
	this->father.setId(42);
	QCOMPARE(this->father.getId(), 42);
	QCOMPARE(this->son.getParentId(), 42);
	try {
		this->father.setId(43);
	} catch(const IdAlreadyAttributedException e) {
		return;
	}
	QFAIL("No exception thrown when attributed an id to a porfolio twice.");
}

/**
 * @brief Checks that the name can actually be changed.
 */
void TestPortfolio::testName() {
	QVERIFY(this->father.getName() == "Father");
	this->father.changeName("John");
	QVERIFY(this->father.getName() == "John");
}

/**
 * @brief Tests the method getValues that retrieve and compute values of a portfolio.
 */
void TestPortfolio::testRetrieveValues() {
	QDate startDate(2014, 1, 3);
	QDate endDate(2014, 1, 6);
	QVector<double> result;
	try {
		result = this->son.retrieveValues(startDate, endDate);
	} catch(PortfolioCalculationException& e) {
		qDebug() << e.what();
	}
	QCOMPARE(result.size(), 4);
	QCOMPARE(result.at(0), 612.0);
	QCOMPARE(result.at(1), 618.0);
	QCOMPARE(result.at(2), 624.0);
	QCOMPARE(result.at(3), 630.0);
}

/**
 * @brief Successful test of retrieveValues without parameters.
 */
void TestPortfolio::testRetrieveValuesNoParameters() {
	QVector<double> result;
	try {
		result = this->son.retrieveValues();
	} catch(PortfolioCalculationException& e) {
		qDebug() << e.what();
	}
	QCOMPARE(result.size(), 6);
	QCOMPARE(result.at(0), 612.0);
	QCOMPARE(result.at(1), 618.0);
	QCOMPARE(result.at(2), 624.0);
	QCOMPARE(result.at(3), 630.0);
	QCOMPARE(result.at(4), 635.0);
	QCOMPARE(result.at(5), 638.0);
}

/**
 * @brief Tests the method retrieveValuesByDate that retrieve and compute date-values of a portfolio
 */
void TestPortfolio::testRetrieveValuesByDate() {
	// RetrieveValuesByDate() behaviour
	// FIRST PORTFOLIO ILLUSTRATION
	//				a1		a2		a3		Portfolio
	// 2014-01-01	100		UNDEF	UNDEF	UNDEF
	// 2014-01-02	101		202		UNDEF	UNDEF
	// 2014-01-03	102		204		306		612
	// 2014-01-04	103		206		309		618
	// 2014-01-05	104		208		312		624
	// 2014-01-06	105		210		315		630
	// 2014-01-07	UNDEF	212		318		635 << using previous value for a1
	// 2014-01-08	UNDEF	UNDEF	321		638 << using previous values for a1 and a2
	QMap<QDate, double> result;

	// EXPECTED USE CASE
	try {
		result = this->son.retrieveValuesByDate(QDate(2014, 1, 3), QDate(2014, 1, 8));
	} catch(PortfolioCalculationException& e) {
		qDebug() << e.what();
	}

	QCOMPARE(result.size(), 6);
	QCOMPARE(result.value(QDate(2014, 1, 3)), 612.0);
	QCOMPARE(result.value(QDate(2014, 1, 4)), 618.0);
	QCOMPARE(result.value(QDate(2014, 1, 5)), 624.0);
	QCOMPARE(result.value(QDate(2014, 1, 6)), 630.0);
	QCOMPARE(result.value(QDate(2014, 1, 7)), 635.0);
	QCOMPARE(result.value(QDate(2014, 1, 8)), 638.0);
}

/**
 * @brief retrieveValuesByDate should throws an exception when asset's values are missing.
 */
void TestPortfolio::testRetrieveValuesByDateMissingValues() {
	QMap<QDate, double> result;
	try {
		result = this->son.retrieveValuesByDate(QDate(2014, 1, 2), QDate(2014, 1, 8));
		QFAIL("retrieveValuesByDate succedded in computing portfolio values despite missing asset values");
	} catch(PortfolioCalculationException& e) {
		qDebug() << e.what();
	}
}

void TestPortfolio::testRetrieveReturns() {
	QVector<double> result;
	result = this->son.retrieveReturns(QDate(2014, 1, 8), 4);
	qDebug() << result;
	qDebug() << this->son.retrieveEndDate();
	result = this->son.retrieveReturns(this->son.retrieveEndDate(), 4);
	// In that case retrieveEndDate() is 2014-01-06 because it is the last shared date
	// among the assets.
	qDebug() << result;
}
