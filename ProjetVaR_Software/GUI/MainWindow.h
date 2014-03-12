#pragma once

#include <QMainWindow>
#include <QDateTime>
#include <QDialog>
#include <QComboBox>
#include "import.h"
#include <string>

namespace Ui {
	class MainWindow;
}

class MainWindow: public QMainWindow {
	Q_OBJECT
    
public:
	explicit MainWindow(QWidget* parent = 0);
	~MainWindow();

private slots:
	void setImportCSV();

public slots:
	void onDataEntered(const QString &text, const QDateTime &fDate ,const QDateTime &lDate, const QString &source);

private:
	Ui::MainWindow *ui;
	Import import_win;
	QString stockName;
    QDateTime startDate;
    QDateTime endDate;
	QString fileName;
    QString origin;
    //SessionSaver session;
};
