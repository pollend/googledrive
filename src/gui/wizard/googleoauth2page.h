#ifndef GOOGLEOAUTH2PAGE_H
#define GOOGLEOAUTH2PAGE_H

#include <QWizardPage>

#define  TOKEN_URL "https://accounts.google.com/o/oauth2/token"
#define CLIENT_ID  "751940853221-eaa2qibr862tbd6nil5pef91dbusgrl8.apps.googleusercontent.comm"
#define CLIENT_SECRETE "8zMh9iA-gP-7qXdQ-OnXrSA2"

#include "ui_googleoauth2page.h"
#include "o2gft.h"

namespace OCC {
    class GoogleOauth2Page : public QWizardPage {
        Q_OBJECT
        public:
            explicit GoogleOauth2Page(QWidget *parent = 0);

            ~GoogleOauth2Page();
        private slots:
            void onLinkedChanged();
            void onLinkingFailed();
            void onLinkingSucceeded();
            void onOpenBrowser(const QUrl *url);
            void onCloseBrowser();
        signals:
            void oauthToken(const QString&);

        private:
            Ui_GoogleOauth2Page _ui;
            O2Gft* _o2gft;


    };
}

#endif // GOOGLEOAUTH2PAGE_H
