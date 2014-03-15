#ifndef SHOWASSETSWIZARDPAGE_H
#define SHOWASSETSWIZARDPAGE_H

#include <QWizardPage>
#include <QDebug>
#include "SessionBuilder.h"

namespace Ui {
class ShowAssetsWizardPage;
}

class ShowAssetsWizardPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit ShowAssetsWizardPage(QWidget *parent = 0);
    ~ShowAssetsWizardPage();

    void setupAssets();

    bool isComplete() const ;

public slots:
    void allLeftButtonClicked();
    void allRightButtonClicked();
    void rightButtonClicked();
    void leftButtonClicked();

private:
    Ui::ShowAssetsWizardPage *ui;
};

#endif // SHOWASSETSWIZARDPAGE_H
