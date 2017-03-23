install(FILES
    wiktionary_sound.desktop
    wiktionary_sound.py
    DESTINATION ${KDE_INSTALL_DATADIR}/parley/plugins)

install(FILES
    mwclient/__init__.py
    mwclient/client.py
    mwclient/errors.py
    mwclient/ex.py
    mwclient/image.py
    mwclient/listing.py
    mwclient/LICENSE.md
    mwclient/page.py
    mwclient/README.rst
    mwclient/sleep.py
    mwclient/util.py
    DESTINATION ${KDE_INSTALL_DATADIR}/parley/plugins/mwclient)

install(FILES
    mwclient/six.py
    mwclient/six.LICENSE
    mwclient/six.README
    DESTINATION ${KDE_INSTALL_DATADIR}/parley/plugins/mwclient)

install(FILES
    mwclient/requests_oauthlib/__init__.py
    mwclient/requests_oauthlib/oauth1_auth.py
    mwclient/requests_oauthlib/oauth1_session.py
    mwclient/requests_oauthlib/oauth2_auth.py
    mwclient/requests_oauthlib/oauth2_session.py
    DESTINATION ${KDE_INSTALL_DATADIR}/parley/plugins/mwclient/requests_oauthlib)

install(FILES
    mwclient/requests_oauthlib/compliance_fixes/__init__.py
    mwclient/requests_oauthlib/compliance_fixes/douban.py
    mwclient/requests_oauthlib/compliance_fixes/facebook.py
    mwclient/requests_oauthlib/compliance_fixes/fitbit.py
    mwclient/requests_oauthlib/compliance_fixes/linkedin.py
    mwclient/requests_oauthlib/compliance_fixes/mailchimp.py
    mwclient/requests_oauthlib/compliance_fixes/slack.py
    mwclient/requests_oauthlib/compliance_fixes/weibo.py
    DESTINATION ${KDE_INSTALL_DATADIR}/parley/plugins/mwclient/requests_oauthlib/compliance_fixes)
