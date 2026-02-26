#pragma once

#include <QMainWindow>
#include <QNetworkAccessManager>

class QTableWidget;
class QLineEdit;
class QPushButton;
class QLabel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private:
    // UI
    QTableWidget *m_table = nullptr;
    QLineEdit *m_idEdit = nullptr;
    QLineEdit *m_branchEdit = nullptr;
    QLineEdit *m_modelEdit = nullptr;
    QLabel *m_status = nullptr;

    QPushButton *m_refreshBtn = nullptr;
    QPushButton *m_addBtn = nullptr;
    QPushButton *m_updateBtn = nullptr;
    QPushButton *m_deleteBtn = nullptr;

    // Network
    QNetworkAccessManager m_net;
    const QString m_baseUrl = QStringLiteral("http://localhost:3000/api");

    void buildUi();

    void refreshCars();
    void addCar();
    void updateCar();
    void deleteCar();

    void setStatus(const QString &text);
    static QByteArray jsonBody(const QString &branch, const QString &model);
};
