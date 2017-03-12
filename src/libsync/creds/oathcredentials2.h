//
// Created by michaelpollind on 3/9/17.
//

#ifndef CLIENT_OATHCREDENTIALS_H
#define CLIENT_OATHCREDENTIALS_H


#define OAUTH2_RESPONSE_TYPE "response_type"
#define OAUTH2_CLIENT_ID "client_id"
#define OAUTH2_CLIENT_SECRET "client_secret"
#define OAUTH2_USERNAME "username"
#define OAUTH2_PASSWORD "password"
#define OAUTH2_REDIRECT_URI "redirect_uri"
#define OAUTH2_SCOPE "scope"
#define OAUTH2_GRANT_TYPE_CODE "code"
#define OAUTH2_GRANT_TYPE_TOKEN "token"
#define OAUTH2_GRANT_TYPE_PASSWORD "password"
#define OAUTH2_GRANT_TYPE "grant_type"
#define OAUTH2_API_KEY "api_key"

// OAuth 2 Response Parameters
#define OAUTH2_ACCESS_TOKEN "access_token"
#define OAUTH2_REFRESH_TOKEN "refresh_token"
#define OAUTH2_EXPIRES_IN "expires_in"

#define OAUTH2_AUTHORIZATION_CODE "authorization_code"
#define MIME_TYPE_XFORM "application/x-www-form-urlencoded"


#define CLIENT_ID  "751940853221-v8jpuhnu40vip2hsfpcctltlp85rco3j.apps.googleusercontent.com"
#define CLIENT_SECRETE "MxuLENFkm-QChCr1eegYHC_c"
#define OAUTH_PORT 1965

#define REDIRECT_URI "http://127.0.0.1:%1/"


#define SCOPE_URL "https://www.googleapis.com/auth/drive https://www.googleapis.com/auth/userinfo.profile"

#define ENDPOINT_URL "https://accounts.google.com/o/oauth2/auth"
#define TOKENURL_URL "https://accounts.google.com/o/oauth2/token"
#define REFRESH_URL "https://accounts.google.com/o/oauth2/token"



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
        public:

            explicit Oauth2Credentials();
            Oauth2Credentials(const QString& userName,const  QString& userId,const QString& oauthToken,const QString& refreshToken,int refreshTime, const QSslCertificate& certificate = QSslCertificate(), const QSslKey& key = QSslKey());
        private Q_SLOTS:


        protected:
            QString _oauth;
            QString _refreshToken;
            int _refreshTime;

    };
}


#endif //CLIENT_OATHCREDENTIALS_H
