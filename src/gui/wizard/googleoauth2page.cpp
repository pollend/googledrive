#include "googleoauth2page.h"
#include "ui_googleoauth2page.h"

namespace OCC{



GoogleOauth2Page::GoogleOauth2Page(QWidget *parent) :
    QWizardPage(),
    _ui()
{

    _ui.setupUi(this);

    _o2gft = new O2Gft(this);
    _o2gft->setClientId(CLIENT_ID);
    _o2gft->setClientSecret(CLIENT_SECRETE);

    connect(_o2gft, SIGNAL(linkedChanged()), this, SLOT(onLinkedChanged()));
    connect(_o2gft, SIGNAL(linkingFailed()), this, SLOT(onLinkingFailed()));
    connect(_o2gft, SIGNAL(linkingSucceeded()), this, SLOT(onLinkingSucceeded()));
    connect(_o2gft, SIGNAL(openBrowser(QUrl)), this, SLOT(onOpenBrowser(QUrl)));
    connect(_o2gft, SIGNAL(closeBrowser()), this, SLOT(onCloseBrowser()));

    _o2gft->setGrantFlow(O2::GrantFlowAuthorizationCode);
    _o2gft->link();

}

void GoogleOauth2Page::onOpenBrowser(const QUrl *url) {
    qDebug() << "opened Browser";
    this->_ui.loginview->setUrl((const QUrl &) url);

}

void GoogleOauth2Page::onLinkedChanged()
{

}
void GoogleOauth2Page::onLinkingFailed()
{

}
void GoogleOauth2Page::onLinkingSucceeded()
{

}
void GoogleOauth2Page::onCloseBrowser()
{

}


GoogleOauth2Page::~GoogleOauth2Page()
{
}

};
