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
#include "TestAsset.h"

/**
 * @brief Initializes an asset for the tests.
 */
TestAsset::TestAsset() {
	QDate startDate(2014, 1, 2);
	QDate endDate(2014, 1, 5);
	this->google = Asset("Google", "../../CSV_examples/asset1.txt", "YAHOO", startDate, endDate);
	this->apple = Asset("Apple", "../../CSV_examples/asset4.txt", "YAHOO", QDate(2014, 1, 1), QDate(2014, 1, 4));
	this->missing = Asset("test", "../../CSV_examples/nonexistingfile.txt", "YAHOO", startDate, endDate);
}

/**
 * @brief Checks that the id can only be set once.
 */
void TestAsset::testId() {
	QCOMPARE(this->google.getId(), -1);
	this->google.setId(42);
	QCOMPARE(this->google.getId(), 42);
	try {
		this->google.setId(43);
	} catch(const IdAlreadyAttributedException e) {
		return;
	}
	QFAIL("No exception thrown when attributed an id to an asset twice.");
}

/**
 * @brief Checks that the name can actually be changed.
 */
void TestAsset::testName() {
	QVERIFY(this->google.getName() == "Google");
	this->google.changeName("Googl");
	QVERIFY(this->google.getName() == "Googl");
}

/**
 * @brief Successful test on retrieveValues.
 */
void TestAsset::testRetrieveValues() {
	QVector<double> result;
	try {
		result = this->google.retrieveValues(this->google.getStartDate(), this->google.getEndDate());
	} catch(CannotOpenFileException& e) {
		qDebug() << e.what();
	}
	QCOMPARE(result.size(), 4);
	QCOMPARE(result.at(0), 101.0);
	QCOMPARE(result.at(1), 102.0);
	QCOMPARE(result.at(2), 103.0);
	QCOMPARE(result.at(3), 104.0);
}

/**
 * @brief Checks that the retrieveValues throws an exception
 * if the dates are in inversed.
 */
void TestAsset::testRetrieveValuesIncorrectDates() {
	QVector<double> result;
	try {
		result = this->google.retrieveValues(this->google.getEndDate(), this->google.getStartDate());
		QFAIL("retrieveValues was able to execute with incorrect date paramaters");
	} catch(std::exception& e) {
		qDebug() << e.what();
	}
}

/**
 * @brief Checks that retrieveValues throws an exception
 * if the file with the values doesn't exist.
 */
void TestAsset::testRetrieveValuesNonexistingFile() {
	QVector<double> result;
	try {
		result = this->missing.retrieveValues(this->missing.getStartDate(), this->missing.getEndDate());
		QFAIL("retrieveValuesByDate() was able to open a file that should not exist");
	} catch(CannotOpenFileException& e) {
		qDebug() << e.what();
	}
}

/**
 * @brief Test a special date parameters case.
 * The first date can be older than the actual oldest date in the file.
 * The returned vector will simply start with the actual oldest date.
 * The same goes with the last date in its opposite way.
 */
void TestAsset::testRetrieveValuesSpecialDates() {
	QVector<double> result;
	QDate startDate(2013, 12, 25);
	QDate endDate(2014, 1, 10);
	try {
		result = this->google.retrieveValues(startDate, endDate);
	} catch(CannotOpenFileException& e) {
		qDebug() << e.what();
	}
	QCOMPARE(result.size(), 6);
	QCOMPARE(result.at(0), 100.0);
	QCOMPARE(result.at(1), 101.0);
	QCOMPARE(result.at(2), 102.0);
	QCOMPARE(result.at(3), 103.0);
	QCOMPARE(result.at(4), 104.0);
	QCOMPARE(result.at(5), 105.0);
}

/**
 * @brief Tries to retrieveValues for a day unavailable
 * between the dates of definition.
 */
void TestAsset::testRetrieveValuesUnavailableDates() {
	QVector<double> result;
	try {
		result = this->apple.retrieveValues(QDate(2014, 1, 3), QDate(2014, 1, 3));
	} catch(CannotOpenFileException& e) {
		qDebug() << e.what();
	}
	QVERIFY(result.isEmpty());
}

/**
 * @brief Successful test on the retrieveValues without parameters.
 */
void TestAsset::testRetrieveValuesNoParameters() {
	QVector<double> result;
	try {
		result = this->google.retrieveValues();
	} catch(CannotOpenFileException& e) {
		qDebug() << e.what();
	}
	QCOMPARE(result.size(), 4);
	QCOMPARE(result.at(0), 101.0);
	QCOMPARE(result.at(1), 102.0);
	QCOMPARE(result.at(2), 103.0);
	QCOMPARE(result.at(3), 104.0);
}

/**
 * @brief Tests the method retrieveValuesByDate that retrieves all date-values of an asset.
 * Successful test
 */
void TestAsset::testRetrieveValuesByDate() {
	QMap<QDate, double> result;
	try {
		result = this->google.retrieveValuesByDate(this->google.getStartDate(), this->google.getEndDate());
	} catch(CannotOpenFileException& e) {
		qDebug() << e.what();
	}
	QCOMPARE(result.size(), 4);
	QCOMPARE(result.value(QDate(2014, 1, 2)), 101.0);
	QCOMPARE(result.value(QDate(2014, 1, 3)), 102.0);
	QCOMPARE(result.value(QDate(2014, 1, 4)), 103.0);
	QCOMPARE(result.value(QDate(2014, 1, 5)), 104.0);
}

/**
 * @brief Checks that the retrieveValuesByDate throws an exception
 * if the dates are in inversed.
 */
void TestAsset::testRetrieveValuesByDateIncorrectDates() {
	QMap<QDate, double> result;
	try {
		result = this->google.retrieveValuesByDate(this->google.getEndDate(), this->google.getStartDate());
		QFAIL("retrieveValuesByDate was able to execute with incorrect date paramaters");
	} catch(std::exception& e) {
		qDebug() << e.what();
	}
}

/**
 * @brief Checks that retrieveValuesByDate throws an exception
 * if the file with the values doesn't exist.
 */
void TestAsset::testRetrieveValuesByDateNonexistingFile() {
	QMap<QDate, double> result;
	try {
		result = this->missing.retrieveValuesByDate(this->missing.getStartDate(), this->missing.getEndDate());
		QFAIL("retrieveValuesByDate() was able to open a file that should not exist");
	} catch(CannotOpenFileException& e) {
		qDebug() << e.what();
	}
}

/**
 * Test a special date parameters case.
 * The first date can be older than the actual oldest date in the file.
 * The returned vector will simply start with the actual oldest date.
 * The same goes with the last date in its opposite way.
 */
void TestAsset::testRetrieveValuesByDateSpecialDates() {
	QMap<QDate, double> result;
	QDate startDate(2013, 12, 25);
	QDate endDate(2014, 1, 10);
	try {
		result = this->google.retrieveValuesByDate(startDate, endDate);
	} catch(CannotOpenFileException& e) {
		qDebug() << e.what();
	}
	QCOMPARE(result.size(), 6);
	QCOMPARE(result.value(QDate(2014, 1, 1)), 100.0);
	QCOMPARE(result.value(QDate(2014, 1, 2)), 101.0);
	QCOMPARE(result.value(QDate(2014, 1, 3)), 102.0);
	QCOMPARE(result.value(QDate(2014, 1, 4)), 103.0);
	QCOMPARE(result.value(QDate(2014, 1, 5)), 104.0);
	QCOMPARE(result.value(QDate(2014, 1, 6)), 105.0);
}
