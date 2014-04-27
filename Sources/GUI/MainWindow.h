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

#include <QMainWindow>
#include <QDialog>
#include <QComboBox>
#include <QFile>
#include <QFileDialog>
#include <QTableWidgetItem>
#include "ImportDialog.h"
#include "NewPortfolioWizard.h"
#include "PortfolioItemModel.h"
#include "PortfolioViewModel.h"
#include "ui_MainWindow.h"
#include <QDate>
#include "ImportNewData.h"
#include "GetStartEndDates.h"

namespace Ui {
	class MainWindow;
}

class MainWindow: public QMainWindow {
	Q_OBJECT
	
public:
	explicit MainWindow(QWidget* parent = 0);
	~MainWindow();

private slots:
	void newPortfolio();
	void showPortfolio(Portfolio* portfolio);
	void setImportCSV();
	void addPortfolio(Portfolio *);

private:
	Ui::MainWindow *ui;
	QString stockName;
	QDate startDate;
	QDate endDate;
	QString fileName;
	QString origin;
	QString path;
	PortfolioItemModel * portfolioListModel;
	QHash<Portfolio*, PortfolioViewModel*> portfoliosModels;
};
