#include "main_window.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QRandomGenerator>
#include <QColor>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), m_currentIndex(-1), m_isUpdatingUI(false) {
    
    setupUi();
    
    //начальное число билетов
    spinCount->setValue(10);
}

void MainWindow::setupUi() {
    setWindowTitle("Procrastination: Подготовка к экзамену");

    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    //Основной лэйаут: слева список, справа управление билетом
    QHBoxLayout* mainLayout = new QHBoxLayout(centralWidget);

    //левая панель
    QVBoxLayout* leftLayout = new QVBoxLayout();
    
    QHBoxLayout* countLayout = new QHBoxLayout();
    countLayout->addWidget(new QLabel("Количество билетов:"));
    spinCount = new QSpinBox();
    spinCount->setMinimum(1);
    spinCount->setMaximum(1000);
    countLayout->addWidget(spinCount);
    leftLayout->addLayout(countLayout);

    listTickets = new QListWidget();
    leftLayout->addWidget(listTickets);

    leftLayout->addWidget(new QLabel("Общий прогресс (Желтые + Зеленые):"));
    progTotal = new QProgressBar();
    leftLayout->addWidget(progTotal);

    leftLayout->addWidget(new QLabel("Прогресс выученного (Только зеленые):"));
    progGreen = new QProgressBar();
    progGreen->setStyleSheet("QProgressBar::chunk { background-color: #90EE90; }");
    leftLayout->addWidget(progGreen);

    //правая панель
    groupQuestion = new QGroupBox("Информация о билете");
    QVBoxLayout* rightLayout = new QVBoxLayout(groupQuestion);

    lblNumber = new QLabel("Выберите билет слева");
    lblName = new QLabel("");
    QFont nameFont = lblName->font();
    nameFont.setBold(true);
    nameFont.setPointSize(12);
    lblName->setFont(nameFont);

    editName = new QLineEdit();
    editName->setPlaceholderText("Введите новое имя и нажмите Enter...");

    comboStatus = new QComboBox();
    comboStatus->addItem("Не трогали (Серый)");
    comboStatus->addItem("Нужно повторить (Желтый)");
    comboStatus->addItem("Выучен (Зеленый)");

    QHBoxLayout* buttonsLayout = new QHBoxLayout();
    btnPrev = new QPushButton("<- Предыдущий");
    btnNext = new QPushButton("Следующий рандомный ->");
    buttonsLayout->addWidget(btnPrev);
    buttonsLayout->addWidget(btnNext);

    rightLayout->addWidget(lblNumber);
    rightLayout->addWidget(lblName);
    rightLayout->addWidget(new QLabel("Изменить название:"));
    rightLayout->addWidget(editName);
    rightLayout->addWidget(new QLabel("Статус:"));
    rightLayout->addWidget(comboStatus);
    rightLayout->addStretch();
    rightLayout->addLayout(buttonsLayout);

    groupQuestion->setEnabled(false);

    //основной лэйаут
    mainLayout->addLayout(leftLayout, 1);
    mainLayout->addWidget(groupQuestion, 1);

    //сигналы
    connect(spinCount, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::onCountChanged);
    connect(listTickets, &QListWidget::itemClicked, this, &MainWindow::onTicketClicked);
    connect(listTickets, &QListWidget::itemDoubleClicked, this, &MainWindow::onTicketDoubleClicked);
    connect(editName, &QLineEdit::returnPressed, this, &MainWindow::onNameReturnPressed);
    connect(comboStatus, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::onStatusChanged);
    connect(btnNext, &QPushButton::clicked, this, &MainWindow::onNextClicked);
    connect(btnPrev, &QPushButton::clicked, this, &MainWindow::onPrevClicked);
}

void MainWindow::onCountChanged(int count) {
    m_tickets.clear();
    m_history.clear();
    listTickets->clear();
    m_currentIndex = -1;

    m_tickets.resize(count);
    for (int i = 0; i < count; ++i) {
        m_tickets[i].name = QString("Билет %1").arg(i + 1);
        m_tickets[i].status = StatusDefault;

        QListWidgetItem* item = new QListWidgetItem();
        listTickets->addItem(item);
        updateTicketViewInList(i);
    }

    progTotal->setMaximum(count);
    progGreen->setMaximum(count);
    updateProgressBars();
    
    groupQuestion->setEnabled(false);
    updateButtonsState();
}

