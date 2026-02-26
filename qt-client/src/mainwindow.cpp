#include "mainwindow.h"

#include <QHBoxLayout>
#include <QHeaderView>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QNetworkReply>
#include <QPushButton>
#include <QTableWidget>
#include <QUrl>
#include <QVBoxLayout>
#include <QWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    buildUi();
    refreshCars();
}

void MainWindow::buildUi()
{
    auto *central = new QWidget(this);
    auto *root = new QVBoxLayout(central);

    // Table
    m_table = new QTableWidget(0, 3, central);
    m_table->setHorizontalHeaderLabels({"ID", "Branch", "Model"});
    m_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_table->setSelectionMode(QAbstractItemView::SingleSelection);
    m_table->setEditTriggers(QAbstractItemView::NoEditTriggers);

    root->addWidget(m_table);

    // Form
    auto *formRow = new QHBoxLayout();
    formRow->addWidget(new QLabel("ID", central));
    m_idEdit = new QLineEdit(central);
    m_idEdit->setPlaceholderText("Select a row or type an ID");
    formRow->addWidget(m_idEdit);

    formRow->addWidget(new QLabel("Branch", central));
    m_branchEdit = new QLineEdit(central);
    m_branchEdit->setPlaceholderText("e.g. Toyota");
    formRow->addWidget(m_branchEdit);

    formRow->addWidget(new QLabel("Model", central));
    m_modelEdit = new QLineEdit(central);
    m_modelEdit->setPlaceholderText("e.g. Corolla");
    formRow->addWidget(m_modelEdit);

    root->addLayout(formRow);

    // Buttons
    auto *btnRow = new QHBoxLayout();
    m_refreshBtn = new QPushButton("Refresh", central);
    m_addBtn = new QPushButton("Add", central);
    m_updateBtn = new QPushButton("Update", central);
    m_deleteBtn = new QPushButton("Delete", central);

    btnRow->addWidget(m_refreshBtn);
    btnRow->addWidget(m_addBtn);
    btnRow->addWidget(m_updateBtn);
    btnRow->addWidget(m_deleteBtn);
    btnRow->addStretch(1);

    root->addLayout(btnRow);

    // Status
    m_status = new QLabel("Ready", central);
    root->addWidget(m_status);

    setCentralWidget(central);
    setWindowTitle("Car Table Client (Qt + REST API)");
    resize(900, 500);

    // Signals
    connect(m_refreshBtn, &QPushButton::clicked, this, &MainWindow::refreshCars);
    connect(m_addBtn, &QPushButton::clicked, this, &MainWindow::addCar);
    connect(m_updateBtn, &QPushButton::clicked, this, &MainWindow::updateCar);
    connect(m_deleteBtn, &QPushButton::clicked, this, &MainWindow::deleteCar);

    connect(m_table, &QTableWidget::cellClicked, this, [this](int row, int) {
        const auto *idItem = m_table->item(row, 0);
        const auto *branchItem = m_table->item(row, 1);
        const auto *modelItem = m_table->item(row, 2);
        if (!idItem || !branchItem || !modelItem) {
            return;
        }
        m_idEdit->setText(idItem->text());
        m_branchEdit->setText(branchItem->text());
        m_modelEdit->setText(modelItem->text());
    });
}

void MainWindow::setStatus(const QString &text)
{
    m_status->setText(text);
}

QByteArray MainWindow::jsonBody(const QString &branch, const QString &model)
{
    QJsonObject obj;
    obj.insert("branch", branch);
    obj.insert("model", model);
    return QJsonDocument(obj).toJson(QJsonDocument::Compact);
}

void MainWindow::refreshCars()
{
    setStatus("Fetching cars...");

    QNetworkRequest req(QUrl(m_baseUrl + "/cars"));
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QNetworkReply *reply = m_net.get(req);
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        reply->deleteLater();

        if (reply->error() != QNetworkReply::NoError) {
            setStatus("Failed to fetch cars");
            QMessageBox::critical(this, "Network Error", reply->errorString());
            return;
        }

        const QByteArray body = reply->readAll();
        const QJsonDocument doc = QJsonDocument::fromJson(body);
        if (!doc.isArray()) {
            setStatus("Invalid JSON from server");
            QMessageBox::critical(this, "Parse Error", "Expected JSON array from /api/cars");
            return;
        }

        const QJsonArray arr = doc.array();
        m_table->setRowCount(0);
        m_table->setRowCount(arr.size());

        for (int i = 0; i < arr.size(); ++i) {
            const QJsonObject obj = arr.at(i).toObject();
            const int id = obj.value("id").toInt();
            const QString branch = obj.value("branch").toString();
            const QString model = obj.value("model").toString();

            m_table->setItem(i, 0, new QTableWidgetItem(QString::number(id)));
            m_table->setItem(i, 1, new QTableWidgetItem(branch));
            m_table->setItem(i, 2, new QTableWidgetItem(model));
        }

        setStatus(QString("Loaded %1 row(s)").arg(arr.size()));
    });
}

void MainWindow::addCar()
{
    const QString branch = m_branchEdit->text().trimmed();
    const QString model = m_modelEdit->text().trimmed();

    if (branch.isEmpty() || model.isEmpty()) {
        QMessageBox::warning(this, "Validation", "Both branch and model are required.");
        return;
    }

    setStatus("Adding car...");

    QNetworkRequest req(QUrl(m_baseUrl + "/cars"));
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QNetworkReply *reply = m_net.post(req, jsonBody(branch, model));
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        reply->deleteLater();

        if (reply->error() != QNetworkReply::NoError) {
            setStatus("Add failed");
            QMessageBox::critical(this, "Network Error", reply->errorString());
            return;
        }

        setStatus("Car added");
        refreshCars();
    });
}

void MainWindow::updateCar()
{
    bool ok = false;
    const int id = m_idEdit->text().trimmed().toInt(&ok);
    if (!ok || id <= 0) {
        QMessageBox::warning(this, "Validation", "A valid ID is required for update.");
        return;
    }

    const QString branch = m_branchEdit->text().trimmed();
    const QString model = m_modelEdit->text().trimmed();

    if (branch.isEmpty() || model.isEmpty()) {
        QMessageBox::warning(this, "Validation", "Both branch and model are required.");
        return;
    }

    setStatus("Updating car...");

    QNetworkRequest req(QUrl(m_baseUrl + QString("/cars/%1").arg(id)));
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QNetworkReply *reply = m_net.put(req, jsonBody(branch, model));
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        reply->deleteLater();

        if (reply->error() != QNetworkReply::NoError) {
            setStatus("Update failed");
            QMessageBox::critical(this, "Network Error", reply->errorString());
            return;
        }

        setStatus("Car updated");
        refreshCars();
    });
}

void MainWindow::deleteCar()
{
    bool ok = false;
    const int id = m_idEdit->text().trimmed().toInt(&ok);
    if (!ok || id <= 0) {
        QMessageBox::warning(this, "Validation", "A valid ID is required for delete.");
        return;
    }

    if (QMessageBox::question(this, "Confirm", QString("Delete car with ID %1?").arg(id)) != QMessageBox::Yes) {
        return;
    }

    setStatus("Deleting car...");

    QNetworkRequest req(QUrl(m_baseUrl + QString("/cars/%1").arg(id)));
    QNetworkReply *reply = m_net.deleteResource(req);

    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        reply->deleteLater();

        if (reply->error() != QNetworkReply::NoError) {
            setStatus("Delete failed");
            QMessageBox::critical(this, "Network Error", reply->errorString());
            return;
        }

        setStatus("Car deleted");
        refreshCars();
    });
}
