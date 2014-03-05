#include "TestImportNewData.h"

/**
* @brief Initializes an asset for the tests.
*/
TestImportNewData::TestImportNewData() {
    TestImportNewData::stockName = "Gogole";
    TestImportNewData::firstDate = "2014-01-02";
    TestImportNewData::lastDate = "2014-01-01";
    TestImportNewData::origin = "Yahoo";

    //mettre une valeur par défaut au firstdate et lastdate
    //les donner en param de l'algo d'importation
    //lire le fichier généré
    //comparer les dates obtenues
    QString data;
    QFile importedCSV("C:/Users/Eric/Downloads/table.csv");
    QStringList rowOfData;
    QStringList rowData;
    data.clear();
    rowOfData.clear();
    rowData.clear();

    if (importedCSV.open(QFile::ReadOnly))
        {
            data = importedCSV.readAll();
            rowOfData = data.split("\r\n");
            importedCSV.close();
        }

    TestImportNewData::newFile =  TestImportNewData::stockName+"_test.csv";
    QFile fileCreated(TestImportNewData::newFile);
    if (!fileCreated.open(QIODevice::WriteOnly | QIODevice::Text))
       return;
    QTextStream flux(&fileCreated);
    flux.setCodec("UTF-8");
    const QDateTime &fiDate = QDateTime::fromString(TestImportNewData::firstDate,"yyyy-MM-dd");
    const QDateTime &laDate = QDateTime::fromString(TestImportNewData::lastDate,"yyyy-MM-dd");
    for (int x =1; x < rowOfData.size()-1; x++)
    {
        rowData = rowOfData.at(x).split(",");
        const QDateTime &currentDate = QDateTime::fromString(rowData[0],"yyyy-MM-dd");
        if ((fiDate >= currentDate) && (currentDate >= laDate)){
            flux << rowData[0] << "," << rowData[6] << "\n";
            qDebug() << rowData[0] << "," << rowData[6];
        }
        //qDebug() << rowData[0] << "," << rowData[6];
    }
    fileCreated.close();
}

/**
* @brief Checks that the firstDate and the lastDate of the generated file correspond to the params
*/
void TestImportNewData::testDates() {
    QString data;
    QFile importedCSV(TestImportNewData::newFile);
    QStringList rowOfData;
    QStringList rowData;
    data.clear();
    rowOfData.clear();
    rowData.clear();

    if (importedCSV.open(QFile::ReadOnly))
    {
        data = importedCSV.readAll();
        rowOfData = data.split("\n");
        importedCSV.close();
    }
    //first date
    rowData = rowOfData.at(0).split(",");
    qDebug() << rowData[0];
    QVERIFY((QDateTime::fromString(TestImportNewData::firstDate,"yyyy-MM-dd")) >= (QDateTime::fromString(rowData[0],"yyyy-MM-dd")));

    //last date
    rowData = rowOfData.at(rowOfData.size()).split(",");
    qDebug() << rowData[0];
    QVERIFY((QDateTime::fromString(TestImportNewData::lastDate,"yyyy-MM-dd")) <= (QDateTime::fromString(rowData[0],"yyyy-MM-dd")));
}


/**
* @brief Checks that the name can actually be changed.
*/
void TestImportNewData::testName() {
    /*
    QVERIFY(this->google.getName() == "Google");
    this->google.changeName("Googl");
    QVERIFY(this->google.getName() == "Googl");
    */
}
