#pragma once
#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>

// Cloud Entegrasyonu (SentezOS - Supabase)
class SupabaseClient : public QObject {
    Q_OBJECT
public:
    explicit SupabaseClient(QObject* parent = nullptr);
    void fetchApps();
    void login(const QString& email, const QString& password);

signals:
    void appsFetched(const QString& jsonString);
    void loginSuccess(const QString& token);
    void errorOccurred(const QString& errorMsg);

private slots:
    void onFetchFinished(QNetworkReply* reply);

private:
    QNetworkAccessManager* m_manager;
    QString m_url;
    QString m_key;
};
