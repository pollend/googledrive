//
// Created by michaelpollind on 3/9/17.
//

#ifndef CLIENT_OATHCREDENTIALS_H
#define CLIENT_OATHCREDENTIALS_H


#include <QMap>
#include <QSslCertificate>
#include <QSslKey>
#include "creds/abstractcredentials.h"

class QNetworkReply;
class QAuthenticator;

namespace QKeychain {
    class Job;
    class WritePasswordJob;
    class ReadPasswordJob;
}


namespace OCC {
    class OWNCLOUDSYNC_EXPORT Oauth2Credentials : public AbstractCredentials
    {
        Q_OBJECT
    //    friend HttpCredentialsAccessManager;
    public:

        explicit Oauth2Credentials();
        Oauth2Credentials(const QString& oauthToken, const QSslCertificate& certificate = QSslCertificate(), const QSslKey& key = QSslKey());
    private Q_SLOTS:
  //      void slotAuthentication(QNetworkReply*, QAuthenticator*);
     //   void slotWriteClientCertPEMJobDone(QKeychain::Job*);
     //   void slotWriteClientKeyPEMJobDone(QKeychain::Job*);
    protected:
        QString _oauth;

    };
}


#endif //CLIENT_OATHCREDENTIALS_H
