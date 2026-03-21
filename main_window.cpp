#include "main_window.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle("Студенческий Планировщик");

    QWidget *central = new QWidget(this);
    setCentralWidget(central);
    
    //основной лейаут
    QHBoxLayout *mainLayout = new QHBoxLayout(central);

    //ввод данных
    QVBoxLayout *leftLayout = new QVBoxLayout();
    
    QFormLayout *form = new QFormLayout();
    editTaskName = new QLineEdit();
    editTaskName->setPlaceholderText("Что нужно сделать?");
    
    dateDeadline = new QDateEdit(QDate::currentDate());
    dateDeadline->setCalendarPopup(true);
    
    comboPriority = new QComboBox();
    comboPriority->addItems({"Низкий", "Средний", "Критический"});
    
    form->addRow("Задача:", editTaskName);
    form->addRow("Дедлайн:", dateDeadline);
    form->addRow("Приоритет:", comboPriority);
    
    btnAdd = new QPushButton("Добавить в список");
    btnAdd->setEnabled(false);
    btnAdd->setStyleSheet("background-color: #4CAF50; color: white; font-weight: bold; padding: 5px;");

    calendar = new QCalendarWidget();
    calendar->setGridVisible(true);

    leftLayout->addLayout(form);
    leftLayout->addWidget(new QLabel("Визуальный календарь:"));
    leftLayout->addWidget(calendar);
    leftLayout->addWidget(btnAdd);
    leftLayout->addStretch();

    //список
    QVBoxLayout *rightLayout = new QVBoxLayout();
    listTasks = new QListWidget();
    
    btnDelete = new QPushButton("Удалить выбранную задачу");
    btnDelete->setStyleSheet("background-color: #f44336; color: white; padding: 5px;");

    statusLabel = new QLabel("Задач пока нет");
    statusLabel->setAlignment(Qt::AlignCenter);

    rightLayout->addWidget(new QLabel("Ваш список дел:"));
    rightLayout->addWidget(listTasks);
    rightLayout->addWidget(btnDelete);
    rightLayout->addWidget(statusLabel);

    mainLayout->addLayout(leftLayout, 1);
    mainLayout->addLayout(rightLayout, 1);

    //сигналы и слоты 
    
    //событие изменения даты
    connect(dateDeadline, &QDateEdit::dateChanged, this, &MainWindow::updateCalendar);
    connect(calendar, &QCalendarWidget::clicked, this, &MainWindow::updateDateEdit);

    //кнопка доступна только при наличии текста
    connect(editTaskName, &QLineEdit::textChanged, this, &MainWindow::validateInput);

    //добавление задачи
    connect(btnAdd, &QPushButton::clicked, this, &MainWindow::addTask);
    
    //удаление задачи
    connect(btnDelete, &QPushButton::clicked, this, &MainWindow::deleteTask);
    new QShortcut(QKeySequence(Qt::Key_Delete), listTasks, SLOT(deleteTask()));
}

void MainWindow::validateInput(const QString &text) {
    btnAdd->setEnabled(!text.trimmed().isEmpty());
}

void MainWindow::updateCalendar(const QDate &date) {
    calendar->setSelectedDate(date);
}

void MainWindow::updateDateEdit(const QDate &date) {
    dateDeadline->setDate(date);
}

void MainWindow::addTask() {
    QString task = editTaskName->text();
    QString dateStr = dateDeadline->date().toString("dd.MM.yyyy");
    QString priority = comboPriority->currentText();

    QListWidgetItem *item = new QListWidgetItem();
    item->setText(QString("[%1] %2 — %3").arg(dateStr, priority, task));

    //дизайн в зависимости от приоритета
    if (priority == "Критический") {
        item->setForeground(Qt::red);
        QFont font = item->font();
        font.setBold(true);
        item->setFont(font);
    } else if (priority == "Средний") {
        item->setForeground(QColor(255, 140, 0));
    }

    listTasks->addItem(item);
    
    //очистка и обновление статуса
    editTaskName->clear();
    statusLabel->setText(QString("Всего задач: %1").arg(listTasks->count()));
}

void MainWindow::deleteTask() {
    QListWidgetItem *item = listTasks->currentItem();
    if (item) {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Удаление", "Удалить эту задачу?",
                                    QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            delete item;
            int count = listTasks->count();
            statusLabel->setText(count > 0 ? QString("Всего задач: %1").arg(count) : "Задач пока нет");
        }
    }
}