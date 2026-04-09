#pragma once
#include <QWidget>
#include <QVBoxLayout>
#include <QListWidget>
#include <QPushButton>
#include "SupabaseClient.h"

// SentezStore Arayüzü: Paket yöneticisi front-end'i
class StoreWindow : public QWidget {
    Q_OBJECT
public:
    explicit StoreWindow(QWidget *parent = nullptr);

private slots:
    void populateList(const QString& jsonString);
    void installSelectedApp();

private:
    QVBoxLayout* m_layout;
    QListWidget* m_appList;
    QPushButton* m_installBtn;
    SupabaseClient* m_supabase;
};
