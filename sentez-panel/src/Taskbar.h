#pragma once
#include <QWidget>
#include <QHBoxLayout>
#include <QString>

class Taskbar : public QWidget {
    Q_OBJECT

public:
    explicit Taskbar(QWidget *parent = nullptr);

private slots:
    void handleWindowClose(const QString& windowId);
    void handleWindowFocus(const QString& windowId);
    void handleWindowPin(const QString& windowId);

private:
    QHBoxLayout *m_layout;
    
    void updateWindowList();
    void addWindowToTaskbar(const QString& windowId, const QString& title);
};
