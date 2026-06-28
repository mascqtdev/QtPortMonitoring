#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QTimer>
#include <QComboBox>
#include <QVBoxLayout>
#include "PortMonitor.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void updatePortTable();

private:
    void setupUI();

    QTableWidget *portTable;
    QComboBox *protocolSelector;
    QTimer *updateTimer;

    PortMonitor monitor;
};

#endif // MAINWINDOW_H