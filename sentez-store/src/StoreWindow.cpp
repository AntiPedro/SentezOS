#include "StoreWindow.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QProcess>
#include <QMessageBox>
#include <QDebug>

StoreWindow::StoreWindow(QWidget *parent) : QWidget(parent) {
    m_layout = new QVBoxLayout(this);
    m_layout->setSpacing(15);

    m_appList = new QListWidget(this);
    m_appList->setStyleSheet("QListWidget { background-color: #1A1A1A; border: 1px solid #333; border-radius: 8px; padding: 10px; font-size: 16px;}");

    m_installBtn = new QPushButton("İndir ve Kur", this);
    m_installBtn->setMinimumHeight(45);

    m_layout->addWidget(m_appList);
    m_layout->addWidget(m_installBtn);

    m_supabase = new SupabaseClient(this);
    connect(m_supabase, &SupabaseClient::appsFetched, this, &StoreWindow::populateList);
    connect(m_installBtn, &QPushButton::clicked, this, &StoreWindow::installSelectedApp);

    // SentezStore açılır açılmaz cloud'dan paketleri çeker
    m_supabase->fetchApps();
}

void StoreWindow::populateList(const QString& jsonString) {
    QJsonDocument doc = QJsonDocument::fromJson(jsonString.toUtf8());
    QJsonArray apps = doc.array();

    for(const QJsonValue& val : apps) {
        QJsonObject obj = val.toObject();
        QString appName = obj["name"].toString();
        QString pkgName = obj["pacman_package"].toString();
        QString desc = obj["description"].toString();

        QListWidgetItem* item = new QListWidgetItem(appName + "\n" + desc);
        item->setData(Qt::UserRole, pkgName); // Arka planda pacman komutu için
        m_appList->addItem(item);
    }
}

void StoreWindow::installSelectedApp() {
    QListWidgetItem* selected = m_appList->currentItem();
    if(!selected) return;

    QString pkgName = selected->data(Qt::UserRole).toString();

    // SentezOS native installer (pacman binding)
    qDebug() << "SentezOS Installer -> Kuruluyor:" << pkgName;
    QProcess* pacman = new QProcess(this);
    // Gerçekte pkexec veya Polkit ajanı ile şifre isteyerek bash çalıştırılır
    pacman->start("pkexec", {"pacman", "-S", "--noconfirm", pkgName});
    
    connect(pacman, &QProcess::finished, this, [this, pkgName]() {
        QMessageBox::information(this, "SentezOS", pkgName + " başarıyla sisteme entegre edildi!");
    });
}
