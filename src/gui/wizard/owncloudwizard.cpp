/*
 * Copyright (C) by Klaas Freitag <freitag@owncloud.com>
 * Copyright (C) by Krzesimir Nowak <krzesimir@endocode.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
 * for more details.
 */

#include "account.h"
#include "configfile.h"
#include "theme.h"

#include "wizard/owncloudwizard.h"
#include "wizard/owncloudsetuppage.h"
#include "wizard/owncloudhttpcredspage.h"
#ifndef NO_SHIBBOLETH
#include "wizard/owncloudshibbolethcredspage.h"

#endif
#include "wizard/googleoauth2page.h"

#include "wizard/owncloudadvancedsetuppage.h"
#include "wizard/owncloudwizardresultpage.h"

#include "QProgressIndicator.h"

#include <QtCore>
#include <QtGui>
#include <stdlib.h>

namespace OCC
{

OwncloudWizard::OwncloudWizard(QWidget *parent)
    : QWizard(parent),
      _account(0),
      _oauthPage(new GoogleOauth2Page(this)),
      _resultPage(new OwncloudWizardResultPage),
      _advanceSetupPage(new OwncloudAdvancedSetupPage()),
      _credentialsPage(0),
      _setupLog()
{
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setPage(WizardCommon::Page_Oauth2, _oauthPage);
    setPage(WizardCommon::Page_AdvancedSetup,_advanceSetupPage);
    setPage(WizardCommon::Page_Result, _resultPage);

    connect(this, SIGNAL(finished(int)), SIGNAL(basicSetupFinished(int)));

    // note: start Id is set by the calling class depending on if the
    // welcome text is to be shown or not.
    setWizardStyle( QWizard::ModernStyle );

    connect( this, SIGNAL(currentIdChanged(int)), SLOT(slotCurrentPageChanged(int)));
    connect( _advanceSetupPage, SIGNAL(createLocalAndRemoteFolders(QString, QString)),
             SIGNAL(createLocalAndRemoteFolders(QString, QString)));
    connect(this, SIGNAL(customButtonClicked(int)), this, SIGNAL(skipFolderConfiguration()));


    Theme *theme = Theme::instance();
    setWindowTitle( tr("%1 Connection Wizard").arg(theme->appNameGUI()));
    setWizardStyle(QWizard::ModernStyle);
    setPixmap( QWizard::BannerPixmap, theme->wizardHeaderBanner() );
    setPixmap( QWizard::LogoPixmap, theme->wizardHeaderLogo() );
    setOption( QWizard::NoBackButtonOnStartPage );
    setOption( QWizard::NoBackButtonOnLastPage );
    setOption( QWizard::NoCancelButton );
    setTitleFormat(Qt::RichText);
    setSubTitleFormat(Qt::RichText);
    setButtonText(QWizard::CustomButton1, tr("Skip folders configuration"));
}

void OwncloudWizard::setAccount(AccountPtr account)
{
    _account = account;
}

AccountPtr OwncloudWizard::account() const
{
    return _account;
}

QString OwncloudWizard::localFolder() const
{
    return(_advanceSetupPage->localFolder());
}

QStringList OwncloudWizard::selectiveSyncBlacklist() const
{
    return _advanceSetupPage->selectiveSyncBlacklist();
}

bool OwncloudWizard::isConfirmBigFolderChecked() const
{
    return _advanceSetupPage->isConfirmBigFolderChecked();
}

QString OwncloudWizard::ocUrl() const
{
    QString url = field("OCUrl").toString().simplified();
    return url;
}

void OwncloudWizard::enableFinishOnResultWidget(bool enable)
{
    _resultPage->setComplete(enable);
}

void OwncloudWizard::setRemoteFolder( const QString& remoteFolder )
{
    _advanceSetupPage->setRemoteFolder( remoteFolder );
    _resultPage->setRemoteFolder( remoteFolder );
}

void OwncloudWizard::successfulStep()
{
    const int id(currentId());

    /*
    switch (id) {
    case WizardCommon::Page_HttpCreds:
        _httpCredsPage->setConnected();
        break;

    case WizardCommon::Page_AdvancedSetup:
        _advancedSetupPage->directoriesCreated();
        break;

    case WizardCommon::Page_ServerSetup:
    case WizardCommon::Page_Result:
        qWarning("Should not happen at this stage.");
        break;
    }*/

    next();
}

void OwncloudWizard::setAuthType(WizardCommon::AuthType type)
{
    /*
  _setupPage->setAuthType(type);
#ifndef NO_SHIBBOLETH
  if (type == WizardCommon::Shibboleth) {
    _credentialsPage = _shibbolethCredsPage;
  } else
#endif
  {
    _credentialsPage = _httpCredsPage;
  }*/
  next();
}

// TODO: update this function
void OwncloudWizard::slotCurrentPageChanged( int id )
{
    qDebug() << "Current Wizard page changed to " << id;

   /* if( id == WizardCommon::Page_ServerSetup ) {
        emit clearPendingRequests();
    }*/

    if( id == WizardCommon::Page_Result ) {
        disconnect(this, SIGNAL(finished(int)), this, SIGNAL(basicSetupFinished(int)));
        emit basicSetupFinished(QDialog::Accepted);
        appendToConfigurationLog( QString::null );
    }

    setOption(QWizard::HaveCustomButton1, id == WizardCommon::Page_AdvancedSetup);
}

void OwncloudWizard::displayError( const QString& msg, bool retryHTTPonly )
{
  /*  switch (currentId()) {
    case WizardCommon::Page_ServerSetup:
        _setupPage->setErrorString( msg, retryHTTPonly );
        break;

    case WizardCommon::Page_HttpCreds:
        _httpCredsPage->setErrorString(msg);
        break;

    case WizardCommon::Page_AdvancedSetup:
        _advancedSetupPage->setErrorString(msg);
        break;
    }*/
}

void OwncloudWizard::appendToConfigurationLog( const QString& msg, LogType /*type*/ )
{
    _setupLog << msg;
    qDebug() << "Setup-Log: " << msg;
}

void OwncloudWizard::setOCUrl( const QString& url )
{
  //_setupPage->setServerUrl( url );
}

AbstractCredentials* OwncloudWizard::getCredentials() const
{
  if (_credentialsPage) {
    return _credentialsPage->getCredentials();
  }

  return 0;
}

} // end namespace