void MainWindow::updateTicketViewInList(int index) {
    if (index < 0 || index >= m_tickets.size()) return;

    QListWidgetItem* item = listTickets->item(index);
    item->setText(QString("%1: %2").arg(index + 1).arg(m_tickets[index].name));

    QColor bgColor;
    switch (m_tickets[index].status) {
        case StatusDefault: bgColor = QColor(255, 150, 150); break;
        case StatusYellow:  bgColor = QColor(255, 255, 153); break;
        case StatusGreen:   bgColor = QColor(144, 238, 144); break;
    }
    item->setBackground(bgColor);
}

void MainWindow::updateProgressBars() {
    int totalProgress = 0;
    int greenProgress = 0;

    for (const auto& ticket : m_tickets) {
        if (ticket.status == StatusYellow || ticket.status == StatusGreen) totalProgress++;
        if (ticket.status == StatusGreen) greenProgress++;
    }

    progTotal->setValue(totalProgress);
    progGreen->setValue(greenProgress);
}

void MainWindow::onTicketClicked(QListWidgetItem* item) {
    int row = listTickets->row(item);
    if (row != m_currentIndex) {
        if (m_currentIndex != -1) {
            m_history.push_back(m_currentIndex);
        }
        selectTicket(row);
    }
}

void MainWindow::onTicketDoubleClicked(QListWidgetItem* item) {
    int row = listTickets->row(item);
    if (m_tickets[row].status == StatusDefault || m_tickets[row].status == StatusYellow) {
        m_tickets[row].status = StatusGreen;
    } else if (m_tickets[row].status == StatusGreen) {
        m_tickets[row].status = StatusYellow;
    }
    
    updateTicketViewInList(row);
    updateProgressBars();
    
    if (row == m_currentIndex) {
        m_isUpdatingUI = true;
        comboStatus->setCurrentIndex(m_tickets[row].status);
        m_isUpdatingUI = false;
    }
    updateButtonsState();
}

void MainWindow::selectTicket(int index) {
    if (index < 0 || index >= m_tickets.size()) return;

    m_currentIndex = index;
    listTickets->setCurrentRow(index);
    
    m_isUpdatingUI = true;
    
    groupQuestion->setEnabled(true);
    lblNumber->setText(QString("Номер билета: %1").arg(index + 1));
    lblName->setText(m_tickets[index].name);
    editName->clear();
    comboStatus->setCurrentIndex(m_tickets[index].status);
    
    m_isUpdatingUI = false;
    updateButtonsState();
}

void MainWindow::onNameReturnPressed() {
    QString newName = editName->text().trimmed();
    if (newName.isEmpty() || m_currentIndex == -1) return;

    m_tickets[m_currentIndex].name = newName;
    lblName->setText(newName);
    updateTicketViewInList(m_currentIndex);
    editName->clear();
}

void MainWindow::onStatusChanged(int index) {
    if (m_isUpdatingUI || m_currentIndex == -1) return;

    m_tickets[m_currentIndex].status = static_cast<TicketStatus>(index);
    updateTicketViewInList(m_currentIndex);
    updateProgressBars();
    updateButtonsState();
}

void MainWindow::onNextClicked() {
    std::vector<int> available;
    for (int i = 0; i < m_tickets.size(); ++i) {
        if (m_tickets[i].status == StatusDefault || m_tickets[i].status == StatusYellow) {
            available.push_back(i);
        }
    }

    if (!available.empty()) {
        int randomIdx = available[QRandomGenerator::global()->bounded(static_cast<int>(available.size()))];
        if (m_currentIndex != -1) m_history.push_back(m_currentIndex);
        selectTicket(randomIdx);
    }
}

void MainWindow::onPrevClicked() {
    if (!m_history.empty()) {
        int prevIdx = m_history.back();
        m_history.pop_back();
        selectTicket(prevIdx);
    }
}

void MainWindow::updateButtonsState() {
    btnPrev->setEnabled(!m_history.empty());

    bool hasPendingTickets = false;
    for (const auto& t : m_tickets) {
        if (t.status == StatusDefault || t.status == StatusYellow) {
            hasPendingTickets = true;
            break;
        }
    }
    btnNext->setEnabled(hasPendingTickets);
}