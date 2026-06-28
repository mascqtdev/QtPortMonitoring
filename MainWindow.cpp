#include "MainWindow.h"
#include <QHeaderView>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setupUI();

    updateTimer = new QTimer(this);
    connect(updateTimer, &QTimer::timeout, this, &MainWindow::updatePortTable);
    updateTimer->start(2000);

    updatePortTable();
}

MainWindow::~MainWindow() = default;

void MainWindow::setupUI() {
    this->setWindowTitle("Cross-Platform Real-time Port Monitor (Qt)");
    this->resize(900, 500);

    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    QHBoxLayout *topLayout = new QHBoxLayout();
    QLabel *label = new QLabel("Select Protocol:", this);
    protocolSelector = new QComboBox(this);
    protocolSelector->addItems({"TCP Only", "UDP Only", "Both TCP/UDP"});

    connect(protocolSelector, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::updatePortTable);

    topLayout->addWidget(label);
    topLayout->addWidget(protocolSelector);
    topLayout->addStretch();
    mainLayout->addLayout(topLayout);

    portTable = new QTableWidget(this);
    portTable->setColumnCount(7);
    QStringList headers = {"Protocol", "Local IP", "Port", "State", "Process (PID)", "Company", "User"};
    portTable->setHorizontalHeaderLabels(headers);

    portTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    portTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    mainLayout->addWidget(portTable);
    setCentralWidget(centralWidget);
}

void MainWindow::updatePortTable() {
    int protocolType = protocolSelector->currentIndex() + 1;

    std::vector<PortInfo> activePorts = monitor.GetActivePorts(protocolType);

    portTable->setRowCount(0);

    for (size_t i = 0; i < activePorts.size(); ++i) {
        int row = portTable->rowCount();
        portTable->insertRow(row);

        const auto& info = activePorts[i];

        portTable->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(info.protocol)));
        portTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(info.localIp)));
        portTable->setItem(row, 2, new QTableWidgetItem(QString::number(info.port)));
        portTable->setItem(row, 3, new QTableWidgetItem(QString::fromStdString(info.state)));

        QString appAndPid = QString::fromStdString(info.appName);
        if (info.pid != "---") appAndPid += " (PID: " + QString::fromStdString(info.pid) + ")";
        portTable->setItem(row, 4, new QTableWidgetItem(appAndPid));

        portTable->setItem(row, 5, new QTableWidgetItem(QString::fromStdString(info.companyName)));
        portTable->setItem(row, 6, new QTableWidgetItem(QString::fromStdString(info.userName)));
    }
}