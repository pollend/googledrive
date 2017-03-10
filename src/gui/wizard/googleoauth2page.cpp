#include "googleoauth2page.h"
#include "ui_googleoauth2page.h"

namespace OCC{



GoogleOauth2Page::GoogleOauth2Page(QWidget *parent) :
    QWizardPage(),
    _ui()
{
    _ui.setupUi(this);

}

GoogleOauth2Page::~GoogleOauth2Page()
{
}

};
