#ifndef GOOGLEOAUTH2PAGE_H
#define GOOGLEOAUTH2PAGE_H

#include <QWizardPage>
#include <QtWebKit/QtWebKit>

#include "ui_googleoauth2page.h"
#include "creds/o2replyserver.h"
#include "creds/abstractcredentials.h"

namespace OCC {
    class GoogleOauth2Page : public QWizardPage {
        Q_OBJECT
        public:
            explicit GoogleOauth2Page();

            ~GoogleOauth2Page();

    public:
       // void setComplete(bool complete);
        virtual int nextId() const Q_DECL_OVERRIDE;
        virtual bool isComplete() const Q_DECL_OVERRIDE;
    private slots:
        void onVerificationReceived(QMap<QString, QString>);
        void onTimeout();
        void onTokenReplyError(QNetworkReply::NetworkError);
        void onTokenReplyFinished();
    signals:
        void registerOauth2(const QString&,const QString&,int);
        void returnFail();

    private:
        /// Build HTTP request body.
        QByteArray buildRequestBody(const QMap<QString, QString> &parameters);


        Ui_GoogleOauth2Page _ui;
        O2ReplyServer * _replyServer;
        QNetworkAccessManager *_manager;
        QTimer* _timer;
        QNetworkReply* _networkReply;
        bool  _hasTokenBeenRegistered;

    };
}

#endif // GOOGLEOAUTH2PAGE_H
