#include "SupabaseClient.h"
#include <QNetworkRequest>
#include <QUrl>

SupabaseClient::SupabaseClient(QObject* parent) : QObject(parent), m_manager(new QNetworkAccessManager(this)) {
    // SentezOS Cloud API Credentials
    m_url = "https://your-supabase-url.supabase.co";
    m_key = "your-supabase-anon-key";
}

void SupabaseClient::fetchApps() {
    // Supabase DB'den mağazadaki uygulamaları çek (SentezPackages tablosu)
    QNetworkRequest request(QUrl(m_url + "/rest/v1/SentezPackages?select=*"));
    request.setRawHeader("apikey", m_key.toUtf8());
    request.setRawHeader("Authorization", ("Bearer " + m_key).toUtf8());

    QNetworkReply* reply = m_manager->get(request);
    connect(reply, &QNetworkReply::finished, this, [this, reply]() { onFetchFinished(reply); });
}

void SupabaseClient::onFetchFinished(QNetworkReply* reply) {
    if(reply->error() == QNetworkReply::NoError) {
        QString json = QString::fromUtf8(reply->readAll());
        emit appsFetched(json);
    } else {
        emit errorOccurred(reply->errorString());
    }
    reply->deleteLater();
}

void SupabaseClient::login(const QString& email, const QString& password) {
    // Sentez ID ile Giriş Yapma - Supabase Auth JSON POST
}
