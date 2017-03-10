#ifndef GOOGLEOAUTH2PAGE_H
#define GOOGLEOAUTH2PAGE_H

#include <QWizardPage>

#define  TOKEN_URL "https://accounts.google.com/o/oauth2/token"
#include "ui_googleoauth2page.h"

namespace OCC {
    class GoogleOauth2Page : public QWizardPage {
        Q_OBJECT
        public:
            explicit GoogleOauth2Page(QWidget *parent = 0);

            ~GoogleOauth2Page();
        signals:
            void oauthToken(const QString&);

        private:
            Ui_GoogleOauth2Page _ui;

    };
}

#endif // GOOGLEOAUTH2PAGE_H
