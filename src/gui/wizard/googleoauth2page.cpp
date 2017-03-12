#include "googleoauth2page.h"
#include "ui_googleoauth2page.h"
#include "owncloudwizardcommon.h"

#include <qwebframe.h>
#include "creds/o2replyserver.h"
#include "creds/oathcredentials2.h"

#include <QList>
#include <QPair>
#include <QDebug>
#include <QTcpServer>
#include <QMap>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QDateTime>
#include <QCryptographicHash>
#include <QTimer>
#include <QVariantMap>

namespace OCC {


    /// Add query parameters to a query
    static void addQueryParametersToUrl(QUrl &url,  QList<QPair<QString, QString> > parameters) {
#if QT_VERSION < 0x050000
        url.setQueryItems(parameters);
#else
        QUrlQuery query(url);
        query.setQueryItems(parameters);
        url.setQuery(query);
#endif
    }

    /// Parse JSON data into a QVariantMap
    static QVariantMap parseTokenResponse(const QByteArray &data) {
#if QT_VERSION >= 0x050000
        QJsonParseError err;
        QJsonDocument doc = QJsonDocument::fromJson(data, &err);
        if (err.error != QJsonParseError::NoError) {
            qWarning() << "parseTokenResponse: Failed to parse token response due to err:" << err.errorString();
            return QVariantMap();
        }

        if (!doc.isObject()) {
            qWarning() << "parseTokenResponse: Token response is not an object";
            return QVariantMap();
        }

        return doc.object().toVariantMap();
#else
        QScriptEngine engine;
    QScriptValue value = engine.evaluate("(" + QString(data) + ")");
    QScriptValueIterator it(value);
    QVariantMap map;

    while (it.hasNext()) {
        it.next();
        map.insert(it.name(), it.value().toVariant());
    }

    return map;
#endif
    }

    GoogleOauth2Page::GoogleOauth2Page() :
            QWizardPage(),
            _ui(),
            _hasTokenBeenRegistered(false)
    {
        _ui.setupUi(this);
        setSubTitle( QLatin1String(" ") );

        _manager = new QNetworkAccessManager(this);
        _replyServer = new O2ReplyServer(this);

        connect(_replyServer, SIGNAL(verificationReceived(QMap<QString,QString>)), this, SLOT(onVerificationReceived(QMap<QString,QString>)));
        _replyServer->listen(QHostAddress::Any,OAUTH_PORT);

        // Assemble intial authentication URL
        QList<QPair<QString, QString> > parameters;
        parameters.append(qMakePair(QString(OAUTH2_RESPONSE_TYPE),QString(OAUTH2_GRANT_TYPE_CODE)));
        parameters.append(qMakePair(QString(OAUTH2_CLIENT_ID), QString(CLIENT_ID)));
        parameters.append(qMakePair(QString(OAUTH2_REDIRECT_URI), QString(REDIRECT_URI).arg(OAUTH_PORT)));
        parameters.append(qMakePair(QString(OAUTH2_SCOPE), QString(SCOPE_URL)));
       // parameters.append(qMakePair(QString(OAUTH2_API_KEY), ""));


        QUrl url(ENDPOINT_URL);
        addQueryParametersToUrl(url, parameters);
        qDebug() << "O2::link: Emit openBrowser" << url.toString();
        this->_ui.loginview->setUrl(url);


    }


    QByteArray GoogleOauth2Page::buildRequestBody(const QMap<QString, QString> &parameters) {
        QByteArray body;
        bool first = true;
                foreach (QString key, parameters.keys()) {
                if (first) {
                    first = false;
                } else {
                    body.append("&");
                }
                QString value = parameters.value(key);
                body.append(QUrl::toPercentEncoding(key) + QString("=").toUtf8() + QUrl::toPercentEncoding(value));
            }
        return body;
    }


    void GoogleOauth2Page::onVerificationReceived(QMap<QString, QString> response) {
        qDebug() << "O2::onVerificationReceived:" << response;
        qDebug() << "O2::onVerificationReceived: Emitting closeBrowser()";

        // Save access code
        QString code(response.value(QString(OAUTH2_GRANT_TYPE_CODE)));

        // Exchange access code for access/refresh tokens
        QString query;
        QNetworkRequest tokenRequest(QUrl(QString(TOKENURL_URL) + query));
        tokenRequest.setHeader(QNetworkRequest::ContentTypeHeader, MIME_TYPE_XFORM);
        QMap<QString, QString> parameters;
        parameters.insert(OAUTH2_GRANT_TYPE_CODE, code);
        parameters.insert(OAUTH2_CLIENT_ID, CLIENT_ID);
        parameters.insert(OAUTH2_CLIENT_SECRET, CLIENT_SECRETE);
        parameters.insert(OAUTH2_REDIRECT_URI,  QString(REDIRECT_URI).arg(OAUTH_PORT));
        parameters.insert(OAUTH2_GRANT_TYPE, OAUTH2_AUTHORIZATION_CODE);
        QByteArray data = buildRequestBody(parameters);
        _networkReply = _manager->post(tokenRequest, data);

        connect(_networkReply, SIGNAL(finished()), this, SLOT(onTokenReplyFinished()), Qt::QueuedConnection);
        connect(_networkReply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(onTokenReplyError(QNetworkReply::NetworkError)), Qt::QueuedConnection);

        _timer = new QTimer(this);
        _timer->singleShot(6000,this,SLOT(onTimeout()));

    }

    void GoogleOauth2Page::onTokenReplyError(QNetworkReply::NetworkError error) {

    }

    void GoogleOauth2Page::onTokenReplyFinished() {
        qDebug() << "O2::onTokenReplyFinished";
        QNetworkReply *tokenReply = qobject_cast<QNetworkReply *>(sender());
        if (tokenReply->error() == QNetworkReply::NoError) {
            QByteArray replyData = tokenReply->readAll();
            QVariantMap tokens = parseTokenResponse(replyData);

            // Check for mandatory tokens
            if (tokens.contains(OAUTH2_ACCESS_TOKEN)) {
                QString accessToken = tokens.take(OAUTH2_ACCESS_TOKEN).toString();

                bool ok = false;
                int expiresIn = tokens.take(OAUTH2_EXPIRES_IN).toInt(&ok);
                if (ok) {
                    qDebug() << "O2::onTokenReplyFinished: Token expires in" << expiresIn << "seconds";
                    expiresIn = (QDateTime::currentMSecsSinceEpoch() / 1000 + expiresIn);
                }
                QString refreshToken = tokens.take(OAUTH2_REFRESH_TOKEN).toString();
                Q_EMIT registerOauth2(accessToken,refreshToken,expiresIn);
                _hasTokenBeenRegistered= true;
            } else {
                qWarning() << "O2::onTokenReplyFinished: oauth_token missing from response" << replyData;
                Q_EMIT returnFail();
            }
        }
        _timer->destroyed();
        _networkReply->deleteLater();

    }

    void GoogleOauth2Page::onTimeout() {
        _networkReply->error(QNetworkReply::TimeoutError);
    }


    bool GoogleOauth2Page::isComplete() const {
        return _hasTokenBeenRegistered;
    }

   int GoogleOauth2Page::nextId() const {
        return WizardCommon::Page_Result;
    }


    GoogleOauth2Page::~GoogleOauth2Page() {
        delete _replyServer;
        delete _manager;
        delete _timer;
        delete _networkReply;
    }

};
